#include "BlockDictionary.hpp"
#include <fstream>

void * BlockDictionary::Serialize(unsigned long * size)
{
    size_t templateCount = mTemplates.size();
    size_t finalsize = 0;
    finalsize += sizeof(size_t);
    finalsize += sizeof(BlockTemplate) * templateCount;

    // Done, build the final buffer
    void * finalbuffer = malloc(finalsize);
    memset(finalbuffer, 0, finalsize);

    char * working = (char*)finalbuffer;

    memcpy(working, &templateCount, sizeof(size_t));
    working += sizeof(size_t);

    map<int, BlockTemplate*>::iterator tempIter = mTemplates.begin();
    while ( tempIter != mTemplates.end() )
    {
        memcpy(working, tempIter->second, sizeof(BlockTemplate));
        working += sizeof(BlockTemplate);
        ++tempIter;
    }

    *size = finalsize;
    return finalbuffer;
}

BlockDictionary * BlockDictionary::Deserialize(void * buffer)
{
    BlockDictionary * dict = new BlockDictionary();

    char * working = (char*)buffer;
    
    size_t templateCount = *(size_t*)working;
    working += sizeof(size_t);

    for ( size_t cTemp = 0; cTemp < templateCount; ++cTemp )
    {
        BlockTemplate * temp = new BlockTemplate();
        memcpy(temp, working, sizeof(BlockTemplate));
        dict->mTemplates[temp->ID] = temp;
        working += sizeof(BlockTemplate);
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

void BlockDictionary::AddTemplate(BlockTemplate * temp)
{
    mTemplates[temp->ID] = temp;
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
