
#include "Includes.hpp"
#include "VertexHeader.hpp"
#include "RenderObject.hpp"
#include "Camera.hpp"
#include "InputManager.hpp"

#include "BlockDictionary.hpp"
#include "Chunk.hpp"

LPDIRECT3D9 dxObj;
LPDIRECT3DDEVICE9 dxDevice;
RenderObject * obj = NULL, * obj2 = NULL;
bool render = true;
Camera * mCamera;
InputManager * mInput;
DWORD mLastTicks, mTicks;
float mFrameTime;
D3DXMATRIX mProj;
ID3DXFont * font;
RECT rect;
char msg[64];
LPDIRECT3DVOLUMETEXTURE9 dxTexture;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Render();
void Input();
void Cleanup();

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"BoxGame", NULL};

	RegisterClassEx(&wc);

	// Create the window
	HWND hWnd = CreateWindow(L"BoxGame", L"Box Game [DirectX Native]", WS_OVERLAPPEDWINDOW, 100, 300, 640, 480, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	dxObj = Direct3DCreate9(D3D_SDK_VERSION);
	if ( !dxObj )
	{
		MessageBox(NULL, L"Error creating D3D9 object.", L"D3D Error", MB_ICONERROR|MB_OK);
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.Windowed = TRUE;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferCount = 1;
	pp.BackBufferWidth = 640;
	pp.BackBufferHeight = 480;
	pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	pp.EnableAutoDepthStencil = TRUE;
	pp.hDeviceWindow = hWnd;

	HRESULT hrDev = dxObj->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &dxDevice);
	if ( FAILED(hrDev) )
	{
		MessageBox(NULL, L"Error creating D3D9 device.", L"D3D Error", MB_ICONERROR|MB_OK);
		return E_FAIL;
	}

	D3DVERTEXELEMENT9 vertElems[] = 
	{
		{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
		{0, 24, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 36, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},
		D3DDECL_END()
	};

	dxDevice->CreateVertexDeclaration(vertElems, &VertexDecl);

	/*
	Vertex vertices[] =
	{
		{ 150.0f,  50.0f, 1.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffff0000, },
		{ 250.0f, 250.0f, 1.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff00ff00, },
		{  50.0f, 200.0f, 1.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff0000ff, },
	};

	obj = new RenderObject(dxDevice, VertexDecl);
	obj->SetGeometry(3, vertices);
	*/

	// Block/chunk stuff
	BlockDictionary * dict = BlockDictionary::FromFile("blocks.dict");
	Chunk * mChunk = new Chunk(dict);
	mChunk->GenerateGeometry();

	obj2 = new RenderObject(dxDevice, VertexDecl);
	obj2->SetGeometry(mChunk->GetGeometryCount(), mChunk->GetGeometry());

	mCamera = new Camera();
	mInput = new InputManager(hWnd);
	mInput->Grab();

	D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(90.0f), 640.0f/480.0f, 1.0f, 1000.0f);
	dxDevice->SetTransform(D3DTS_PROJECTION, &mProj);

	D3DXCreateFontA(dxDevice, 12, 8, 1, 0, FALSE, NULL, NULL, NULL, NULL, "Arial", &font);
	LONG left = 5;//rand() % 200;
	LONG top = 5;//rand() % 150;
	rect.left = left; rect.right = left + 250;
	rect.top = top; rect.bottom = top + 150;

	HRESULT hrTex = D3DXCreateVolumeTextureFromFile(dxDevice, L"texture.dds", &dxTexture);

	// ----------
	ShowWindow(hWnd, nShowCmd);
	//ShowWindow(hWnd, SW_SHOWNORMAL)
	BringWindowToTop(hWnd);
	UpdateWindow(hWnd);

	MSG msg;
	while ( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		mLastTicks = mTicks;
		mTicks = GetTickCount();
		DWORD diff = (mTicks>mLastTicks) ? mTicks - mLastTicks : mTicks;
		mFrameTime = (float)(diff) / 1000.0f;

		if ( render )
		{
			Input();
			Render();
		} else {
			mInput->Poll();
			if ( mInput->MouseDown(0) )
			{
				render = true;
				mInput->Grab();
			} else {
				Sleep(25);
			}
		}
	}

	return 0;
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if ( render && 
		( msg == WM_SIZE && wParam == SIZE_MINIMIZED ) ||
		( msg == WM_ENTERSIZEMOVE ) || ( msg == WM_ENTERMENULOOP ) )
	{
		render = false;
		if ( mInput )
		{
			mInput->Drop();
		}
	} else if ( !render &&
		( msg == WM_SIZE && wParam != SIZE_MINIMIZED ) ||
		( msg == WM_EXITSIZEMOVE ) || ( msg == WM_EXITMENULOOP ) ||
		( msg == WM_ACTIVATEAPP && wParam == TRUE ) ||
		( msg == WM_SHOWWINDOW && wParam == TRUE ) )
	{
		render = true;
		if ( mInput )
		{
			mInput->Grab();
		}
	}

	switch ( msg )
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		//ValidateRect(hWnd, NULL);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Render()
{
	dxDevice->SetTransform(D3DTS_VIEW, mCamera->GetViewMatrix());

	dxDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(32, 32, 96), 1.0f, 0);	

	dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	dxDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	dxDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	dxDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	dxDevice->SetTexture(0, dxTexture);

	//obj->Render();
	obj2->Render();

	sprintf(msg, "FPS: %2.4f", 1.0f / mFrameTime);

	dxDevice->BeginScene();
	font->DrawTextA(NULL, msg, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
	dxDevice->EndScene();

	dxDevice->Present(NULL, NULL, NULL, NULL);
}

void Input()
{
	mInput->Poll();

	if ( mInput->KeyDown(DIK_ESCAPE) )
	{
		mInput->Drop();
		render = false;
		return;
	}

	float defaultSpeed = 20.0f; // two meters

	D3DXVECTOR3 translate(0, 0, 0);

	if ( mInput->KeyDown(DIK_W) )
	{
		translate.z += defaultSpeed * mFrameTime;
	}
	if ( mInput->KeyDown(DIK_S) )
	{
		translate.z -= defaultSpeed * mFrameTime;
	}
	if ( mInput->KeyDown(DIK_A) )
	{
		translate.x -= defaultSpeed * mFrameTime;
	}
	if ( mInput->KeyDown(DIK_D) )
	{
		translate.x += defaultSpeed * mFrameTime;
	}

	mCamera->Translate(translate);
	mCamera->Rotate(mInput->MouseX() / 10.0f, mInput->MouseY() / 10.0f);
}

VOID Cleanup()
{
	if ( dxDevice )
	{
		dxDevice->Release();
	}
	if ( dxObj )
	{
		dxObj->Release();
	}
}