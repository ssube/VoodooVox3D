#pragma once

#ifdef RENDERENGINE_INTERFACE
#include "RenderObject.hpp"

class RenderEngine
{
public:
	RenderObject * CreateRenderObject();
	void DestroyRenderObject(RenderObject * object);
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

	RenderObject * CreateRenderObject();
	void DestroyRenderObject(RenderObject * object);

	Camera * GetCamera();
	float GetFrameDelta();

	void Render();

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

	vector<RenderObject*> mRenderObjects;
};
#endif