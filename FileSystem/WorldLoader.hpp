#pragma once

#include "Includes.hpp"
#include "CoherentNoise.hpp"
#include "BlockDictionary.hpp"

using namespace std;
using namespace Common;

struct WorldMeta
{
    uint64 WorldSeed;
    fvec3  WorldSpawn;
};

struct RawChunk 
{
    bool DataValid, ChunkNew;
    ivec3 ChunkPosition;
    uint32 BlockCount;
    uint8 BlockData[CHUNK_BLOCKS][CHUNK_BLOCKS][CHUNK_BLOCKS][2];
};

class FILEMANAGER_API WorldLoader
{
public:
    WorldLoader(const char * name);
    ~WorldLoader();

    bool MakeChunk(ivec3 position, RawChunk ** data);
    bool LoadChunk(ivec3 position, RawChunk ** data);
    bool SaveChunk(ivec3 position, RawChunk * data);

    void * Compress  (uint32 size, void * data, uint32 * finalsize);
    void * Decompress(uint32 size, void * data, uint32 * finalsize);

protected:
    void BuildWorldMeta();
    void BuildWorldDict();

private:
    sqlite3 * mDatabase;
    sqlite3_stmt * mStmtLoad, * mStmtSave;
    Noise::CoherentNoise * mNoise;

    // World meta data
    WorldMeta * mMeta;
    BlockDictionary * mDict;
};
