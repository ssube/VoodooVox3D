#include "World.hpp"

World::World(BlockDictionary * dict, RenderEngine * render)
	: mRenderer(render), mDictionary(dict)
{
	ZeroMemory(mBlocks, sizeof(Block*)*WORLD_BLOCKS*WORLD_BLOCKS*WORLD_BLOCKS);

	vector<int> idList = dict->GetTemplateList();
	mGen = new WorldGenerator(idList);
	WorldSegment * segment = mGen->Generate(13, uvec3(), uvec3(WORLD_BLOCKS));

	for ( size_t x = 0; x < WORLD_BLOCKS; ++x )
	{
		for ( size_t y = 0; y < WORLD_BLOCKS; ++y )
		{
			for ( size_t z = 0; z < WORLD_BLOCKS; ++z )
			{
				//float raw = mGen->GetPoint(x/D3DX_PI, y/D3DX_PI, z/D3DX_PI);
				//int cid = floor(raw * idList.size());

				BlockTemplate * temp = dict->GetTemplate(idList[segment->Blocks[segment->Index(x, y, z)]]);

				mBlocks[x][y][z] = new Block(temp);
			}
		}
	}

	delete segment;

	for ( size_t x = 0; x < WORLD_CHUNKS; ++x )
	{
		for ( size_t y = 0; y < WORLD_CHUNKS; ++y )
		{
			for ( size_t z = 0; z < WORLD_CHUNKS; ++z )
			{
				RenderObject * ro = render->CreateRenderObject(); // Cross dependency

				ro->SetPosition(fvec3(CHUNK_SIZE * x, CHUNK_SIZE * y, CHUNK_SIZE * z)); // Cross dependency

				mObjects[x][y][z] = ro;

				GenerateGeometry(uvec3(x, y, z));
			}
		}
	}
}

World::~World(void)
{
	for ( size_t x = 0; x < WORLD_CHUNKS; ++x )
	{
		for ( size_t y = 0; y < WORLD_CHUNKS; ++y )
		{
			for ( size_t z = 0; z < WORLD_CHUNKS; ++z )
			{
				delete mObjects[x][y][z];
			}
		}
	}
}

ivec3 World::GetBlockPos(fvec3 pos)
{
	return Common::Math::Floor(pos / BLOCK_SIZE);
}

ivec3 World::GetChunkPos(fvec3 pos)
{
	return ( Common::Math::Floor(pos / CHUNK_SIZE) + mOriginChunk );
}

void World::Update()
{
	// Handle block physics
}

void World::UpdateChunks(fvec3 pos)
{
	using namespace Common::Math;

	// Find if we've left the central chunk
	//bool needUpdate = All(pos, CHUNK_SIZE) && !Any(pos, CHUNK_SIZE*2);

	// Find the new center chunk
	ivec3 newOrigin = GetChunkPos(pos);

	if ( newOrigin == mOriginChunk )
	{
		return;
	} else {
		// We need to update, find out how much
		ivec3 shift = newOrigin - mOriginChunk;

		if ( Any(shift, WORLD_CHUNKS) )
		{
			// Full reload
		} else {
			// Partial reload
		}
	}
}

fvec3 World::UpdatePosition(fvec3 pos, fvec3 shift)
{
	using namespace Common::Math;

	// Need to find the newPos' block, get the speed from that, and lerp
	//	between oldPos and newPos based on that
	fvec3 final;
	Block * newBlock = GetBlock(pos + shift);

	if ( newBlock )
	{
		fvec3 realShift = shift * newBlock->Speed;
		final = pos + realShift;
	
		this->UpdateChunks(final);
	} else {
		final = pos + shift;
	}

	return final;
}

Block * World::GetBlock(fvec3 pos)
{
	using namespace Common::Math;

	if ( !Within(0.0f, WORLD_SIZE, pos) )
	{
		return NULL;
	}

	// Is within the world, find the chunk
	ivec3 blockPos = GetBlockPos( pos / BLOCK_SIZE );

	Block * block = INDEX3(mBlocks, blockPos);

	return block;
}

void World::GenerateGeometry(uvec3 position)
{
	size_t lodOffset[4], lodVCount[4];

	mGeometryVector.clear();

	for ( size_t lod = 0; lod < LOD_COUNT; ++lod )
	{
		lodOffset[lod] = mGeometryVector.size();

		uvec3 blockpos(0);

		for (blockpos.x = CHUNK_BLOCKS*position.x; blockpos.x < CHUNK_BLOCKS*(position.x+1); blockpos.x += pow(2.0f, (int)lod))
		{
			for (blockpos.y = CHUNK_BLOCKS*position.y; blockpos.y < CHUNK_BLOCKS*(position.y+1); blockpos.y += pow(2.0f, (int)lod))
			{
				for (blockpos.z = CHUNK_BLOCKS*position.z; blockpos.z < CHUNK_BLOCKS*(position.z+1); blockpos.z += pow(2.0f, (int)lod))
				{
					this->ProcessPoint(lod, blockpos, position);
				}
			}
		}

		lodVCount[lod] = mGeometryVector.size() - lodOffset[lod];
	}

	INDEX3(mObjects, position)->SetGeometry(mGeometryVector.size(), lodOffset, lodVCount, mGeometryVector.begin()._Myptr); // Cross dependency
}

void World::ProcessPoint(size_t lod, uvec3 position, uvec3 chunk)
{
	// Flag names and chunk sizes
	int minSize = 0, maxSize = 3;

	Block * block = INDEX3(mBlocks, position);

	// Build the flags
	size_t texture = 0, texCol = 0, texRow = 0;
	float texX, texY;
	bool b[3][3][3];
	memset(b, 0, sizeof(bool)*3*3*3);

	if ( !block )
	{
		return;
	} else {
		b[1][1][1] = block->Occludes;
		texture = block->Texture;
		texRow = texture / 2;
		texCol = texture % 2;

		texX = 0.5f * texCol;
		texY = 0.5f * texRow;

		if ( !block->Visible )
		{
			return;
		}
	}

	if ( position.x > 0 )
	{
		block = mBlocks[position.x-1][position.y][position.z];
		if ( block )
		{
			b[0][1][1] = block->Occludes;
		}
	}

	if ( position.x < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[position.x+1][position.y][position.z];
		if ( block )
		{
			b[2][1][1] = block->Occludes;
		}
	}

	if ( position.y > 0 )
	{
		block = mBlocks[position.x][position.y-1][position.z];
		if ( block )
		{
			b[1][0][1] = block->Occludes;
		}
	}

	if ( position.y < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[position.x][position.y+1][position.z];
		if ( block )
		{
			b[1][2][1] = block->Occludes;
			//if ( block->Occludes )
			//{
			//	lighting = block->Light - 0.02f;
			//}
		}
	}

	if ( position.z > 0 )
	{
		block = mBlocks[position.x][position.y][position.z-1];
		if ( block )
		{
			b[1][1][0] = block->Occludes;
		}
	}

	if ( position.z < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[position.x][position.y][position.z+1];
		if ( block )
		{
			b[1][1][2] = block->Occludes;
		}
	}

	//mBlocks[x][y][z]->Light = lighting;
	float vrshade = ( 255.0f - ( rand() % 32 ) ) / 255.0f;// * lighting;
	float vgshade = ( 255.0f - ( rand() % 24 ) ) / 255.0f;// * lighting;
	float vbshade = ( 255.0f - ( rand() % 48 ) ) / 255.0f;// * lighting;

	// Calculate the offset and positions
	float offset = BLOCK_SIZE * (lod + 1);
	float px = (position.x * BLOCK_SIZE) - ( chunk.x * CHUNK_SIZE );
	float py = (position.y * BLOCK_SIZE) - ( chunk.y * CHUNK_SIZE );
	float pz = (position.z * BLOCK_SIZE) - ( chunk.z * CHUNK_SIZE );

	// Now, build geometry. We'll test each of the 6 surfaces and create a quad
	// if one should exist. The important thing here is to make sure the quad
	// points the right way, that is, toward the empty block.

	// Test the left:
	if (!b[0][1][1])
	{
		// On the -X, facing -X
		mGeometryVector.push_back(Vertex(         px,          py,          pz, -1, 0, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px,          py, pz + offset, -1, 0, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, -1, 0, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));

		mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, -1, 0, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px, py + offset,          pz, -1, 0, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px,          py,          pz, -1, 0, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
	}

	// Test the right:
	if (!b[2][1][1])
	{
		// On the +X, facing +X
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset,          py, pz + offset, +1, 0, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset,          py,          pz, +1, 0, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));

		mGeometryVector.push_back(Vertex(px + offset,          py,          pz, +1, 0, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, +1, 0, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
	}

	// Test the bottom:
	if (!b[1][0][1])
	{
		// On the -Y, facing -Y
		mGeometryVector.push_back(Vertex(px + offset,          py,          pz, 0, -1, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset,          py, pz + offset, 0, -1, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px,          py, pz + offset, 0, -1, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));

		mGeometryVector.push_back(Vertex(         px,          py, pz + offset, 0, -1, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px,          py,          pz, 0, -1, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset,          py,          pz, 0, -1, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
	}

	// Test the top:
	if (!b[1][2][1])
	{
		// On the +Y, facing -Y
		mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, 0, +1, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, +1, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, 0, +1, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));

		mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, 0, +1, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px, py + offset,          pz, 0, +1, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, 0, +1, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
	}

	// Test the near side:
	if (!b[1][1][0])
	{
		// On the -Z, facing -Z
		mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, 0, 0, -1, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset,          py,          pz, 0, 0, -1, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px,          py,          pz, 0, 0, -1,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));

		mGeometryVector.push_back(Vertex(         px,          py,          pz, 0, 0, -1,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px, py + offset,          pz, 0, 0, -1,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, 0, 0, -1, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
	}

	// Test the far side:
	if (!b[1][1][2])
	{
		// On the +Z, facing -Z
		mGeometryVector.push_back(Vertex(         px,          py, pz + offset, 0, 0, +1,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset,          py, pz + offset, 0, 0, +1, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));

		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, 0, 0, +1,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
		mGeometryVector.push_back(Vertex(         px,          py, pz + offset, 0, 0, +1,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
	}
}
