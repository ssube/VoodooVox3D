#include "World.hpp"

World::World(BlockDictionary * dict, RenderEngine * render)
	: mRenderer(render), mDictionary(dict)
{
	mGen = new Generator(13);

	vector<int> idList = dict->GetTemplateList();

	for ( size_t x = 0; x < WORLD_BLOCKS; ++x )
	{
		for ( size_t y = 0; y < WORLD_BLOCKS; ++y )
		{
			for ( size_t z = 0; z < WORLD_BLOCKS; ++z )
			{
				float raw = mGen->GetPoint(x, y, z);
				int cid = (int)(raw * 1000.0f) % idList.size();

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
				D3DXMATRIX trans;
				D3DXMatrixTranslation(&trans, CHUNK_SIZE * x, CHUNK_SIZE * y, CHUNK_SIZE * z);
				ro->SetTransform(trans);
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

	float newSpeed = 1.0f;
	if ( newBlock && newBlock->GetAttribute("fSpeed", &newSpeed) == sizeof(float) )
	{
		D3DXVECTOR3 realShift = shift * newSpeed;
		D3DXVec3Add(&final, &pos, &realShift);
	
		this->UpdateChunks(final);
	} else {
		D3DXVec3Add(&final, &pos, &shift);
	}

	return final;
}

Block * World::GetBlock(D3DXVECTOR3 pos)
{
	float worldEdge = WORLD_SIZE / 2;

	if (	pos.x < -worldEdge	|| pos.y < -worldEdge	|| pos.z < -worldEdge	|| 
			pos.x > worldEdge	|| pos.y > worldEdge	|| pos.z > worldEdge	)
	{
		return NULL;
	}

	// Is within the world, find the chunk
	float blockEdge = BLOCK_SIZE / 2;

	float cX = floor( (pos.x + blockEdge) / BLOCK_SIZE );
	float cY = floor( (pos.y + blockEdge) / BLOCK_SIZE );
	float cZ = floor( (pos.z + blockEdge) / BLOCK_SIZE );
	cX = max(0, min(WORLD_BLOCKS, cX));
	cY = max(0, min(WORLD_BLOCKS, cY));
	cZ = max(0, min(WORLD_BLOCKS, cZ));

	Block * block = mBlocks[(size_t)cX][(size_t)cY][(size_t)cZ];

	return block;
}

void World::GenerateGeometry(size_t x, size_t y, size_t z)
{
	mGeometryVector.clear();

	for (int px = CHUNK_BLOCKS*x; px < CHUNK_BLOCKS*(x+1); ++px)
	{
		for (int py = CHUNK_BLOCKS*y; py < CHUNK_BLOCKS*(y+1); ++py)
		{
			for (int pz = CHUNK_BLOCKS*z; pz < CHUNK_BLOCKS*(z+1); ++pz)
			{
				this->ProcessPoint(px, py, pz);
			}
		}
	}

	size_t mGeometryCount = mGeometryVector.size();
	mObjects[x][y][z]->SetGeometry(mGeometryCount, mGeometryVector.begin()._Myptr);
}

void World::ProcessPoint(size_t x, size_t y, size_t z)
{
	// Flag names and chunk sizes
	int minSize = 0, maxSize = 3;
	string attrRender = "bOcclude", attrTex = "fTexture", attrOcc = "bOcclude";

	Block * block = mBlocks[x][y][z];
	bool drawThis;
	block->GetAttribute(attrRender, &drawThis);

	unsigned char vshade = 0xff - ( rand() % 128 );
	DWORD vcolor = 0xff000000 | ( vshade << 16 ) | ( vshade << 8 ) | vshade;

	if ( !drawThis )
	{
		return;
	}

	// Build the flags
	float texture = 0.0f;
	bool b[3][3][3];
	memset(b, 0, sizeof(bool)*3*3*3);

	if ( block )
	{
		block->GetAttribute(attrOcc, &b[1][1][1]);
		block->GetAttribute(attrTex, &texture);
	}

	if ( x > 0 )
	{
		block = mBlocks[x-1][y][z];
		if ( block )
		{
			block->GetAttribute(attrOcc, &b[0][1][1]);
		}
	}

	if ( x < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[x+1][y][z];
		if ( block )
		{
			block->GetAttribute(attrOcc, &b[2][1][1]);
		}
	}

	if ( y > 0 )
	{
		block = mBlocks[x][y-1][z];
		if ( block )
		{
			block->GetAttribute(attrOcc, &b[1][0][1]);
		}
	}

	if ( y < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[x][y+1][z];
		if ( block )
		{
			block->GetAttribute(attrOcc, &b[1][2][1]);
		}
	}

	if ( z > 0 )
	{
		block = mBlocks[x][y][z-1];
		if ( block )
		{
			block->GetAttribute(attrOcc, &b[1][1][0]);
		}
	}

	if ( z < (WORLD_BLOCKS-1) )
	{
		block = mBlocks[x][y][z+1];
		if ( block )
		{
			block->GetAttribute(attrOcc, &b[1][1][2]);
		}
	}

	// Calculate the offset and positions
	float offset = BLOCK_SIZE;
	float px = (float)x * BLOCK_SIZE;
	float py = (float)y * BLOCK_SIZE;
	float pz = (float)z * BLOCK_SIZE;

	// Now, build geometry. We'll test each of the 6 surfaces and create a quad
	// if one should exist. The important thing here is to make sure the quad
	// points the right way, that is, toward the empty block.

	// Test the left:
	if (!b[0][1][1])
	{
		// On the -X, facing -X, with b[1][1][]'s texture
		mGeometryVector.push_back(Vertex(px, py, pz, -1, 0, 0, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px, py, pz + offset, -1, 0, 0, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px, py + offset, pz + offset, -1, 0, 0, 1, 1, texture, vcolor));

		mGeometryVector.push_back(Vertex(px , py + offset, pz + offset, -1, 0, 0, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz , -1, 0, 0, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz , -1, 0, 0, 0, 0, texture, vcolor));
	}

	// Test the right:
	if (!b[2][1][1])
	{
		// On the +X, facing -X
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz + offset, +1, 0, 0, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz , +1, 0, 0, 0, 0, texture, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py , pz , +1, 0, 0, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , +1, 0, 0, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, 1, 1, texture, vcolor));
	}

	// Test the bottom:
	if (!b[1][0][1])
	{
		// On the -Y, facing -Y
		mGeometryVector.push_back(Vertex(px + offset, py , pz , 0, -1, 0, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz + offset, 0, -1, 0, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz + offset, 0, -1, 0, 0, 1, texture, vcolor));

		mGeometryVector.push_back(Vertex(px , py , pz + offset, 0, -1, 0, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz , 0, -1, 0, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz , 0, -1, 0, 1, 0, texture, vcolor));
	}

	// Test the top:
	if (!b[1][2][1])
	{
		// On the +Y, facing -Y
		mGeometryVector.push_back(Vertex(px , py + offset, pz + offset, 0, +1, 0, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, +1, 0, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , 0, +1, 0, 1, 0, texture, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , 0, +1, 0, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz , 0, +1, 0, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz + offset, 0, +1, 0, 0, 1, texture, vcolor));
	}

	// Test the near side:
	if (!b[1][1][0])
	{
		// On the -Z, facing -Z
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , 0, 0, -1, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz , 0, 0, -1, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz , 0, 0, -1, 0, 0, texture, vcolor));

		mGeometryVector.push_back(Vertex(px , py , pz , 0, 0, -1, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz , 0, 0, -1, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz , 0, 0, -1, 1, 1, texture, vcolor));
	}

	// Test the far side:
	if (!b[1][1][2])
	{
		// On the +Z, facing -Z
		mGeometryVector.push_back(Vertex(px , py , pz + offset, 0, 0, +1, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py , pz + offset, 0, 0, +1, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, 1, 1, texture, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py + offset, pz + offset, 0, 0, +1, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px , py , pz + offset, 0, 0, +1, 0, 0, texture, vcolor));
	}
}
