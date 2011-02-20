#pragma once

#include "Includes.hpp"

using namespace std;
using namespace Common;

class FILEMANAGER_API WorldLoader
{
public:
    WorldLoader(const char * name);
    ~WorldLoader();

    bool LoadChunk(ivec3 position, size_t * blocks, void * data);
    bool SaveChunk(ivec3 position, size_t * blocks, void * data);

    void * Compress(size_t size, void * data, size_t * finalsize);
    void * Decompress(size_t size, void * data, size_t * finalsize);

private:
    sqlite3 * mDatabase;
    sqlite3_stmt * mStmtLoad, * mStmtSave;
};
