#pragma  once

#include <string>
#include <vector>
#include <list>
#include <map>

#include <windows.h>

#define CHUNK_BLOCKS 8
#define WORLD_CHUNKS 5
#define WORLD_BLOCKS WORLD_CHUNKS * CHUNK_BLOCKS

#define BLOCK_SIZE 10.0f
#define CHUNK_SIZE BLOCK_SIZE * CHUNK_BLOCKS
#define WORLD_SIZE CHUNK_SIZE * WORLD_CHUNKS
