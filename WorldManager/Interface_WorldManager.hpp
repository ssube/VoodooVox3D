#pragma once

#ifndef IMPORT_WORLDMANAGER
#    define WORLDMANAGER_API __declspec(dllexport)
#else
#    define WORLDMANAGER_API __declspec(dllimport)
#endif

struct BlockTemplate;
class Block;
class BlockDictionary;
class World;