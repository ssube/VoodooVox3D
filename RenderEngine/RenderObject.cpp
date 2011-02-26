#include "RenderObject.hpp"

#include "World.hpp"

RenderObject::RenderObject
(
    LPDIRECT3DDEVICE9 device, 
    LPDIRECT3DVERTEXDECLARATION9 vertDecl
)
    : mDevice(device), mVertDecl(vertDecl), mVertBuffer(NULL), mHasGeometry(false), mVisible(true), mOccluded(false)
{
    ZeroMemory(mVertOffset, sizeof(size_t) * 4);
    ZeroMemory(mVertCount, sizeof(size_t) * 4);

    mDevice->AddRef();
    mVertDecl->AddRef();

    D3DXMatrixIdentity(&mTransform);

    HRESULT hrOQ = mDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, &mOcclQuery);
}

RenderObject::~RenderObject(void)
{
    //mEngine->DestroyRenderObject(this);

    if ( mVertBuffer )
    {
        mVertBuffer->Release();
    }
    if ( mVertDecl )
    {
        mVertDecl->Release();
    }
    if ( mVertOcclDecl )
    {
        mVertOcclDecl->Release();
    }
    if ( mOcclQuery )
    {
        mOcclQuery->Release();
    }
    if ( mOcclGeometry )
    {
        mOcclGeometry->Release();
    }
    if ( mDevice )
    {
        mDevice->Release();
    }
}

uint32 RenderObject::GetObjectType()
{
    return OBJECT_TYPE_RENDEROBJECT;
}

bool RenderObject::GetVisible()
{
    return ( mVisible && mHasGeometry && !mOccluded );
}

int32 RenderObject::GetLOD()
{
    return mLOD;
}

fvec3 RenderObject::GetPosition()
{
    return mPosition;
}

fvec3 RenderObject::GetSize()
{
    return fvec3(CHUNK_SIZE);
}

fmat4x4 RenderObject::GetTransform()
{
    return mTransform;
}

void RenderObject::SetVisible(bool visible)
{
    mVisible = visible;
}

void RenderObject::SetLOD(int32 lod)
{
    mLOD = lod;
}

void RenderObject::SetPosition(fvec3 pos)
{
    mPosition = pos;
    D3DXMatrixTranslation(&mTransform, pos.x, pos.y, pos.z);
}

void RenderObject::SetTransform(fmat4x4 trans)
{
    mTransform = trans;
}

size_t RenderObject::GetVertCount(int32 lod)
{
    return mVertCount[lod];
}

size_t RenderObject::GetVertOffset(int32 lod)
{
    return mVertOffset[lod];
}

void RenderObject::SetGeometry(uint32 total, uint32 * offsets, uint32 * counts, Vertex * verts)
{
    if ( mVertBuffer )
    {
        mVertBuffer->Release();
        mVertBuffer = NULL;
    }

    memcpy(mVertOffset, offsets, sizeof(uint32) * LOD_COUNT);
    memcpy(mVertCount,  counts,  sizeof(uint32) * LOD_COUNT);

    // Check for counts
    if ( total == 0 )
    {
        mHasGeometry = false;
    } else {
        mHasGeometry = true;

        UINT vbSize = sizeof(Vertex) * total;

        if ( FAILED(mDevice->CreateVertexBuffer(vbSize, D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &mVertBuffer, NULL) ) )
        {
            throw std::runtime_error("Unable to create vertex buffer.");
        }

        VOID * vbVerts;

        HRESULT hrVB = mVertBuffer->Lock(0, vbSize, &vbVerts, 0);

        if ( FAILED(hrVB) )
        {
            throw std::runtime_error("Error locking vertex buffer.");
        }

        memcpy(vbVerts, verts, vbSize);

        mVertBuffer->Unlock();
    }
}

void RenderObject::Render(int32 lod)
{
    using namespace Common::Math;

    int32 clod = ( lod == -1 ) ? mLOD : Clamp(0, LOD_COUNT, lod);

    if ( this->mVertCount[0] == 0 ) return;

    mDevice->SetTransform(D3DTS_WORLD, &mTransform);

    mDevice->SetStreamSource(0, mVertBuffer, mVertOffset[0] * sizeof(Vertex), sizeof(Vertex));
    mDevice->SetVertexDeclaration(mVertDecl);

    mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mVertCount[0] / 3);
}

uint32 RenderObject::UpdateOcclusion()
{
    if ( !mVisible || !mHasGeometry )
    {
        mOccluded = true;
        return 0;
    }

    DWORD pixels = 0;
    HRESULT queryDone = mOcclQuery->GetData(&pixels, sizeof(DWORD), 0);

    if ( queryDone == S_OK )
    {
        mOccluded = ( pixels == 0 );
    }

    mOcclQuery->Issue(D3DISSUE_BEGIN);

    this->Render(mLOD);

    mOcclQuery->Issue(D3DISSUE_END);

    return pixels;
}
