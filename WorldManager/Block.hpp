#pragma once

#include "Interface_WorldManager.hpp"
#include "BlockTemplate.hpp"

class WORLDMANAGER_API Block :
    public BlockTemplate
{
public:
    Block(BlockTemplate * temp);
    ~Block(void);

    int Health;
    float Light;
};
