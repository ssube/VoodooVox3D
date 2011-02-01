#pragma once

#include "Includes.hpp"
#include "BlockTemplate.hpp"

using namespace std;

class __declspec(dllexport) BlockDictionary
{
public:
	// Serialization functions
	static BlockDictionary * Deserialize(void * buffer);
	void * Serialize(unsigned long * size);

	BlockDictionary(void);
	~BlockDictionary(void);

	void AddAttribute(string attribute);
	void AddTemplate(BlockTemplate * temp);

private:
	vector<string> mAttributes;
	map<int, BlockTemplate*> mTemplates;
};
