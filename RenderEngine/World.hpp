#pragma once

#include "Includes.hpp"
#include "Block.hpp"
#include "BlockDictionary.hpp"
#include "RenderEngine.hpp"
#include "WorldGenerator.hpp"

class World
{
public:
	World(BlockDictionary * dict, RenderEngine * render);
	~World(void);

	Block * GetBlock(D3DXVECTOR3 pos);

	void Update();
	void UpdateChunks(D3DXVECTOR3 pos);
	D3DXVECTOR3 UpdatePosition(D3DXVECTOR3 pos, D3DXVECTOR3 shift);

private:
	void GenerateGeometry(size_t x, size_t y, size_t z);
	void ProcessPoint(size_t lod, size_t x, size_t y, size_t z, size_t cx, size_t cy, size_t cz);

private:
	Block * mBlocks[WORLD_BLOCKS][WORLD_BLOCKS][WORLD_BLOCKS];
	RenderObject * mObjects[WORLD_CHUNKS][WORLD_CHUNKS][WORLD_CHUNKS];

	BlockDictionary * mDictionary;
	RenderEngine * mRenderer;
	vector<Vertex> mGeometryVector;
	Generator * mGen;
};
