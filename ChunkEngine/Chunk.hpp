#pragma once

#include "Includes.hpp"
#include "BlockTemplate.hpp"
#include "Block.hpp"
#include "BlockDictionary.hpp"

#include "WorldGenerator.hpp"
#include "VertexHeader.hpp"

using namespace std;

class __declspec(dllexport) Chunk
{
public:
	Chunk(BlockDictionary * dict);
	~Chunk(void);

	Vertex * GetGeometry();
	size_t GetGeometryCount();

private:
	static const size_t ChunkWidth = 4;
	static const size_t ChunkHeight = 4; // 16;

	Generator * mGen;
	Block * mBlocks[ChunkWidth][ChunkWidth][ChunkHeight];

	bool mDirty;

	vector<Vertex> mGeometry;

	void GenerateGeometry();
	void ProcessPoint(int x, int y, int z);
	void AddVertex(float x, float y, float z, float nx, float ny, float nz, float tu, float tv, float tw, unsigned long color);
};
