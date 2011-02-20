#pragma once

#include "Includes.hpp"

using namespace std;
using namespace Common;

class WorldLoader
{
public:
	WorldLoader(string name);
	~WorldLoader();

	bool LoadChunk(ivec3 position, size_t * blocks, void * data);
	bool SaveChunk(ivec3 position, size_t * blocks, void * data);

	void * Compress(size_t size, void * data);
	void * Decompress(size_t size, void * data);

private:
	sqlite3 * mDatabase;
	sqlite3_stmt * mStmtLoad, * mStmtSave;
};
