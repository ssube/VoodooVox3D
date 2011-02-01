#include "BlockDictionary.hpp"

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

		AttributeType type = BlockTemplate::GetType(mAttributes[cAttr]);
		switch ( type )
		{
		case AT_Bool:
			templatesize += 1;
			break;
		case AT_Float:
		case AT_Int:
			templatesize += 4;
			break;
		case AT_String:
			templatesize += 32;
			break;
		}
	}

	finalsize += templatesize * mTemplates.size();

	// Done, build the final buffer
	void * finalbuffer = malloc(finalsize);
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

	map<int, BlockTemplate*>::iterator tempIter = mTemplates.begin();
	while ( tempIter != mTemplates.end() )
	{
		for ( size_t cAttr = 0; cAttr < attrCount; ++cAttr )
		{
			AttributeType type = BlockTemplate::GetType(mAttributes[cAttr]);
			switch ( type )
			{
			case AT_Bool:
				memcpy(working, tempIter->second->GetAttribute<bool>(mAttributes[cAttr]), 1);
				working += 1;
				break;
			case AT_Float:
				memcpy(working, tempIter->second->GetAttribute<float>(mAttributes[cAttr]), 4);
				templatesize += 4;
				break;
			case AT_Int:
				memcpy(working, tempIter->second->GetAttribute<int>(mAttributes[cAttr]), 4);
				templatesize += 4;
				break;
			case AT_String:
				memcpy(working, tempIter->second->GetAttribute<char>(mAttributes[cAttr]), 4);
				templatesize += 32;
				break;
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
		//char buffer[256];
		size_t len = strlen(working);
		
		dict->mAttributes.push_back(working);

		working += len + 1;
	}

	return dict;
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
	int * id = temp->GetAttribute<int>("iID");
	mTemplates[*id] = temp;
}