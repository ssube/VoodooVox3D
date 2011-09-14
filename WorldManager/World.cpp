#include "World.hpp"
#include "Block.hpp"

#include "VectorMath.hpp"

#include "RenderEngine.hpp"

World * World::Create(RenderEngine * render)
{
    return new World(render);
}

void World::Destroy()
{
    delete this;
}

World::World(RenderEngine * render)
    : mRenderEngine(render)
{
    memset(mBlocks, 0, sizeof(Block*) * WORLD_BLOCKS_COUNT);
    memset(mObjects, 0, sizeof(RenderObject*) * WORLD_CHUNKS_COUNT);

    mDictionary = new BlockDictionary();
    mWorldLoader = WorldLoader::Create("world.db3", mDictionary);

    uvec3 relativeCoord;

    for ( relativeCoord.x = 0; relativeCoord.x < WORLD_CHUNKS; ++relativeCoord.x )
    {
        for ( relativeCoord.y = 0; relativeCoord.y < WORLD_CHUNKS; ++relativeCoord.y )
        {
            for ( relativeCoord.z = 0; relativeCoord.z < WORLD_CHUNKS; ++relativeCoord.z )
            {
                this->LoadChunk(relativeCoord);
            }
        }
    }
}

World::~World(void)
{
    for ( size_t x = 0; x < WORLD_CHUNKS; ++x )
    {
        for ( size_t y = 0; y < WORLD_CHUNKS; ++y )
        {
            for ( size_t z = 0; z < WORLD_CHUNKS; ++z )
            {
                delete mObjects[x][y][z];
            }
        }
    }
}

ivec3 World::GetBlockPos(fvec3 pos)
{
    return Common::Math::Floor(pos / BLOCK_SIZE);
}

uvec3 World::GetRelativeChunk(fvec3 pos)
{
    return ( Common::Math::Floor(pos / CHUNK_SIZE) );
}

ivec3 World::GetAbsoluteChunk(fvec3 pos)
{
    uvec3 relativeComp = GetRelativeChunk(pos);
    return mOriginChunk + relativeComp;
}

void World::Update()
{
    // Handle block physics
}

void World::UpdateChunks(fvec3 & pos)
{
    using namespace Common::Math;

    // Find the new center chunk
    uvec3 newOrigin = GetAbsoluteChunk(pos);
    //uvec3 center(1, 1, 1);

    if ( newOrigin == mOriginChunk )
    {
        return;
    } else {
        // We need to update, find out how much
        mOriginChunk = newOrigin;

        //if ( Any(shift, WORLD_CHUNKS) )
        //{
        //     Full reload
            uvec3 chunk;
            for ( chunk.x = 0; chunk.x < WORLD_CHUNKS; ++chunk.x )
            {
                for ( chunk.y = 0; chunk.y < WORLD_CHUNKS; ++chunk.y )
                {
                    for ( chunk.z = 0; chunk.z < WORLD_CHUNKS; ++chunk.z )
                    {
                        ClearChunk(chunk);
                        LoadChunk(chunk);
                    }
                }
            }
        //} else {
        //     Partial reload; find affected chunks and reload
        //}

        //pos = pos - ( center * CHUNK_SIZE );
        pos = WrapBoth<fvec3>(pos, newOrigin * CHUNK_SIZE, ( newOrigin + 1.0f ) * CHUNK_SIZE, 1.0f);
    }
}

fvec3 World::UpdatePosition(fvec3 pos, fvec3 shift)
{
    using namespace Common::Math;

    // Need to find the newPos' block, get the speed from that, and lerp
    //    between oldPos and newPos based on that
    fvec3 final;
    Block * newBlock = GetBlock(pos + shift);

    if ( newBlock )
    {
        fvec3 realShift = shift * newBlock->Speed;
        final = pos + realShift;
    } else {
        final = pos + shift;
    }

    this->UpdateChunks(final);

    return final;
}

void World::ClearChunk(uvec3 chunk)
{
    uvec3 relativeBlock;

    for ( relativeBlock.x = 0; relativeBlock.x < CHUNK_BLOCKS; ++relativeBlock.x )
    {
        for ( relativeBlock.y = 0; relativeBlock.y < CHUNK_BLOCKS; ++relativeBlock.y )
        {
            for ( relativeBlock.z = 0; relativeBlock.z < CHUNK_BLOCKS; ++relativeBlock.z )
            {
                uvec3 absoluteBlock = relativeBlock + ( chunk * CHUNK_BLOCKS );

                Block * block = INDEX3(mBlocks, absoluteBlock);

                if ( block )
                {
                    delete block;
                }

                INDEX3(mBlocks, relativeBlock) = NULL;
            }
        }
    }
}

void World::LoadChunk(uvec3 relcoord)
{
    RawChunk chunk;

    ivec3 abschunk = relcoord + mOriginChunk;

    if ( mWorldLoader->LoadChunk(abschunk, &chunk) )
    {
        uvec3 relativeBlock;

        for ( relativeBlock.x = 0; relativeBlock.x < CHUNK_BLOCKS; ++relativeBlock.x )
        {
            for ( relativeBlock.y = 0; relativeBlock.y < CHUNK_BLOCKS; ++relativeBlock.y )
            {
                for ( relativeBlock.z = 0; relativeBlock.z < CHUNK_BLOCKS; ++relativeBlock.z )
                {
                    uvec3 absoluteBlock = relativeBlock + ( relcoord * CHUNK_BLOCKS );

                    uint8 id = INDEX3(chunk.BlockData, relativeBlock)[0];

                    if ( id > 0 )
                    {
                        INDEX3(mBlocks, absoluteBlock) = new Block(mDictionary->GetTemplate(id));
                    } else {
                        INDEX3(mBlocks, absoluteBlock) = NULL;
                    }
                }
            }
        }
    }

    RenderObject * ro = INDEX3(mObjects, relcoord);

    if ( ro ) { mRenderEngine->RemoveRenderObject(ro); delete ro; }

    ro = mRenderEngine->CreateRenderObject();

    ro->SetPosition(relcoord * CHUNK_SIZE); // Cross dependency

    INDEX3(mObjects, relcoord) = ro;

    mRenderEngine->AddRenderObject(ro);

    GenerateGeometry(relcoord);
}

void World::MoveChunk(uvec3 chunk, uvec3 dest)
{
    uvec3 relativeBlock;

    for ( relativeBlock.x = 0; relativeBlock.x < CHUNK_BLOCKS; ++relativeBlock.x )
    {
        for ( relativeBlock.y = 0; relativeBlock.y < CHUNK_BLOCKS; ++relativeBlock.y )
        {
            for ( relativeBlock.z = 0; relativeBlock.z < CHUNK_BLOCKS; ++relativeBlock.z )
            {
                uvec3 absoluteBlock = relativeBlock + ( chunk * CHUNK_BLOCKS );
                uvec3     destBlock = relativeBlock + (  dest * CHUNK_BLOCKS );

                INDEX3(mBlocks, destBlock) = INDEX3(mBlocks, absoluteBlock);
            }
        }
    }

    RenderObject * ro = INDEX3(mObjects, chunk);

    ro->SetPosition(chunk * CHUNK_SIZE); // Cross dependency

    std::swap(INDEX3(mObjects, dest), INDEX3(mObjects, chunk));
}

Block * World::GetBlock(fvec3 pos)
{
    using namespace Common::Math;

    if ( !Within(0.0f, WORLD_SIZE, pos) )
    {
        return NULL;
    }

    // Is within the world, find the chunk
    ivec3 blockPos = GetBlockPos( pos / BLOCK_SIZE );

    Block * block = INDEX3(mBlocks, blockPos);

    return block;
}

bool World::GetBlockNeighbors(uvec3 block, uint8 & faces, uint8 & texture)
{
    using namespace Common::Math;

    if ( Any<uint32>(block, WORLD_BLOCKS_COUNT-1 ) )
    {
        return false;
    }

    Block * cblock = INDEX3(mBlocks, block);

    if ( !cblock )
    {
        return false;
    } else if ( !cblock->Visible ) {
        return false;
    }

    texture = cblock->Texture;
    faces = 0;

    if ( block.x > 0 )
    {
        cblock = mBlocks[block.x-1][block.y][block.z];
        if ( cblock && cblock->Occludes )
        {
            faces |= 0x01;
        }
    }

    if ( block.x < (WORLD_BLOCKS_COUNT-1) )
    {
        cblock = mBlocks[block.x+1][block.y][block.z];
        if ( cblock && cblock->Occludes )
        {
            faces |= 0x02;
        }
    }

    if ( block.y > 0 )
    {
        cblock = mBlocks[block.x][block.y-1][block.z];
        if ( cblock && cblock->Occludes )
        {
            faces |= 0x04;
        }
    }

    if ( block.y < (WORLD_BLOCKS_COUNT-1) )
    {
        cblock = mBlocks[block.x][block.y+1][block.z];
        if ( cblock && cblock->Occludes )
        {
            faces |= 0x08;
        }
    }

    if ( block.z > 0 )
    {
        cblock = mBlocks[block.x][block.y][block.z-1];
        if ( cblock && cblock->Occludes )
        {
            faces |= 0x10;
        }
    }

    if ( block.z < (WORLD_BLOCKS_COUNT-1) )
    {
        cblock = mBlocks[block.x][block.y][block.z+1];
        if ( cblock && cblock->Occludes )
        {
            faces |= 0x20;
        }
    }

    if ( faces == 0 )
    {
        return false;
    }

    return true;
}

void World::GenerateGeometry(uvec3 position)
{
    using namespace Common;

    uint32 lodOffset[LOD_COUNT], lodVCount[LOD_COUNT];

    mGeometryVector.clear();

    for ( uint32 lod = 0; lod < LOD_COUNT; ++lod )
    {
        lodOffset[lod] = mGeometryVector.size();

        Common::uvec3 blockpos;

        for (blockpos.x = CHUNK_BLOCKS*position.x; blockpos.x < CHUNK_BLOCKS*(position.x+1); blockpos.x += pow(2.0f, (int)lod))
        {
            for (blockpos.y = CHUNK_BLOCKS*position.y; blockpos.y < CHUNK_BLOCKS*(position.y+1); blockpos.y += pow(2.0f, (int)lod))
            {
                for (blockpos.z = CHUNK_BLOCKS*position.z; blockpos.z < CHUNK_BLOCKS*(position.z+1); blockpos.z += pow(2.0f, (int)lod))
                {
                    this->ProcessPoint(lod, blockpos, position);
                }
            }
        }

        lodVCount[lod] = mGeometryVector.size() - lodOffset[lod];
    }

    INDEX3(mObjects, position)->SetGeometry(mGeometryVector.size(), lodOffset, lodVCount, mGeometryVector.begin()._Ptr); // Cross dependency
}

void World::ProcessPoint(uint8 lod, uvec3 position, uvec3 chunk)
{
    // Flag names and chunk sizes
    int32 minSize = 0, maxSize = 3;

    Block * block = INDEX3(mBlocks, position);

    // Build the flags
    size_t texture = 0, texCol = 0, texRow = 0;
    float texX, texY;
    bool b[3][3][3];
    memset(b, 0, sizeof(bool)*3*3*3);

    if ( !block )
    {
        return;
    } else {
        b[1][1][1] = block->Occludes;
        texture = block->Texture;
        texRow = texture / 2;
        texCol = texture % 2;

        texX = 0.5f * texCol;
        texY = 0.5f * texRow;

        if ( !block->Visible )
        {
            return;
        }
    }

    if ( position.x > 0 )
    {
        block = mBlocks[position.x-1][position.y][position.z];
        if ( block )
        {
            b[0][1][1] = block->Occludes;
        }
    }

    if ( position.x < (WORLD_BLOCKS-1) )
    {
        block = mBlocks[position.x+1][position.y][position.z];
        if ( block )
        {
            b[2][1][1] = block->Occludes;
        }
    }

    if ( position.y > 0 )
    {
        block = mBlocks[position.x][position.y-1][position.z];
        if ( block )
        {
            b[1][0][1] = block->Occludes;
        }
    }

    if ( position.y < (WORLD_BLOCKS-1) )
    {
        block = mBlocks[position.x][position.y+1][position.z];
        if ( block )
        {
            b[1][2][1] = block->Occludes;
            //if ( block->Occludes )
            //{
            //    lighting = block->Light - 0.02f;
            //}
        }
    }

    if ( position.z > 0 )
    {
        block = mBlocks[position.x][position.y][position.z-1];
        if ( block )
        {
            b[1][1][0] = block->Occludes;
        }
    }

    if ( position.z < (WORLD_BLOCKS-1) )
    {
        block = mBlocks[position.x][position.y][position.z+1];
        if ( block )
        {
            b[1][1][2] = block->Occludes;
        }
    }

    //mBlocks[x][y][z]->Light = lighting;
    float vrshade = ( 255.0f - ( rand() % 32 ) ) / 255.0f;// * lighting;
    float vgshade = ( 255.0f - ( rand() % 24 ) ) / 255.0f;// * lighting;
    float vbshade = ( 255.0f - ( rand() % 48 ) ) / 255.0f;// * lighting;

    // Calculate the offset and positions
    float offset = BLOCK_SIZE * (lod + 1);
    float px = ( position.x * BLOCK_SIZE ) - ( chunk.x * CHUNK_SIZE );
    float py = ( position.y * BLOCK_SIZE ) - ( chunk.y * CHUNK_SIZE );
    float pz = ( position.z * BLOCK_SIZE ) - ( chunk.z * CHUNK_SIZE );

    // Now, build geometry. We'll test each of the 6 surfaces and create a quad
    // if one should exist. The important thing here is to make sure the quad
    // points the right way, that is, toward the empty block.

    // Test the left:
    if (!b[0][1][1])
    {
        // On the -X, facing -X
        mGeometryVector.push_back(Vertex(         px,          py,          pz, -1, 0, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px,          py, pz + offset, -1, 0, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, -1, 0, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));

        mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, -1, 0, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px, py + offset,          pz, -1, 0, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px,          py,          pz, -1, 0, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
    }

    // Test the right:
    if (!b[2][1][1])
    {
        // On the +X, facing +X
        mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset,          py, pz + offset, +1, 0, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset,          py,          pz, +1, 0, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));

        mGeometryVector.push_back(Vertex(px + offset,          py,          pz, +1, 0, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, +1, 0, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, +1, 0, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
    }

    // Test the bottom:
    if (!b[1][0][1])
    {
        // On the -Y, facing -Y
        mGeometryVector.push_back(Vertex(px + offset,          py,          pz, 0, -1, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset,          py, pz + offset, 0, -1, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px,          py, pz + offset, 0, -1, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));

        mGeometryVector.push_back(Vertex(         px,          py, pz + offset, 0, -1, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px,          py,          pz, 0, -1, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset,          py,          pz, 0, -1, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
    }

    // Test the top:
    if (!b[1][2][1])
    {
        // On the +Y, facing -Y
        mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, 0, +1, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, +1, 0, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, 0, +1, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));

        mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, 0, +1, 0, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px, py + offset,          pz, 0, +1, 0,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, 0, +1, 0,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
    }

    // Test the near side:
    if (!b[1][1][0])
    {
        // On the -Z, facing -Z
        mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, 0, 0, -1, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset,          py,          pz, 0, 0, -1, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px,          py,          pz, 0, 0, -1,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));

        mGeometryVector.push_back(Vertex(         px,          py,          pz, 0, 0, -1,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px, py + offset,          pz, 0, 0, -1,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset, py + offset,          pz, 0, 0, -1, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
    }

    // Test the far side:
    if (!b[1][1][2])
    {
        // On the +Z, facing -Z
        mGeometryVector.push_back(Vertex(         px,          py, pz + offset, 0, 0, +1,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset,          py, pz + offset, 0, 0, +1, texX + 0.5f,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));

        mGeometryVector.push_back(Vertex(px + offset, py + offset, pz + offset, 0, 0, +1, texX + 0.5f, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px, py + offset, pz + offset, 0, 0, +1,        texX, texY + 0.5f, 0, vrshade, vgshade, vbshade, 1.0f));
        mGeometryVector.push_back(Vertex(         px,          py, pz + offset, 0, 0, +1,        texX,        texY, 0, vrshade, vgshade, vbshade, 1.0f));
    }
}
