#pragma  once

#include <string>
#include <vector>
#include <list>
#include <map>

#include <windows.h>

#ifdef _DEBUG
#	define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>

#include <dinput.h>

#define BOOL_NULL   (bool*)   NULL
#define INT_NULL    (int*)    NULL
#define FLOAT_NULL  (float*)  NULL
#define STRING_NULL (string*) NULL

#define CHUNK_BLOCKS 8
#define WORLD_CHUNKS 3
#define WORLD_BLOCKS WORLD_CHUNKS * CHUNK_BLOCKS

#define BLOCK_SIZE 10.0f
#define CHUNK_SIZE BLOCK_SIZE * CHUNK_BLOCKS
#define WORLD_SIZE CHUNK_SIZE * WORLD_CHUNKS
