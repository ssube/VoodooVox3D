#pragma once

#include <math.h>
#include <vector>

// Common includes
#include "CommonTypes.hpp"
#include "Math.hpp"
#include "CoherentNoise.hpp"

using namespace Common;

struct __declspec(dllexport) WorldSegment
{
    Vector3<size_t> Origin;
    Vector3<size_t> Size;

    int * Blocks;

    ~WorldSegment()
    {
        if ( Blocks ) delete Blocks;
    }

    size_t Index(Vector3<size_t> pos)
    {
        return (pos.x*Size.y*Size.z)+(pos.y*Size.z)+pos.z;
    }

    size_t Index(size_t x, size_t y, size_t z)
    {
        return (x*Size.y*Size.z)+(y*Size.z)+z;
    }
};

class __declspec(dllexport) WorldGenerator
{
public:
    WorldGenerator(std::vector<int> typeIDs);
    ~WorldGenerator(void);

    WorldSegment * Generate(int seed, Vector3<size_t> origin, Vector3<size_t> size);

private:
    std::vector<int> mTypes;
};
