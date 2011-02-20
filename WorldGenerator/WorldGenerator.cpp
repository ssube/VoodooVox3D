#include "WorldGenerator.hpp"

WorldGenerator::WorldGenerator(std::vector<int> typeIDs)
{
    mTypes = typeIDs;
}

WorldGenerator::~WorldGenerator(void)
{
}

WorldSegment * WorldGenerator::Generate
(
    int seed, 
    Vector3<size_t> origin,
    //size_t originX, size_t originY, size_t originZ, 
    Vector3<size_t> size
    //size_t sizeX, size_t sizeY, size_t sizeZ
)
{
    using namespace Common::Noise;

    WorldSegment * world = new WorldSegment();
    world->Blocks = new int[size.x*size.y*size.z];
    world->Origin = origin;

    CoherentNoise * noiseThing = new CoherentNoise(seed);

    for ( size_t x = origin.x; x < origin.x + size.x; ++x )
    {
        for ( size_t y = origin.y; y < origin.y + size.y; ++y )
        {
            for ( size_t z = origin.z; z < origin.z + size.z; ++z )
            {
                float value = noiseThing->GetPoint(fvec3(x, y, z) / 4.0f);
                value *= mTypes.size();
                size_t index = (size_t)floor(value);

                world->Blocks[world->Index(x, y, z)] = index;
            }
        }
    }

    return world;
}

/*
int WorldGenerator::GetBlock(size_t absX, size_t absY, size_t absZ)
{
    if ( !mWorld ) return -1;

    return mWorld[(absX*sizeY*sizeZ)+(absY*sizeZ)+absZ];
}
*/

