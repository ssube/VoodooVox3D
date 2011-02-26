#include "GeometryBuilder.hpp"

const fvec3 normals[6] =
{
    fvec3(-1,  0,  0),
    fvec3(+1,  0,  0),
    fvec3( 0, -1,  0),
    fvec3( 0, +1,  0),
    fvec3( 0,  0, -1),
    fvec3( 0,  0, +1),
};

DWORD WINAPI GeometryBuilder(LPVOID lpParameter)
{
    GeometryBuilderDesc * desc = (GeometryBuilderDesc*)lpParameter;

    // Create the geometry buffers
    std::vector<Vertex> geometry;

    // Generate the range coordinates for the chunk
    uvec3 minCoords = desc->mRelativeChunk * CHUNK_BLOCKS;
    uvec3 maxCoords = minCoords + uvec3( CHUNK_BLOCKS - 1 );

    uvec3 curCoords = minCoords;
    uint8 faces, texture;

    // Loop through the blocks
    for ( curCoords.x = minCoords.x; curCoords.x < maxCoords.x; ++curCoords.x )
    {
        for ( curCoords.y = minCoords.y; curCoords.y < maxCoords.y; ++curCoords.y )
        {
            for ( curCoords.z = minCoords.z; curCoords.z < maxCoords.z; ++curCoords.z )
            {
                if ( desc->mWorld->GetBlockNeighbors(curCoords, faces, texture) )
                {
                    fvec3 corners[8] =
                    {
                        fvec3( curCoords.x    * BLOCK_SIZE,  curCoords.y    * BLOCK_SIZE,  curCoords.z    * BLOCK_SIZE),
                        fvec3( curCoords.x    * BLOCK_SIZE,  curCoords.y    * BLOCK_SIZE, (curCoords.z+1) * BLOCK_SIZE),
                        fvec3( curCoords.x    * BLOCK_SIZE, (curCoords.y+1) * BLOCK_SIZE,  curCoords.z    * BLOCK_SIZE),
                        fvec3( curCoords.x    * BLOCK_SIZE, (curCoords.y+1) * BLOCK_SIZE, (curCoords.z+1) * BLOCK_SIZE),
                        fvec3((curCoords.x+1) * BLOCK_SIZE,  curCoords.y    * BLOCK_SIZE,  curCoords.z    * BLOCK_SIZE),
                        fvec3((curCoords.x+1) * BLOCK_SIZE,  curCoords.y    * BLOCK_SIZE, (curCoords.z+1) * BLOCK_SIZE),
                        fvec3((curCoords.x+1) * BLOCK_SIZE, (curCoords.y+1) * BLOCK_SIZE,  curCoords.z    * BLOCK_SIZE),
                        fvec3((curCoords.x+1) * BLOCK_SIZE, (curCoords.y+1) * BLOCK_SIZE, (curCoords.z+1) * BLOCK_SIZE),
                    };

                    uint8 texCol = texture % 2;
                    uint8 texRow = ( texture - texCol ) / 2;
                    float tx = texCol * 0.5f;
                    float ty = texRow * 0.5f;

                    fvec3 texture[4] =
                    {
                        fvec3(tx,        ty,        0.0f),
                        fvec3(tx,        ty + 0.5f, 0.0f),
                        fvec3(tx + 0.5f, ty,        0.0f),
                        fvec3(tx + 0.5f, ty + 0.5f, 0.0f),
                    };

                    fvec4 color;
                    color.r = ( rand() / 65536.0f ) + 0.5f;
                    color.g = ( rand() / 65536.0f ) + 0.5f;
                    color.b = ( rand() / 65536.0f ) + 0.5f;
                    color.a = 1.0f;

                    if ( faces & 0x01 )
                    {
                        geometry.push_back(Vertex(corners[0], normals[0], texture[0], color));
                        geometry.push_back(Vertex(corners[1], normals[0], texture[1], color));
                        geometry.push_back(Vertex(corners[3], normals[0], texture[3], color));
                        geometry.push_back(Vertex(corners[3], normals[0], texture[3], color));
                        geometry.push_back(Vertex(corners[2], normals[0], texture[2], color));
                        geometry.push_back(Vertex(corners[0], normals[0], texture[0], color));
                    }
                    if ( faces & 0x02 )
                    {
                        geometry.push_back(Vertex(corners[3], normals[1], texture[3], color));
                        geometry.push_back(Vertex(corners[1], normals[1], texture[1], color));
                        geometry.push_back(Vertex(corners[0], normals[1], texture[0], color));
                        geometry.push_back(Vertex(corners[0], normals[1], texture[0], color));
                        geometry.push_back(Vertex(corners[2], normals[1], texture[2], color));
                        geometry.push_back(Vertex(corners[3], normals[1], texture[3], color));
                    }
                    // Bottom
                    if ( faces & 0x04 )
                    {
                        geometry.push_back(Vertex(corners[4], normals[2], texture[3], color));
                        geometry.push_back(Vertex(corners[5], normals[2], texture[1], color));
                        geometry.push_back(Vertex(corners[1], normals[2], texture[0], color));
                        geometry.push_back(Vertex(corners[1], normals[2], texture[0], color));
                        geometry.push_back(Vertex(corners[0], normals[2], texture[2], color));
                        geometry.push_back(Vertex(corners[4], normals[2], texture[3], color));
                    }
                    // Top
                    if ( faces & 0x08 )
                    {
                        geometry.push_back(Vertex(corners[3], normals[3], texture[3], color));
                        geometry.push_back(Vertex(corners[5], normals[3], texture[1], color));
                        geometry.push_back(Vertex(corners[4], normals[3], texture[0], color));
                        geometry.push_back(Vertex(corners[4], normals[3], texture[0], color));
                        geometry.push_back(Vertex(corners[2], normals[3], texture[2], color));
                        geometry.push_back(Vertex(corners[3], normals[3], texture[3], color));
                    }
                    // Far side
                    if ( faces & 0x10 )
                    {
                        geometry.push_back(Vertex(corners[6], normals[4], texture[3], color));
                        geometry.push_back(Vertex(corners[4], normals[4], texture[1], color));
                        geometry.push_back(Vertex(corners[0], normals[4], texture[0], color));
                        geometry.push_back(Vertex(corners[0], normals[4], texture[0], color));
                        geometry.push_back(Vertex(corners[2], normals[4], texture[2], color));
                        geometry.push_back(Vertex(corners[6], normals[4], texture[3], color));
                    }
                    // Near side
                    if ( faces & 0x20 )
                    {
                        geometry.push_back(Vertex(corners[1], normals[5], texture[3], color));
                        geometry.push_back(Vertex(corners[5], normals[5], texture[1], color));
                        geometry.push_back(Vertex(corners[7], normals[5], texture[0], color));
                        geometry.push_back(Vertex(corners[7], normals[5], texture[0], color));
                        geometry.push_back(Vertex(corners[3], normals[5], texture[2], color));
                        geometry.push_back(Vertex(corners[1], normals[5], texture[3], color));
                    }
                }
            }
        }
    }

    uint32 offset[LOD_COUNT];
    uint32 counts[LOD_COUNT];
    memset(offset, 0, sizeof(uint32) * LOD_COUNT);
    memset(counts, 0, sizeof(uint32) * LOD_COUNT);
    counts[0] = geometry.size();

    desc->mRenderObject->SetGeometry(counts[0], offset, counts, geometry.begin()._Ptr);
    desc->mRenderObject->SetPosition(desc->mRelativeChunk * CHUNK_SIZE);

    return TRUE;
}

