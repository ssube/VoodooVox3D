#include "World.hpp"

World::World(void)
{
}

World::~World(void)
{
}

void World::UpdateChunks()
{
	// Find if we've left the central chunk
	bool needUpdate = ( mLastPosition.x < 0 ) || ( mLastPosition.y < 0 ) || ( mLastPosition.z < 0 ) ||
		( mLastPosition.x > Chunk::ChunkSize ) || ( mLastPosition.y > Chunk::ChunkSize ) || ( mLastPosition.z > Chunk::ChunkSize );
}

D3DXVECTOR3 World::ProcessCollision(D3DXVECTOR3 pos)
{
	// Need to find the newPos' block, get the speed from that, and lerp
	//	between oldPos and newPos based on that
	Block * newBlock = GetBlock(pos);

	float newSpeed = 1.0f;
	if ( newBlock && newBlock->GetAttribute("fSpeed", &newSpeed) == sizeof(float) )
	{
		mLastPosition = ( pos * newSpeed ) + ( mLastPosition * ( 1 - newSpeed ) );
		return mLastPosition;
	} else {
		return pos;
	}
}

Block * World::GetBlock(D3DXVECTOR3 pos)
{
	// Find out if it is in the world
	size_t worldChunkReach = ( WorldChunks & 0xFFFFFFFE ) / 2;
	float worldReach = Chunk::ChunkSize * worldChunkReach;
	float negReach = 0 - worldReach;

	if (	pos.x < negReach	|| pos.y < negReach		|| pos.z < negReach || 
			pos.x > worldReach	|| pos.y > worldReach	|| pos.z > worldReach )
	{
		return NULL;
	}

	// Is within the world, find the chunk
	float cX = floor( pos.x / Chunk::ChunkSize );
	float cY = floor( pos.y / Chunk::ChunkSize );
	float cZ = floor( pos.z / Chunk::ChunkSize );
	float bX = pos.x - ( cX * Chunk::ChunkSize );
	float bY = pos.y - ( cY * Chunk::ChunkSize );
	float bZ = pos.z - ( cZ * Chunk::ChunkSize );

	Chunk * chunk = mChunks[(size_t)cX][(size_t)cY][(size_t)cZ];
	Block * block = chunk->GetBlock((size_t)bX, (size_t)bY, (size_t)bZ);

	return block;
}

/*Chunk * World::GetChunk(D3DXVECTOR3 pos)
{

}*/
