#include "BlockDictionary.hpp"
#include <fstream>

void * BlockDictionary::Serialize(unsigned long * size)
{
	unsigned long finalsize = 0;
	unsigned long templatesize = 0;

	// Attribute count (one size_t)
	finalsize += sizeof(size_t);

	// Add each attribute
	size_t attrCount = mAttributes.size();
	for ( size_t cAttr = 0; cAttr < attrCount; ++cAttr )
	{
		finalsize += mAttributes[cAttr].length() + 1;
	}

	// Template count
	finalsize += sizeof(size_t);

	map<int, BlockTemplate*>::iterator tempIter = mTemplates.begin();
	while ( tempIter != mTemplates.end() )
	{
		for ( size_t cAttr = 0; cAttr < attrCount; ++cAttr )
		{
			AttributeType type = BlockTemplate::GetType(mAttributes[cAttr]);
			if ( type == AT_Bool)
			{
				finalsize += tempIter->second->GetAttribute(mAttributes[cAttr], BOOL_NULL);
			} else if ( type == AT_Int ) {
				finalsize += tempIter->second->GetAttribute(mAttributes[cAttr], INT_NULL);
			} else if ( type == AT_Float ) {
				finalsize += tempIter->second->GetAttribute(mAttributes[cAttr], FLOAT_NULL);
			} else if ( type == AT_String ) {
				finalsize += tempIter->second->GetAttribute(mAttributes[cAttr], STRING_NULL) + 1;
			}
		}

		++tempIter;
	}

	// Done, build the final buffer
	void * finalbuffer = malloc(finalsize);
	memset(finalbuffer, 0, finalsize);

	char * working = (char*)finalbuffer;

	memcpy(working, &attrCount, sizeof(size_t));
	working += sizeof(size_t);

	for ( size_t cAttr = 0; cAttr < attrCount; ++cAttr )
	{
		size_t len = mAttributes[cAttr].length();
		memcpy(working, mAttributes[cAttr].data(), len);
		working += len;
		working[0] = 0x00;
		++working;
	}

	size_t templateCount = mTemplates.size();
	memcpy(working, &templateCount, sizeof(size_t));
	working += sizeof(size_t);

	tempIter = mTemplates.begin();
	while ( tempIter != mTemplates.end() )
	{
		for ( size_t cAttr = 0; cAttr < attrCount; ++cAttr )
		{
			AttributeType type = BlockTemplate::GetType(mAttributes[cAttr]);
			if ( type == AT_Bool)
			{
				bool temp;
				int len = tempIter->second->GetAttribute(mAttributes[cAttr], &temp);
				memcpy(working, &temp, len);
				working += len;
			} else if ( type == AT_Int ) {
				int temp;
				int len = tempIter->second->GetAttribute(mAttributes[cAttr], &temp);
				memcpy(working, &temp, len);
				working += len;
			} else if ( type == AT_Float ) {
				float temp;
				int len = tempIter->second->GetAttribute(mAttributes[cAttr], &temp);
				memcpy(working, &temp, len);
				working += len;
			} else if ( type == AT_String ) {
				string temp;
				int len = tempIter->second->GetAttribute(mAttributes[cAttr], &temp);
				memcpy(working, temp.data(), len);
				working += len;
				working[0] = 0x00;
				++working;
			}
		}

		++tempIter;
	}

	*size = finalsize;
	return finalbuffer;
}

BlockDictionary * BlockDictionary::Deserialize(void * buffer)
{
	BlockDictionary * dict = new BlockDictionary();

	char * working = (char*)buffer;
	size_t attrCount = *(size_t*)working;
	working += sizeof(size_t);

	for ( size_t cAttr = 0; cAttr < attrCount; ++cAttr )
	{
		size_t len = strlen(working);
		
		dict->mAttributes.push_back(working);

		working += len + 1;
	}

	size_t templateCount = *(size_t*)working;
	working += sizeof(size_t);

	for ( size_t cTemp = 0; cTemp < templateCount; ++cTemp )
	{
		BlockTemplate * temp = new BlockTemplate();

		for ( size_t cAttr = 0; cAttr < attrCount; ++cAttr )
		{
			string attr = dict->mAttributes[cAttr];

			AttributeType type = BlockTemplate::GetType(attr);
			switch ( type )
			{
			case AT_Bool:
				temp->AddAttribute(attr, (bool*)working);
				working += temp->GetAttribute(attr, BOOL_NULL);
				break;
			case AT_Int:
				temp->AddAttribute(attr, (int*)working);
				working += temp->GetAttribute(attr, INT_NULL);
				break;
			case AT_Float:
				temp->AddAttribute(attr, (float*)working);
				working += temp->GetAttribute(attr, FLOAT_NULL);
				break;
			case AT_String:
				temp->AddAttribute(attr, working);
				working += temp->GetAttribute(attr, STRING_NULL) + 1;
				break;
			}
		}

		int id = 0;
		temp->GetAttribute("iID", &id);
		dict->mTemplates[id] = temp;
	}

	return dict;
}

BlockDictionary * BlockDictionary::FromFile(string filename)
{
	fstream file;
	file.open(filename.c_str(), ios_base::in);

	file.seekg(0, ios_base::end);
	ios::pos_type len = file.tellg();
	char * buffer = new char[len];
	file.seekg(0, ios_base::beg);
	file.get(buffer, len);

	BlockDictionary * dict = BlockDictionary::Deserialize(buffer);
	delete[] buffer;
	return dict;
}

void BlockDictionary::ToFile(string filename)
{
	fstream file;

	file.open(filename.c_str(), ios_base::trunc|ios_base::out);

	unsigned long size;
	void * data = this->Serialize(&size);

	file.write((const char*)data, size);

	file.close();
}

BlockDictionary::BlockDictionary(void)
{
}

BlockDictionary::~BlockDictionary(void)
{
}

void BlockDictionary::AddAttribute(std::string attribute)
{
	mAttributes.push_back(attribute);
}

void BlockDictionary::AddTemplate(BlockTemplate * temp)
{
	int id = 0;
	temp->GetAttribute("iID", &id);
	mTemplates[id] = temp;
}

vector<int> BlockDictionary::GetTemplateList()
{
	vector<int> ids;
	map<int, BlockTemplate*>::iterator ittr = mTemplates.begin();
	while ( ittr != mTemplates.end() )
	{
		ids.push_back(ittr->first);
		++ittr;
	}
	return ids;
}
BlockTemplate * BlockDictionary::GetTemplate(int id)
{
	map<int, BlockTemplate*>::iterator ittr = mTemplates.find(id);
	if ( ittr != mTemplates.end() )
	{
		return ittr->second;
	} else {
		return NULL;
	}
}