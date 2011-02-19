#pragma once

#ifdef RENDERENGINE_INTERFACE
#include "RenderObject.hpp"

class RenderEngine
{
public:
	virtual RenderObject * CreateRenderObject();
	virtual void DestroyRenderObject(RenderObject * object);

	virtual void * GetCamera();

	virtual float GetFrameDelta();

	virtual void Render();

	virtual void CreateOcclusionData();
};
#else
#include "Includes.hpp"
#include "RenderObject.hpp"
#include "Camera.hpp"

using namespace std;

class RenderEngine
{
public:
	RenderEngine(HWND hWnd);
	~RenderEngine(void);

	virtual RenderObject * CreateRenderObject();
	virtual void DestroyRenderObject(RenderObject * object);

	virtual Camera * GetCamera();
	virtual float GetFrameDelta();

	virtual void Render();

	virtual void CreateOcclusionData();

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

	vector<RenderObject*> mRenderObjects;
};
#endif
