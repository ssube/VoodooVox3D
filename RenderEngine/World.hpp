#pragma once

#include "Includes.hpp"
#include "Block.hpp"
#include "Chunk.hpp"

class World
{
public:
	World(void);
	~World(void);

	D3DXVECTOR3 ProcessCollision(D3DXVECTOR3 pos);

	Block * GetBlock(D3DXVECTOR3 pos);

	static const size_t WorldChunks = 5;
	static const size_t WorldSize = WorldChunks * Chunk::ChunkSize;

private:
	Chunk * mChunks[WorldChunks][WorldChunks][WorldChunks];

	D3DXVECTOR3 mLastPosition;
};
