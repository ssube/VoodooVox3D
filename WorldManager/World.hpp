#pragma once

#include "Interface_WorldManager.hpp"

#define IMPORT_COMMON
#include "CommonTypes.hpp"
#include "WorldDefs.hpp"

// Render engine includes
#define IMPORT_RENDERENGINE
#include "Interface_RenderEngine.hpp"

#ifndef IMPORT_WORLDMANAGER
#   include <vector>
#   include "BlockDictionary.hpp"
//  World loader includes
#   define IMPORT_WORLDLOADER
#   include "WorldLoader.hpp"
#   include "RenderObject.hpp"
#endif

using namespace Common;

class WORLDMANAGER_API World
{
public:
    static World * Create(RenderEngine * render);
    void Destroy();

    virtual Block * GetBlock(fvec3 pos);
    virtual ivec3 GetBlockPos(fvec3 pos);

    virtual uvec3 GetRelativeChunk(fvec3 pos);
    virtual ivec3 GetAbsoluteChunk(fvec3 pos);

    virtual void Update();
    virtual void UpdateChunks(fvec3 & pos);
    virtual fvec3 UpdatePosition(fvec3 pos, fvec3 shift);

    //GeometryBuffer * GenerateChunkGeometry(Vector3<size_t> position);
    virtual bool GetBlockNeighbors(uvec3 block, uint8 & faces, uint8 & texture);

private:
    World(RenderEngine * render);
    ~World(void);

    virtual void GenerateGeometry(uvec3 position);
    virtual void ProcessPoint(uint8 lod, uvec3 position, uvec3 chunk);

    virtual void LoadChunk(uvec3 chunk);
    virtual void ClearChunk(uvec3 chunk);
    virtual void MoveChunk(uvec3 chunk, uvec3 dest);

#ifndef IMPORT_WORLDMANAGER
private:
    Block * mBlocks[WORLD_BLOCKS][WORLD_BLOCKS][WORLD_BLOCKS];
    RenderObject * mObjects[WORLD_CHUNKS][WORLD_CHUNKS][WORLD_CHUNKS];

    BlockDictionary * mDictionary;
    RenderEngine * mRenderEngine;
    vector<Vertex> mGeometryVector;
    WorldLoader * mWorldLoader;

    ivec3 mOriginChunk;
#endif
};
