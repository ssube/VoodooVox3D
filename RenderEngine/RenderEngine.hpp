#pragma once

#ifndef RENDERENGINE_INTERFACE
#    include "Includes.hpp"
#endif

#include "Camera.hpp"
#include "RenderObject.hpp"

class RenderEngine
{
public:
#ifndef RENDERENGINE_INTERFACE
    RenderEngine(HWND hWnd);
    ~RenderEngine(void);
#endif

    virtual RenderObject * CreateRenderObject();
    virtual void DestroyRenderObject(RenderObject * object);

    virtual Camera * GetCamera();

    virtual float GetFrameDelta();

    virtual void Render();

    virtual void CreateOcclusionData();

#ifndef RENDERENGINE_INTERFACE
private:
    LPDIRECT3D9 mObject;
    LPDIRECT3DDEVICE9 mDevice;

    DWORD mLastTicks, mTicks;
    float mFrameTime;
    ID3DXFont * mFont;
    RECT mTextRect;
    Camera * mCamera;

    D3DXMATRIX mProj;

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
#endif
};
