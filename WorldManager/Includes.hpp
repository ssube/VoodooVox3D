#pragma  once

#include <string>
#include <vector>
#include <list>
#include <map>

#include <windows.h>

#ifndef WORLDMANAGER_IMPORT
#    define WORLDMANAGER_API __declspec(dllexport)
#else
#    define WORLDMANAGER_API __declspec(dllimport)
#endif

#define CHUNK_BLOCKS 16
#define WORLD_CHUNKS 8
#define WORLD_BLOCKS WORLD_CHUNKS * CHUNK_BLOCKS

#define BLOCK_SIZE 5.0f
#define CHUNK_SIZE BLOCK_SIZE * CHUNK_BLOCKS
#define WORLD_SIZE CHUNK_SIZE * WORLD_CHUNKS
