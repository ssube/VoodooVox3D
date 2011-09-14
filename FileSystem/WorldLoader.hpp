#pragma once

#include "Interface_WorldLoader.hpp"

#define IMPORT_COMMON
#include "CommonTypes.hpp"

#include "WorldDefs.hpp"

#ifndef IMPORT_WORLDLOADER
#   include <sqlite3.h>
#   include <zlib.h>
#   include "CoherentNoise.hpp"
#   define IMPORT_WORLDMANAGER
#endif

#include "Interface_WorldManager.hpp"

using namespace std;
using namespace Common;

struct WorldMeta
{
    uint64 WorldSeed;
    float WaterLevel;
    fvec3  WorldSpawn;
};

struct RawChunk 
{
    bool DataValid, ChunkNew;
    ivec3 ChunkPosition;
    uint32 BlockCount;
    uint8 BlockData[CHUNK_BLOCKS][CHUNK_BLOCKS][CHUNK_BLOCKS][2];
};

class WORLDLOADER_API WorldLoader
{
public:
    static WorldLoader * Create(const char * name, BlockDictionary * dict);
    void Destroy();

    virtual bool MakeChunk(ivec3 position, RawChunk * data);
    virtual bool LoadChunk(ivec3 position, RawChunk * data);
    virtual bool SaveChunk(ivec3 position, RawChunk * data);

    virtual void * Compress  (uint32 size, void * data, uint32 * finalsize);
    virtual void * Decompress(uint32 size, void * data, uint32 * finalsize);

protected:
    WorldLoader(const char * name, BlockDictionary * dict);
    ~WorldLoader();

    virtual void BuildWorldMeta();
    virtual void BuildWorldDict();

#ifndef IMPORT_WORLDLOADER
private:
    sqlite3 * mDatabase;
    sqlite3_stmt * mStmtLoad, * mStmtSave;
    Noise::CoherentNoise * mNoise;

    // World meta data
    WorldMeta * mMeta;
    BlockDictionary * mDict;
#endif
};
