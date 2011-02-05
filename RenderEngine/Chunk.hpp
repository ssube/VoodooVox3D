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
	void GenerateGeometry();

private:
	static const size_t ChunkSize = 16;

	Generator * mGen;
	Block * mBlocks[ChunkSize][ChunkSize][ChunkSize];

	bool mDirty;

	vector<Vertex> mGeometryVector;
	Vertex * mGeometry;
	size_t mGeometryCount;

	void ProcessPoint(int x, int y, int z);
	void AddVertex(float x, float y, float z, float nx, float ny, float nz, float tu, float tv, float tw, unsigned long color);
};
