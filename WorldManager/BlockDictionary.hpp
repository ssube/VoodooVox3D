#pragma once

#include "Interface_WorldManager.hpp"
#include <vector>

#define IMPORT_COMMON
#include "CommonTypes.hpp"

#ifndef IMPORT_WORLDMANAGER
#   include <map>
#endif

using namespace std;
using namespace Common;

class WORLDMANAGER_API BlockDictionary
{
public:
    BlockDictionary(void);
    ~BlockDictionary(void);

    virtual void AddTemplate(BlockTemplate * temp);

    virtual vector<uint8> GetTemplateList();
    virtual BlockTemplate * GetTemplate(uint8 id);

#ifndef IMPORT_WORLDMANAGER
private:
    map<uint8, BlockTemplate*> mTemplates;
#endif
};
