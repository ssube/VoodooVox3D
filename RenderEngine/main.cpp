
#include "Includes.hpp"
#include "VertexHeader.hpp"
#include "RenderObject.hpp"

LPDIRECT3D9 dxObj;
LPDIRECT3DDEVICE9 dxDevice;
RenderObject * obj = NULL, * obj2 = NULL;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Render();

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
		{0,  0, D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0},
		D3DDECL_END()
	};

	dxDevice->CreateVertexDeclaration(vertElems, &VertexDecl);

	Vertex vertices[] =
	{
		{ 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, },
		{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
		{  50.0f, 250.0f, 0.5f, 1.0f, 0xff0000ff, },
	};

	obj = new RenderObject(dxDevice, VertexDecl);
	obj->SetGeometry(3, vertices);

	vertices[0].x =  50.0f; vertices[0].y = 150.0f; vertices[0].z = 0.6f;
	vertices[1].x =  50.0f; vertices[1].y =  50.0f; vertices[0].z = 0.3f;
	vertices[2].x = 250.0f; vertices[2].y =  50.0f; vertices[2].z = 0.5f;

	obj2 = new RenderObject(dxDevice, VertexDecl);
	obj2->SetGeometry(3, vertices);

	// ----------
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while ( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		Render();
		ValidateRect(hWnd, NULL);
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

	dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(32, 32, 96), 1.0f, 0);	

	dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	dxDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	obj->Render();
	obj2->Render();

	dxDevice->Present(NULL, NULL, NULL, NULL);
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