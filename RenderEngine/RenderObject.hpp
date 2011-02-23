#pragma once

#define LOD_COUNT 3
#define OCCL_THRESHOLD 8

#ifndef RENDERENGINE_INTERFACE
#    include "Includes.hpp"
#endif

#include "CommonTypes.hpp"

using namespace Common;

class RenderObject
    : GenericObject
{
public:
#ifndef RENDERENGINE_INTERFACE
    RenderObject(LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXDECLARATION9 vertDecl);
    ~RenderObject(void);
#endif

    virtual uint32  GetObjectType();
    virtual bool    GetVisible();
    virtual int32   GetLOD();
    virtual fvec3   GetPosition();
    virtual fvec3   GetSize();
    virtual fmat4x4 GetTransform();

    virtual void    SetVisible(bool visible);
    virtual void    SetLOD(int32 lod);
    virtual void    SetPosition(fvec3 pos);
    virtual void    SetTransform(fmat4x4 transform);

    virtual uint32  GetVertCount(int32 lod);
    virtual uint32  GetVertOffset(int32 lod);
    virtual void    SetGeometry(uint32 total, uint32 * offsets, uint32 * counts, Vertex * verts);

    virtual void    Render(int32 lod = -1);
    virtual uint32  UpdateOcclusion();

#ifndef RENDERENGINE_INTERFACE
private:
    int32 mLOD;
    uint32 mVertCount[LOD_COUNT];
    uint32 mVertOffset[LOD_COUNT];

    fvec3 mPosition;
    D3DXMATRIX mTransform;

    bool mHasGeometry, mOccluded, mVisible;

    LPDIRECT3DDEVICE9 mDevice;
    LPDIRECT3DVERTEXDECLARATION9 mVertDecl;

    LPDIRECT3DQUERY9 mOcclQuery;
    LPDIRECT3DVERTEXDECLARATION9 mVertOcclDecl;
    LPDIRECT3DVERTEXBUFFER9 mOcclGeometry;

    LPDIRECT3DVERTEXBUFFER9 mVertBuffer;
#endif
};
