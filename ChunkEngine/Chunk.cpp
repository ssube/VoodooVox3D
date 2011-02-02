#include "Chunk.hpp"

#define COORD(x, y, z) (x*Chunk::ChunkWidth*Chunk::ChunkHeight)+(y*Chunk::ChunkHeight)+z

Chunk::Chunk(BlockDictionary * dict)
	: mDirty(true)
{
	//mBlocks = malloc(sizeof(Block*Chunk::ChunkWidth*Chunk::ChunkWidth*Chunk::ChunkHeight));
	mGen = new Generator(13);

	vector<int> idList = dict->GetTemplateList();

	for ( size_t x = 0; x < Chunk::ChunkWidth; ++x )
	{
		for ( size_t y = 0; y < Chunk::ChunkWidth; ++y )
		{
			for ( size_t z = 0; z < Chunk::ChunkHeight; ++z )
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
	//free(mBlocks);
	delete mGen;
}

Vertex * Chunk::GetGeometry()
{
	if ( mDirty )
	{
		this->GenerateGeometry();
	}

	return mGeometry.begin()._Myptr;
}

void Chunk::GenerateGeometry()
{
	mGeometry.clear();

	for (int px = 0; px < 4; ++px)
	{
		for (int py = 0; py < 4; ++py)
		{
			for (int pz = 0; pz < 4; ++pz)
			{
				ProcessPoint(px, py, pz);
			}
		}
	}
}

size_t Chunk::GetGeometryCount()
{
	return mGeometry.size();
}

void Chunk::AddVertex(float x, float y, float z, float nx, float ny, float nz, float tu, float tv, float tw, unsigned long color)
{
	Vertex vert = {x, y, z, nx, ny, nz, tu, tv, tw, color};
	mGeometry.push_back(vert);
}

void Chunk::ProcessPoint(int x, int y, int z)
{
	// Flag names and chunk sizes
	int minSize = 0, maxSize = 3;
	string attrTex = "fTexture", attrOcc = "bOcclude";

	// Build the flags
	float texture = 0.0f;
	bool b[3][3][3];
	memset(b, 0, sizeof(bool)*3*3*3);

	Block * block = mBlocks[x][y][z];
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

	if ( x < (Chunk::ChunkWidth-1) )
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

	if ( y < (Chunk::ChunkWidth-1) )
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

	if ( z < (Chunk::ChunkHeight-1) )
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
	if (b[1][1][1] && !b[0][1][1])
	{
		// On the -X, facing -X, with b[1][1][]'s texture
		AddVertex(px - offset, py - offset, pz - offset, -1, 0, 0, 0, 0, texture, 0xff0000ff);
		AddVertex(px - offset, py - offset, pz + offset, -1, 0, 0, 0, 0, texture, 0xffff8800);
		AddVertex(px - offset, py + offset, pz + offset, -1, 0, 0, 0, 0, texture, 0xff0000ff);

		AddVertex(px - offset, py + offset, pz + offset, -1, 0, 0, 0, 0, texture, 0xff0000ff);
		AddVertex(px - offset, py + offset, pz - offset, -1, 0, 0, 0, 0, texture, 0xffff0000);
		AddVertex(px - offset, py - offset, pz - offset, -1, 0, 0, 0, 0, texture, 0xff0000ff);
	}

	// Test the right:
	if (b[1][1][1] && !b[2][1][1])
	{
		// On the +X, facing -X
		AddVertex(px + offset, py + offset, pz + offset, +1, 0, 0, 0, 0, texture, 0xffff8800);
		AddVertex(px + offset, py - offset, pz + offset, +1, 0, 0, 0, 0, texture, 0xff00ff00);
		AddVertex(px + offset, py - offset, pz - offset, +1, 0, 0, 0, 0, texture, 0xffff8800);

		AddVertex(px + offset, py - offset, pz - offset, +1, 0, 0, 0, 0, texture, 0xffff8800);
		AddVertex(px + offset, py + offset, pz - offset, +1, 0, 0, 0, 0, texture, 0xff00ff00);
		AddVertex(px + offset, py + offset, pz + offset, +1, 0, 0, 0, 0, texture, 0xffff8800);
	}

	// Test the bottom:
	if (b[1][1][1] && !b[1][0][1])
	{
		// On the -Y, facing -Y
		AddVertex(px + offset, py - offset, pz - offset, 0, -1, 0, 0, 0, texture, 0xffff8800);
		AddVertex(px + offset, py - offset, pz + offset, 0, -1, 0, 0, 0, texture, 0xff00ff00);
		AddVertex(px - offset, py - offset, pz + offset, 0, -1, 0, 0, 0, texture, 0xffff8800);

		AddVertex(px - offset, py - offset, pz + offset, 0, -1, 0, 0, 0, texture, 0xffff8800);
		AddVertex(px - offset, py - offset, pz - offset, 0, -1, 0, 0, 0, texture, 0xff0000ff);
		AddVertex(px + offset, py - offset, pz - offset, 0, -1, 0, 0, 0, texture, 0xffff8800);
	}

	// Test the top:
	if (b[1][1][1] && !b[1][2][1])
	{
		// On the +Y, facing -Y
		AddVertex(px - offset, py + offset, pz + offset, 0, +1, 0, 0, 0, texture, 0xff0000ff);
		AddVertex(px + offset, py + offset, pz + offset, 0, +1, 0, 0, 0, texture, 0xffff8800);
		AddVertex(px + offset, py + offset, pz - offset, 0, +1, 0, 0, 0, texture, 0xff00ff00);

		AddVertex(px + offset, py + offset, pz - offset, 0, +1, 0, 0, 0, texture, 0xff00ff00);
		AddVertex(px - offset, py + offset, pz - offset, 0, +1, 0, 0, 0, texture, 0xffff0000);
		AddVertex(px - offset, py + offset, pz + offset, 0, +1, 0, 0, 0, texture, 0xff0000ff);
	}

	// Test the near side:
	if (b[1][1][1] && !b[1][1][0])
	{
		// On the -Z, facing -Z
		AddVertex(px + offset, py + offset, pz - offset, 0, 0, -1, 0, 0, texture, 0xff00ff00);
		AddVertex(px + offset, py - offset, pz - offset, 0, 0, -1, 0, 0, texture, 0xffff8800);
		AddVertex(px - offset, py - offset, pz - offset, 0, 0, -1, 0, 0, texture, 0xff0000ff);

		AddVertex(px - offset, py - offset, pz - offset, 0, 0, -1, 0, 0, texture, 0xff0000ff);
		AddVertex(px - offset, py + offset, pz - offset, 0, 0, -1, 0, 0, texture, 0xffff0000);
		AddVertex(px + offset, py + offset, pz - offset, 0, 0, -1, 0, 0, texture, 0xff00ff00);
	}

	// Test the far side:
	if (b[1][1][1] && !b[1][1][2])
	{
		// On the +Z, facing -Z
		AddVertex(px - offset, py - offset, pz + offset, 0, 0, +1, 0, 0, texture, 0xffff8800);
		AddVertex(px + offset, py - offset, pz + offset, 0, 0, +1, 0, 0, texture, 0xff00ff00);
		AddVertex(px + offset, py + offset, pz + offset, 0, 0, +1, 0, 0, texture, 0xffff8800);

		AddVertex(px + offset, py + offset, pz + offset, 0, 0, +1, 0, 0, texture, 0xffff8800);
		AddVertex(px - offset, py + offset, pz + offset, 0, 0, +1, 0, 0, texture, 0xff0000ff);
		AddVertex(px - offset, py - offset, pz + offset, 0, 0, +1, 0, 0, texture, 0xffff8800);
	}
}