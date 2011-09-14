#include "BlockDictionary.hpp"

#include "BlockTemplate.hpp"

BlockDictionary::BlockDictionary(void)
    : mTemplates()
{
    mTemplates.clear();
}

BlockDictionary::~BlockDictionary(void)
{
    map<uint8, BlockTemplate*>::iterator tempittr = mTemplates.begin();

    while ( tempittr != mTemplates.end() )
    {
        delete tempittr->second;
        ++tempittr;
    }
}

void BlockDictionary::AddTemplate(BlockTemplate * temp)
{
    mTemplates[temp->ID] = temp;
}

vector<uint8> BlockDictionary::GetTemplateList()
{
    vector<uint8> ids;
    map<uint8, BlockTemplate*>::iterator ittr = mTemplates.begin();
    while ( ittr != mTemplates.end() )
    {
        ids.push_back(ittr->first);
        ++ittr;
    }
    return ids;
}
BlockTemplate * BlockDictionary::GetTemplate(uint8 id)
{
    map<uint8, BlockTemplate*>::iterator ittr = mTemplates.find(id);
    if ( ittr != mTemplates.end() )
    {
        return ittr->second;
    } else {
        return NULL;
    }
}
