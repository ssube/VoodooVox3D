#include "WorldLoader.hpp"

#define IMPORT_WORLDMANAGER
#include "BlockTemplate.hpp"
#include "BlockDictionary.hpp"

static const uint32 chunkBlocks = CHUNK_BLOCKS * CHUNK_BLOCKS * CHUNK_BLOCKS;

WorldLoader * WorldLoader::Create(const char * name, BlockDictionary * dict)
{
    return new WorldLoader(name, dict);
}

void WorldLoader::Destroy()
{
    delete this;
}

WorldLoader::WorldLoader(const char * name, BlockDictionary * dict)
    : mDict(dict)
{
    int rc = sqlite3_open(name, &mDatabase);
    if ( rc )
    {
        fprintf(stderr, "Unable to open SQLite database (file \"%s\").\n", name);
        sqlite3_close(mDatabase);
        return;
    }

    sqlite3_exec(mDatabase, "CREATE TABLE IF NOT EXISTS 'chunks';", NULL, NULL, NULL);

    rc = sqlite3_prepare_v2(mDatabase, "SELECT blocks FROM chunks WHERE ( world = ?1 AND x = ?2 AND y = ?3 AND z = ?4 );", -1, &mStmtLoad, NULL);
    rc = sqlite3_prepare_v2(mDatabase, "INSERT OR REPLACE INTO chunks ( world, x, y, z, blocks ) VALUES ( ?1, ?2, ?3, ?4, ?5 );", -1, &mStmtSave, NULL);

    mMeta = new WorldMeta();

    this->BuildWorldMeta();
    this->BuildWorldDict();

    mNoise = new Noise::CoherentNoise(mMeta->WorldSeed);
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
    delete mNoise;
}

int BuildWorldMetaCallback(void * meta, int columns, char ** value, char ** colname)
{
    WorldMeta * Meta = (WorldMeta*)meta;

    Meta->WorldSeed    = atoi(value[0]);
    Meta->WaterLevel   = atof(value[1]);
    Meta->WorldSpawn.x = atof(value[2]);
    Meta->WorldSpawn.y = atof(value[3]);
    Meta->WorldSpawn.z = atof(value[4]);

    return 0;
}

void WorldLoader::BuildWorldMeta()
{
    char * error;

    sqlite3_exec(mDatabase, "SELECT seed, waterlevel, spawnx, spawny, spawnz FROM worlds LIMIT 1;", &BuildWorldMetaCallback, this->mMeta, &error);

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

bool WorldLoader::MakeChunk(ivec3 position, RawChunk * data)
{
    data->DataValid = false;
    data->ChunkPosition = position;
    data->BlockCount = chunkBlocks;

    vector<uint8> idList = mDict->GetTemplateList();

    float waterlevel = mMeta->WaterLevel;

    fprintf(stdout, "Generating world.\n");
    const float wbfloat = CHUNK_BLOCKS;
    const float chunkheight = position.y * CHUNK_SIZE;

    for ( size_t x = 0; x < CHUNK_BLOCKS; ++x )
    {
        for ( size_t z = 0; z < CHUNK_BLOCKS; ++z )
        {
            for ( size_t y = 0; y < CHUNK_BLOCKS; ++y )
            {
                fvec3 noisepoint(( x / wbfloat ) + position.x, 0, (z/wbfloat)+position.z);
                float hlimit = mNoise->GetOctavePoint(noisepoint, 4);
                float height = ( y / wbfloat ) + chunkheight;

                if ( height < hlimit )
                {
                    uint8 type = 1 + min(2.0f, ((y / wbfloat)+0.15f) * 2.0f);
                    data->BlockData[x][y][z][0] = idList[type];
                } else if ( height < waterlevel ) {
                    data->BlockData[x][y][z][0] = idList[3];
                } else {
                    data->BlockData[x][y][z][0] = 0;
                }
            }
        }
    }

    data->DataValid = true;

    this->SaveChunk(position, data);

    return true;
}

bool WorldLoader::LoadChunk(ivec3 position, RawChunk * data)
{
    data->DataValid = false;

    int rc = sqlite3_bind_int(mStmtLoad, 1, 1);
    rc = sqlite3_bind_int(mStmtLoad, 2, position.x);
    rc = sqlite3_bind_int(mStmtLoad, 3, position.y);
    rc = sqlite3_bind_int(mStmtLoad, 4, position.z);

    rc = sqlite3_step(mStmtLoad);

    if ( rc == SQLITE_ROW )
    {
        const void * cdata = sqlite3_column_blob(mStmtLoad, 0);

        if ( cdata )
        {
            data->ChunkPosition = position;

            int size = sqlite3_column_bytes(mStmtLoad, 0);
            data->DataValid = true;
            data->BlockCount = size / 2;

            data->BlockCount = chunkBlocks;

            memset(data->BlockData, 0, chunkBlocks * 2);
            memcpy(data->BlockData, cdata, chunkBlocks * 2);

            sqlite3_reset(mStmtLoad);

            data->DataValid = true;
            return true;
        }
    }

    sqlite3_reset(mStmtLoad);

    return this->MakeChunk(position, data);
}

bool WorldLoader::SaveChunk(ivec3 position, RawChunk * data)
{
    int rc = sqlite3_bind_int(mStmtLoad, 1, 1);
    rc = sqlite3_bind_int(mStmtSave, 2, position.x);
    rc = sqlite3_bind_int(mStmtSave, 3, position.y);
    rc = sqlite3_bind_int(mStmtSave, 4, position.z);
    rc = sqlite3_bind_blob(mStmtSave, 5, data->BlockData, chunkBlocks * 2, NULL);

    rc = sqlite3_step(mStmtSave);

    if ( rc == SQLITE_DONE )
    {
        sqlite3_reset(mStmtSave);
        return true;
    } else {
        sqlite3_reset(mStmtSave);
        return false;
    }
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
