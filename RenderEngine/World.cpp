#include "World.hpp"

World::World(BlockDictionary * dict, RenderEngine * render)
	: mRenderer(render), mDictionary(dict)
{
	ZeroMemory(mBlocks, sizeof(Block*)*WORLD_BLOCKS*WORLD_BLOCKS*WORLD_BLOCKS);

	mGen = new Generator(13);

	vector<int> idList = dict->GetTemplateList();

	for ( size_t x = 0; x < WORLD_BLOCKS; ++x )
	{
		for ( size_t y = 0; y < WORLD_BLOCKS; ++y )
		{
			for ( size_t z = 0; z < WORLD_BLOCKS; ++z )
			{
				float raw = mGen->GetPoint(x/D3DX_PI, y/D3DX_PI, z/D3DX_PI);
				int cid = floor(raw * idList.size());

				BlockTemplate * temp = dict->GetTemplate(idList[cid]);

				mBlocks[x][y][z] = new Block(temp);
			}
		}
	}

	for ( size_t x = 0; x < WORLD_CHUNKS; ++x )
	{
		for ( size_t y = 0; y < WORLD_CHUNKS; ++y )
		{
			for ( size_t z = 0; z < WORLD_CHUNKS; ++z )
			{
				RenderObject * ro = render->CreateRenderObject();
				//D3DXMATRIX trans;
				//D3DXMatrixTranslation(&trans, CHUNK_SIZE * x, CHUNK_SIZE * y, CHUNK_SIZE * z);
				//ro->SetTransform(trans);
				ro->SetPosition(D3DXVECTOR3(CHUNK_SIZE * x, CHUNK_SIZE * y, CHUNK_SIZE * z));
				mObjects[x][y][z] = ro;

				GenerateGeometry(x, y, z);
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

void World::Update()
{
	// Handle block physics
}

void World::UpdateChunks(D3DXVECTOR3 pos)
{
	// Find if we've left the central chunk
	bool needUpdate = ( pos.x < CHUNK_SIZE ) || ( pos.y < CHUNK_SIZE ) || ( pos.z < CHUNK_SIZE ) ||
		( pos.x > (CHUNK_SIZE*2) ) || ( pos.y > (CHUNK_SIZE*2) ) || ( pos.z > (CHUNK_SIZE*2) );

	if ( !needUpdate ) return;

	// We need to update, find out how much
	D3DXVECTOR3 relChunk = pos / CHUNK_SIZE;
	size_t dX = floor(relChunk.x);
	size_t dY = floor(relChunk.y);
	size_t dZ = floor(relChunk.z);

	if ( dX < WORLD_CHUNKS && dY < WORLD_CHUNKS && dZ < WORLD_CHUNKS )
	{
		// Partial reload
	} else {
		// Full reload
	}
}

D3DXVECTOR3 World::UpdatePosition(D3DXVECTOR3 pos, D3DXVECTOR3 shift)
{
	// Need to find the newPos' block, get the speed from that, and lerp
	//	between oldPos and newPos based on that
	D3DXVECTOR3 final;
	D3DXVECTOR3 tempPos;
	D3DXVec3Add(&tempPos, &pos, &shift);
	Block * newBlock = GetBlock(tempPos);

	if ( newBlock )
	{
		D3DXVECTOR3 realShift = shift * newBlock->Speed;
		D3DXVec3Add(&final, &pos, &realShift);
	
		this->UpdateChunks(final);
	} else {
		D3DXVec3Add(&final, &pos, &shift);
	}

	return final;
}

Block * World::GetBlock(D3DXVECTOR3 pos)
{
	if (	pos.x < 0	|| pos.y < 0	|| pos.z < 0	|| 
			pos.x > WORLD_SIZE	|| pos.y > WORLD_SIZE	|| pos.z > WORLD_SIZE	)
	{
		return NULL;
	}

	// Is within the world, find the chunk
	float cX = floor( pos.x / BLOCK_SIZE );
	float cY = floor( pos.y / BLOCK_SIZE );
	float cZ = floor( pos.z / BLOCK_SIZE );
	//cX = max(0, min(WORLD_BLOCKS, cX));
	//cY = max(0, min(WORLD_BLOCKS, cY));
	//cZ = max(0, min(WORLD_BLOCKS, cZ));

	Block * block = mBlocks[(size_t)cX][(size_t)cY][(size_t)cZ];

	return block;
}

void World::GenerateGeometry(size_t x, size_t y, size_t z)
{
	for ( size_t lod = 0; lod < LOD_COUNT; ++lod )
	{
		mGeometryVector.clear();

		int maxY = ( CHUNK_BLOCKS * ( y + 1 ) ) - 1;

		for (int px = CHUNK_BLOCKS*x; px < CHUNK_BLOCKS*(x+1); px += pow(2.0f, (int)lod))
		{
			for (int py = CHUNK_BLOCKS*y; py < CHUNK_BLOCKS*(y+1); py += pow(2.0f, (int)lod))
			{
				for (int pz = CHUNK_BLOCKS*z; pz < CHUNK_BLOCKS*(z+1); pz += pow(2.0f, (int)lod))
				{
					this->ProcessPoint(lod, px, py, pz, x, y, z);
				}
			}
		}

		size_t mGeometryCount = mGeometryVector.size();
		mObjects[x][y][z]->SetGeometry(lod, mGeometryCount, mGeometryVector.begin()._Myptr);
	}
}

void World::ProcessPoint(size_t lod, size_t x, size_t y, size_t z, size_t cx, size_t cy, size_t cz)
{
	// Flag names and chunk sizes
	int minSize = 0, maxSize = 3;

	Block * block = mBlocks[x][y][z];

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

	if ( x > 0 )
	{
		block = mBlocks[x-1][y][z];
		if ( block )
		{
			b[0][1][1] = block->Occludes;
		}
	}

	if ( x < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[x+1][y][z];
		if ( block )
		{
			b[2][1][1] = block->Occludes;
		}
	}

	if ( y > 0 )
	{
		block = mBlocks[x][y-1][z];
		if ( block )
		{
			b[1][0][1] = block->Occludes;
		}
	}

	if ( y < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[x][y+1][z];
		if ( block )
		{
			b[1][2][1] = block->Occludes;
			//if ( block->Occludes )
			//{
			//	lighting = block->Light - 0.02f;
			//}
		}
	}

	if ( z > 0 )
	{
		block = mBlocks[x][y][z-1];
		if ( block )
		{
			b[1][1][0] = block->Occludes;
		}
	}

	if ( z < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[x][y][z+1];
		if ( block )
		{
			b[1][1][2] = block->Occludes;
		}
	}

	//mBlocks[x][y][z]->Light = lighting;
	unsigned char vrshade = ( 0xff - ( rand() % 32 ) );// * lighting;
	unsigned char vgshade = ( 0xff - ( rand() % 24 ) );// * lighting;
	unsigned char vbshade = ( 0xff - ( rand() % 48 ) );// * lighting;
	DWORD vcolor = 0xff000000 | ( vrshade << 16 ) | ( vgshade << 8 ) | vbshade;

	// Calculate the offset and positions
	float offset = BLOCK_SIZE * (lod + 1);
	float px = (x * BLOCK_SIZE) - ( cx * CHUNK_SIZE );
	float py = (y * BLOCK_SIZE) - ( cy * CHUNK_SIZE );
	float pz = (z * BLOCK_SIZE) - ( cz * CHUNK_SIZE );

	// Now, build geometry. We'll test each of the 6 surfaces and create a quad
	// if one should exist. The important thing here is to make sure the quad
	// points the right way, that is, toward the empty block.

	// Test the left:
	if (!b[0][1][1])
	{
		// On the -X, facing -X, with b[1][1][]'s texture
		mGeometryVector.push_back(Vertex(px, py, pz, -1, 0, 0, texX, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px, py, pz + offset, -1, 0, 0, texX, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px, py + offset, pz + offset, -1, 0, 0, texX+0.5f, texY+0.5f, 0, vcolor));

		mGeometryVector.push_back(Vertex(px , py + offset, pz + offset, -1, 0, 0, texX+0.5f, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz , -1, 0, 0, texX+0.5f, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz , -1, 0, 0, texX, texY, 0, vcolor));
	}

	// Test the right:
	if (!b[2][1][1])
	{
		// On the +X, facing -X
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, texX+0.5f, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz + offset, +1, 0, 0, texX, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz , +1, 0, 0, texX, texY, 0, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py , pz , +1, 0, 0, texX, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , +1, 0, 0, texX+0.5f, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, texX+0.5f, texY+0.5f, 0, vcolor));
	}

	// Test the bottom:
	if (!b[1][0][1])
	{
		// On the -Y, facing -Y
		mGeometryVector.push_back(Vertex(px + offset, py , pz , 0, -1, 0, texX+0.5f, texY, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz + offset, 0, -1, 0, texX+0.5f, texY+0.5f, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz + offset, 0, -1, 0, texX, texY+0.5f, texture, vcolor));

		mGeometryVector.push_back(Vertex(px , py , pz + offset, 0, -1, 0, texX, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz , 0, -1, 0, texX, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz , 0, -1, 0, texX+0.5f, texY, 0, vcolor));
	}

	// Test the top:
	if (!b[1][2][1])
	{
		// On the +Y, facing -Y
		mGeometryVector.push_back(Vertex(px , py + offset, pz + offset, 0, +1, 0, texX, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, +1, 0, texX+0.5f, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , 0, +1, 0, texX+0.5f, texY, 0, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , 0, +1, 0, texX+0.5f, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz , 0, +1, 0, texX, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz + offset, 0, +1, 0, texX, texY+0.5f, 0, vcolor));
	}

	// Test the near side:
	if (!b[1][1][0])
	{
		// On the -Z, facing -Z
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , 0, 0, -1, texX+0.5f, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz , 0, 0, -1, texX+0.5f, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz , 0, 0, -1, texX, texY, 0, vcolor));

		mGeometryVector.push_back(Vertex(px , py , pz , 0, 0, -1, texX, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz , 0, 0, -1, texX, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , 0, 0, -1, texX+0.5f, texY+0.5f, 0, vcolor));
	}

	// Test the far side:
	if (!b[1][1][2])
	{
		// On the +Z, facing -Z
		mGeometryVector.push_back(Vertex(px , py , pz + offset, 0, 0, +1, texX, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz + offset, 0, 0, +1, texX+0.5f, texY, 0, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, texX+0.5f, texY+0.5f, 0, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, texX+0.5f, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz + offset, 0, 0, +1, texX, texY+0.5f, 0, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz + offset, 0, 0, +1, texX, texY, 0, vcolor));
	}
}
