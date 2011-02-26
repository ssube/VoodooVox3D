#include "WorldLoader.hpp"

WorldLoader::WorldLoader(const char * name)
{
    int rc = sqlite3_open(name, &mDatabase);
    if ( rc )
    {
        fprintf(stderr, "Unable to open SQLite database (file \"%s\").\n", name);
        sqlite3_close(mDatabase);
        return;
    }

    sqlite3_exec(mDatabase, "CREATE TABLE IF NOT EXISTS 'chunks';", NULL, NULL, NULL);

    rc = sqlite3_prepare_v2(mDatabase, "SELECT blocks FROM chunks WHERE ( x = ?1 AND y = ?2 AND z = ?3 );", -1, &mStmtLoad, NULL);
    rc = sqlite3_prepare_v2(mDatabase, "INSERT OR REPLACE INTO chunks ( px, py, pz, blocks ) VALUES ( ?1, ?2, ?3, ?4 );", -1, &mStmtSave, NULL);

    mMeta = new WorldMeta();
    mDict = new BlockDictionary();
}

WorldLoader::~WorldLoader()
{
    sqlite3_finalize(mStmtLoad);
    sqlite3_finalize(mStmtSave);

    if ( mDatabase )
    {
        sqlite3_close(mDatabase);
    }

    delete mMeta;
    delete mDict;
}

int BuildWorldMetaCallback(void * meta, int columns, char ** value, char ** colname)
{
    WorldMeta * Meta = (WorldMeta*)meta;

    if ( strcmp(colname[0], "seed") == 0 )
    {
        Meta->WorldSeed = atoi(value[0]);
        return 0;
    } else if ( strcmp(colname[0], "spawnx") == 0 ) {
        Meta->WorldSpawn.x = atof(value[0]);
        Meta->WorldSpawn.y = atof(value[1]);
        Meta->WorldSpawn.z = atof(value[2]);
        return 0;
    } else {
        fprintf(stderr, "SQLite Error: World loader called back with unknown stage.\n");
        return 1;
    }

    return 0;
}

void WorldLoader::BuildWorldMeta()
{
    char * error;

    sqlite3_exec(mDatabase, "SELECT seed FROM world LIMIT 1;", &BuildWorldMetaCallback, this->mMeta, &error);

    if ( error )
    {
        fprintf(stderr, "SQLite Error: %s\n", error);
        sqlite3_free(error);
        return;
    }

    sqlite3_exec(mDatabase, "SELECT spawnx, spawny, spawnz FROM world LIMIT 1;", &BuildWorldMetaCallback, this->mMeta, &error);

    if ( error )
    {
        fprintf(stderr, "SQLite Error: %s\n", error);
        sqlite3_free(error);
        return;
    }
}

int BuildWorldDictCallback(void * dict, int columns, char ** value, char ** colname)
{
    BlockDictionary * dictionary = (BlockDictionary*)dict;

    BlockTemplate * temp = new BlockTemplate();
    temp->ID = atoi(value[0]);
    strcpy_s(temp->Name, value[1]);
    temp->Texture = atoi(value[2]);

    temp->DefaultHealth = atoi(value[3]);

    temp->Speed = atof(value[4]);

    temp->Occludes = ( value[5][0] == '1' );
    temp->Visible = ( value[6][0] == '1' );

    dictionary->AddTemplate(temp);

    return 0;
}

void WorldLoader::BuildWorldDict()
{
    char * error;

    sqlite3_exec(mDatabase, "SELECT id, name, texture, health, speed, occludes, visible FROM blocktypes;", &BuildWorldDictCallback, this->mDict, &error);

    if ( error )
    {
        fprintf(stderr, "SQLite Error: %s\n", error);
        sqlite3_free(error);
        return;
    }
}

bool WorldLoader::MakeChunk(ivec3 position, RawChunk ** data)
{
    // Need to generate a chunk because it doesn't exist
    *data = nullptr;

    return true;
}

bool WorldLoader::LoadChunk(ivec3 position, RawChunk ** data)
{
    int rc = sqlite3_bind_int(mStmtLoad, 1, position.x);
    rc = sqlite3_bind_int(mStmtLoad, 1, position.y);
    rc = sqlite3_bind_int(mStmtLoad, 1, position.z);

    rc = sqlite3_step(mStmtLoad);

    if ( rc == SQLITE_ROW )
    {
        const void * cdata = sqlite3_column_blob(mStmtLoad, 0);

        if ( cdata )
        {
            RawChunk * rawChunk = new RawChunk();

            rawChunk->ChunkPosition = position;

            int size = sqlite3_column_bytes(mStmtLoad, 0);
            rawChunk->DataValid = true;
            rawChunk->BlockCount = size / 2;

            memset(rawChunk->BlockData, 0, sizeof(uint8) * CHUNK_BLOCKS * CHUNK_BLOCKS * CHUNK_BLOCKS * 2);
            memcpy(rawChunk->BlockData, cdata, size);

            *data = rawChunk;

            sqlite3_reset(mStmtLoad);
            return true;
        }
    }

    *data = nullptr;

    sqlite3_reset(mStmtLoad);
    return false;
}

bool WorldLoader::SaveChunk(ivec3 position, RawChunk * data)
{
    return false;
}

void * WorldLoader::Compress(size_t size, void * data, size_t * finalsize)
{
    vector<unsigned char> buffer;
    unsigned char * startbuffer = (unsigned char*)data;
    size_t pos = 0;

    while ( pos < size )
    {
        if ( pos == size-1 )
        {
            if ( startbuffer[pos] == 0xFF )
            {
                buffer.push_back(0xFF);
                buffer.push_back(1);
                buffer.push_back(0xFF);
            } else {
                buffer.push_back(startbuffer[pos]);
            }
            ++pos;
        } else {
            if ( startbuffer[pos] != startbuffer[pos+1] )
            {
                if ( startbuffer[pos] == 0xFF )
                {
                    buffer.push_back(0xFF);
                    buffer.push_back(1);
                    buffer.push_back(0xFF);
                } else {
                    buffer.push_back(startbuffer[pos]);
                }
                ++pos;
            } else {
                // May be the start of a run, find the end
                unsigned char runlen = 0;
                unsigned char runchar = startbuffer[pos];
                while ( startbuffer[pos] == runchar && runlen < 255 && pos < size )
                {
                    ++runlen;
                    ++pos;
                }

                if ( runlen > 3 || runchar == 0xFF )
                {
                    // Write the run
                    buffer.push_back(0xFF);
                    buffer.push_back(runlen);
                    buffer.push_back(runchar);
                } else {
                    for ( unsigned char i = 0; i < runlen; ++i )
                    {
                        buffer.push_back(runchar);
                    }
                }
            }
        }
    }

    *finalsize = buffer.size();
    void * endbuffer = malloc(buffer.size());
    memcpy(endbuffer, buffer.begin()._Ptr, *finalsize);
    return endbuffer;
}

void * WorldLoader::Decompress(size_t size, void * data, size_t * finalsize)
{
    vector<unsigned char> buffer;
    unsigned char * startbuffer = (unsigned char*)data;
    size_t pos = 0;

    while ( pos < size )
    {
        if ( startbuffer[pos] != 0xFF )
        {
            buffer.push_back(startbuffer[pos++]);
        } else {
            // Found a run, decompress
            unsigned char runlen = startbuffer[++pos];
            unsigned char runchar = startbuffer[++pos];

            for ( unsigned char i = 0; i < runlen; ++i )
            {
                buffer.push_back(runchar);
            }

            ++pos;
        }
    }

    *finalsize = buffer.size();
    void * endbuffer = malloc(buffer.size());
    memcpy(endbuffer, buffer.begin()._Ptr, *finalsize);
    return endbuffer;
}
