
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

	HRESULT hrDev = dxObj->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &dxDevice);
	if ( FAILED(hrDev) )
	{
		MessageBox(NULL, L"Error creating D3D9 device.", L"D3D Error", MB_ICONERROR|MB_OK);
		return E_FAIL;
	}

	D3DVERTEXELEMENT9 vertElems[] = 
	{
		{0,  0, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 16, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
		{0, 28, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 40, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},
		D3DDECL_END()
	};

	dxDevice->CreateVertexDeclaration(vertElems, &VertexDecl);

	Vertex vertices[] =
	{
		{ 1500.0f,   50.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffff0000, },
		{ 2500.0f, 2500.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff00ff00, },
		{   50.0f, 2500.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff0000ff, },
	};

	obj = new RenderObject(dxDevice, VertexDecl);
	obj->SetGeometry(3, vertices);

	// Block/chunk stuff
	BlockDictionary * dict = BlockDictionary::FromFile("blocks.dict");
	Chunk * nChunk = new Chunk(dict);
	Vertex * chunkVerts = nChunk->GetGeometry();
	size_t chunkVertCount = nChunk->GetGeometryCount(); 

	obj2 = new RenderObject(dxDevice, VertexDecl);
	obj2->SetGeometry(chunkVertCount, chunkVerts);

	mCamera = new Camera();
	mInput = new InputManager(hWnd);

	// ----------
	ShowWindow(hWnd, nShowCmd);
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
			Sleep(10);
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
		mInput->Drop();
	} else if ( !render &&
		( msg == WM_SIZE && wParam != SIZE_MINIMIZED ) ||
		( msg == WM_EXITSIZEMOVE ) || ( msg == WM_EXITMENULOOP ) ||
		( msg == WM_ACTIVATEAPP && wParam == TRUE ) )
	{
		render = true;
		mInput->Grab();
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
	/*
	clear();

	foreach ( object in world )
	{
		render(object);
	}

	present();
	*/

	dxDevice->SetTransform(D3DTS_VIEW, mCamera->GetViewMatrix());

	dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(32, 32, 96), 1.0f, 0);	

	dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	dxDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	obj->Render();
	obj2->Render();

	dxDevice->Present(NULL, NULL, NULL, NULL);
}

long lastticks;

void Input()
{
	mInput->Poll();

	if ( mInput->KeyDown(DIK_ESCAPE) )
	{
		mInput->Drop();
		render = false;
		return;
	}

	D3DXVECTOR3 translate(0, 0, 0);

	if ( mInput->KeyDown(DIK_W) )
	{
		translate.z += 2.0f * mFrameTime;
	}
	if ( mInput->KeyDown(DIK_S) )
	{
		translate.z -= 2.0f * mFrameTime;
	}
	if ( mInput->KeyDown(DIK_A) )
	{
		translate.x -= 2.0f * mFrameTime;
	}
	if ( mInput->KeyDown(DIK_D) )
	{
		translate.x += 2.0f * mFrameTime;
	}

	mCamera->Translate(translate);
	mCamera->Rotate(mInput->MouseX(), mInput->MouseY());
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