#pragma once

#include "Includes.hpp"

#include "World.hpp"

// Render engine includes
#define RENDERENGINE_INTERFACE
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
