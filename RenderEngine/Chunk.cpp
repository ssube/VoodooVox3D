#include "Chunk.hpp"

#define COORD(x, y, z) (x*Chunk::ChunkWidth*Chunk::ChunkHeight)+(y*Chunk::ChunkHeight)+z

Chunk::Chunk(BlockDictionary * dict)
	: mDirty(true), mGeometry(NULL)
{
	mGen = new Generator(13);

	vector<int> idList = dict->GetTemplateList();

	for ( size_t x = 0; x < Chunk::ChunkSize; ++x )
	{
		for ( size_t y = 0; y < Chunk::ChunkSize; ++y )
		{
			for ( size_t z = 0; z < Chunk::ChunkSize; ++z )
			{
				float raw = mGen->GetPoint(x, y, z);
				int cid = (int)(raw * 1000.0f) % idList.size();

				BlockTemplate * temp = dict->GetTemplate(idList[cid]);

				mBlocks[x][y][z] = new Block(temp);
			}
		}
	}
}

Chunk::~Chunk(void)
{
	delete[] mGeometry;
	delete mGen;
}

Vertex * Chunk::GetGeometry()
{
	return mGeometry;
}

void Chunk::GenerateGeometry()
{
	if ( mDirty )
	{
		mDirty = false;

		for (int px = 0; px < Chunk::ChunkSize; ++px)
		{
			for (int py = 0; py < Chunk::ChunkSize; ++py)
			{
				for (int pz = 0; pz < Chunk::ChunkSize; ++pz)
				{
					ProcessPoint(px, py, pz);
				}
			}
		}

		if (mGeometry)
		{
			delete[] mGeometry;
		}

		mGeometryCount = mGeometryVector.size();
		mGeometry = new Vertex[mGeometryCount];

		for ( size_t cv = 0; cv < mGeometryCount; ++cv )
		{
			mGeometry[cv] = mGeometryVector[cv];
		}
	}
}

size_t Chunk::GetGeometryCount()
{
	return mGeometryCount;
}

void Chunk::ProcessPoint(int x, int y, int z)
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

	if ( x < (Chunk::ChunkSize-1) )
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

	if ( y < (Chunk::ChunkSize-1) )
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

	if ( z < (Chunk::ChunkSize-1) )
	{
		block = mBlocks[x][y][z+1];
		if ( block )
		{
			block->GetAttribute(attrOcc, &b[1][1][2]);
		}
	}

	// Calculate the offset and positions
	float BOXENGINE_BLOCK_UNITS = 10.0f;
	float offset = (BOXENGINE_BLOCK_UNITS / 2);
	float px = (float)x * BOXENGINE_BLOCK_UNITS;
	float py = (float)y * BOXENGINE_BLOCK_UNITS;
	float pz = (float)z * BOXENGINE_BLOCK_UNITS;

	// Now, build geometry. We'll test each of the 6 surfaces and create a quad
	// if one should exist. The important thing here is to make sure the quad
	// points the right way, that is, toward the empty block.

	// Test the left:
	if (!b[0][1][1])
	{
		// On the -X, facing -X, with b[1][1][]'s texture
		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz - offset, -1, 0, 0, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz + offset, -1, 0, 0, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py + offset, pz + offset, -1, 0, 0, 1, 1, texture, vcolor));

		mGeometryVector.push_back(Vertex(px - offset, py + offset, pz + offset, -1, 0, 0, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py + offset, pz - offset, -1, 0, 0, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz - offset, -1, 0, 0, 0, 0, texture, vcolor));
	}

	// Test the right:
	if (!b[2][1][1])
	{
		// On the +X, facing -X
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py - offset, pz + offset, +1, 0, 0, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py - offset, pz - offset, +1, 0, 0, 0, 0, texture, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py - offset, pz - offset, +1, 0, 0, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz - offset, +1, 0, 0, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, 1, 1, texture, vcolor));
	}

	// Test the bottom:
	if (!b[1][0][1])
	{
		// On the -Y, facing -Y
		mGeometryVector.push_back(Vertex(px + offset, py - offset, pz - offset, 0, -1, 0, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py - offset, pz + offset, 0, -1, 0, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz + offset, 0, -1, 0, 0, 1, texture, vcolor));

		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz + offset, 0, -1, 0, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz - offset, 0, -1, 0, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py - offset, pz - offset, 0, -1, 0, 1, 0, texture, vcolor));
	}

	// Test the top:
	if (!b[1][2][1])
	{
		// On the +Y, facing -Y
		mGeometryVector.push_back(Vertex(px - offset, py + offset, pz + offset, 0, +1, 0, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, +1, 0, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz - offset, 0, +1, 0, 1, 0, texture, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz - offset, 0, +1, 0, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py + offset, pz - offset, 0, +1, 0, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py + offset, pz + offset, 0, +1, 0, 0, 1, texture, vcolor));
	}

	// Test the near side:
	if (!b[1][1][0])
	{
		// On the -Z, facing -Z
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz - offset, 0, 0, -1, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py - offset, pz - offset, 0, 0, -1, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz - offset, 0, 0, -1, 0, 0, texture, vcolor));

		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz - offset, 0, 0, -1, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py + offset, pz - offset, 0, 0, -1, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz - offset, 0, 0, -1, 1, 1, texture, vcolor));
	}

	// Test the far side:
	if (!b[1][1][2])
	{
		// On the +Z, facing -Z
		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz + offset, 0, 0, +1, 0, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py - offset, pz + offset, 0, 0, +1, 1, 0, texture, vcolor));
		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, 1, 1, texture, vcolor));

		mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, 1, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py + offset, pz + offset, 0, 0, +1, 0, 1, texture, vcolor));
		mGeometryVector.push_back(Vertex(px - offset, py - offset, pz + offset, 0, 0, +1, 0, 0, texture, vcolor));
	}
}
