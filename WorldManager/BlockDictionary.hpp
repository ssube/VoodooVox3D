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

	static BlockDictionary * FromFile(string filename);
	void ToFile(string filename);

	BlockDictionary(void);
	~BlockDictionary(void);

	void AddTemplate(BlockTemplate * temp);

	vector<int> GetTemplateList();
	BlockTemplate * GetTemplate(int id);

private:
	map<int, BlockTemplate*> mTemplates;
};
