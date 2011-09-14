#pragma once

#include "Interface_WorldManager.hpp"
#define IMPORT_COMMON
#include "CommonTypes.hpp"

using namespace Common;

struct WORLDMANAGER_API BlockTemplate
{
public:
    uint8 ID;
    uint8 DefaultHealth;
    char Name[16];
    float Speed;
    uint32 Texture;
    bool Occludes;
    bool Visible;
};
