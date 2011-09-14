#pragma once

#include "Interface_RenderEngine.hpp"

#ifndef IMPORT_RENDERENGINE
#   include <list>

#   include <windows.h>

#   ifdef _DEBUG
#       define D3D_DEBUG_INFO
#   endif
#   include <d3d9.h>
#   include <d3dx9.h>
#   include <d3dx9effect.h>

//  Common includes
#   define IMPORT_COMMON
#   define D3DX_CONVERSION_FUNCS
#   include "CommonTypes.hpp"
#   include "Math.hpp"
#   include "VectorMath.hpp"
#   include "Octree.hpp"
#endif

class RENDERENGINE_API RenderEngine
{
public:
#ifndef IMPORT_RENDERENGINE
    RenderEngine(HWND hWnd);
    ~RenderEngine(void);
#endif

    virtual RenderObject * CreateRenderObject();
    virtual void DestroyRenderObject(RenderObject * object);
    virtual void AddRenderObject(RenderObject * object);
    virtual void RemoveRenderObject(RenderObject * object);

    virtual Camera * GetCamera();

    virtual float GetFrameDelta();

    virtual void Render();

    virtual void CreateOcclusionData();

#ifndef IMPORT_RENDERENGINE
private:
    LPDIRECT3D9 mObject;
    LPDIRECT3DDEVICE9 mDevice;

    DWORD mLastTicks, mTicks;
    float mFrameTime;
    ID3DXFont * mFont;
    RECT mTextRect;
    Camera * mCamera;

    LPDIRECT3DTEXTURE9 mLandTexture;
    LPDIRECT3DVERTEXDECLARATION9 mVertDecl;

    LPDIRECT3DVERTEXDECLARATION9 mVertOcclDecl;
    LPDIRECT3DVERTEXBUFFER9 mOcclGeometry;
    LPDIRECT3DSURFACE9 mOcclusionSurface;
    LPDIRECT3DTEXTURE9 mOcclusionTexture;
    LPD3DXRENDERTOSURFACE mOcclusionRTS;

    LPD3DXEFFECT mDefaultShader;
    D3DXHANDLE mShader_Technique;
    D3DXHANDLE mShader_BaseTexture;
    D3DXHANDLE mShader_MVPMatrix;

    std::list<RenderObject*> mRenderObjects;
    Common::Trees::Octree * mSceneTree;
#endif
};
