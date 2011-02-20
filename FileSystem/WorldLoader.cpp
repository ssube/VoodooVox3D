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
}

WorldLoader::~WorldLoader()
{
    sqlite3_finalize(mStmtLoad);
    sqlite3_finalize(mStmtSave);

    if ( mDatabase )
    {
        sqlite3_close(mDatabase);
    }
}

bool WorldLoader::LoadChunk(ivec3 position, size_t * blocks, void * data)
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
            int size = sqlite3_column_bytes(mStmtLoad, 0);
            *blocks = size / 2;

            memcpy(data, cdata, size);
        } else {
            data = NULL;
            *blocks = 0;
        }

        sqlite3_reset(mStmtLoad);

        return true;
    } else {
        return false;
    }
}

bool WorldLoader::SaveChunk(ivec3 position, size_t * blocks, void * data)
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
    memcpy(endbuffer, buffer.begin()._Myptr, *finalsize);
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
    memcpy(endbuffer, buffer.begin()._Myptr, *finalsize);
    return endbuffer;
}
