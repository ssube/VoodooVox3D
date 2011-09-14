#pragma once

#ifndef IMPORT_WORLDMANAGER

#include <windows.h>

#include "Interface_WorldManager.hpp"
#include "WorldDefs.hpp"
#include "World.hpp"

// Render engine includes
#define IMPORT_RENDERENGINE
#include "RenderEngine.hpp"
#include "RenderObject.hpp"

// Threading entrypoint
struct GeometryBuilderDesc
{
    World * mWorld;
    ivec3 mChunk;
    uvec3 mRelativeChunk;
    RenderObject * mRenderObject;
};

DWORD WINAPI GeometryBuilder(LPVOID lpParameter);

#endif