#pragma once

#define LOD_COUNT 3
#define OCCL_THRESHOLD 8

#ifndef RENDERENGINE_INTERFACE
#    include "Includes.hpp"
#endif

#include "CommonTypes.hpp"

using namespace Common;

class RenderObject
{
public:
#ifndef RENDERENGINE_INTERFACE
    RenderObject(LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXDECLARATION9 vertDecl);
    ~RenderObject(void);
#endif

    virtual bool    GetVisible();
    virtual size_t  GetLOD();
    virtual fvec3   GetPosition();
    virtual fmat4x4 GetTransform();

    virtual void    SetVisible(bool visible);
    virtual void    SetLOD(size_t lod);
    virtual void    SetPosition(fvec3 pos);
    virtual void    SetTransform(fmat4x4 transform);

    virtual size_t  GetVertCount(size_t lod);
    virtual size_t  GetVertOffset(size_t lod);
    virtual void    SetGeometry(size_t total, size_t * offsets, size_t * counts, Vertex * verts);

    virtual void    Render();
    virtual void    UpdateOcclusion();


#ifndef RENDERENGINE_INTERFACE
private:
    size_t mLOD;
    size_t mVertCount[LOD_COUNT];
    size_t mVertOffset[LOD_COUNT];

    fvec3 mPosition;
    fmat4x4 mTransform;

    bool mHasGeometry, mOccluded, mVisible;

    LPDIRECT3DDEVICE9 mDevice;
    LPDIRECT3DVERTEXDECLARATION9 mVertDecl;

    LPDIRECT3DQUERY9 mOcclQuery;
    LPDIRECT3DVERTEXDECLARATION9 mVertOcclDecl;
    LPDIRECT3DVERTEXBUFFER9 mOcclGeometry;

    LPDIRECT3DVERTEXBUFFER9 mVertBuffer;
#endif
};
