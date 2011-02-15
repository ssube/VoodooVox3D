#pragma once

// Internal includes
#include "Includes.hpp"
#include "Block.hpp"
#include "BlockDictionary.hpp"
#include "WorldGenerator.hpp"

// Common includes
#include "CommonTypes.hpp"
#include "VectorMath.hpp"

// Render engine includes
#define RENDERENGINE_INTERFACE
#include "RenderEngine.hpp"
#include "RenderObject.hpp"

class __declspec(dllexport) World
{
public:
	World(BlockDictionary * dict, RenderEngine * render);
	~World(void);

	Block * GetBlock(fvec3 pos);

	void Update();
	void UpdateChunks(fvec3 pos);
	fvec3 UpdatePosition(fvec3 pos, fvec3 shift);

	//GeometryBuffer * GenerateChunkGeometry(Vector3<size_t> position);

private:
	void GenerateGeometry(uvec3 position);
	void ProcessPoint(size_t lod, uvec3 position, uvec3 chunk);

private:
	Block * mBlocks[WORLD_BLOCKS][WORLD_BLOCKS][WORLD_BLOCKS];
	RenderObject * mObjects[WORLD_CHUNKS][WORLD_CHUNKS][WORLD_CHUNKS];

	BlockDictionary * mDictionary;
	RenderEngine * mRenderer;
	vector<Vertex> mGeometryVector;
	WorldGenerator * mGen;
};
