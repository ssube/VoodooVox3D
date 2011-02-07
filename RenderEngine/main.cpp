
#include "Includes.hpp"
#include "VertexHeader.hpp"
#include "RenderObject.hpp"
#include "RenderEngine.hpp"
#include "Camera.hpp"
#include "InputManager.hpp"

#include "BlockDictionary.hpp"
#include "Block.hpp"
#include "Chunk.hpp"
#include "World.hpp"

RenderEngine * engine = NULL;
RenderObject * obj = NULL;
InputManager * input = NULL;
Camera * camera = NULL;
World * world = NULL;

bool render = true;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Input(float delta);
void Cleanup();

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"BoxGame", NULL};

	RegisterClassEx(&wc);

	// Create the window
	HWND hWnd = CreateWindow(L"BoxGame", L"Box Game [DirectX Native]", WS_OVERLAPPEDWINDOW, 100, 300, 640, 480, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	engine = new RenderEngine(hWnd);

	// Block/chunk stuff
	BlockDictionary * dict = BlockDictionary::FromFile("blocks.dict");
	Chunk * mChunk = new Chunk(dict);
	mChunk->GenerateGeometry();

	obj = engine->CreateRenderObject();
	obj->SetGeometry(mChunk->GetGeometryCount(), mChunk->GetGeometry());

	camera = engine->GetCamera();
	input = new InputManager(hWnd);
	input->Grab();

	// ----------
	ShowWindow(hWnd, nShowCmd);
	//ShowWindow(hWnd, SW_SHOWNORMAL)
	BringWindowToTop(hWnd);
	UpdateWindow(hWnd);

	float inputDelay = 0.0f;

	MSG msg;
	while ( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if ( render )
		{
			if ( inputDelay > 0.02f )
			{
				Input(inputDelay);
				inputDelay = 0.0f;
			} else {
				inputDelay += engine->GetFrameDelta();
			}

			engine->Render();
		} else {
			input->Poll();
			if ( input->MouseDown(0) )
			{
				render = true;
				input->Grab();
			} else {
				Sleep(100);
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
		if ( input )
		{
			input->Drop();
		}
	} else if ( !render &&
		( msg == WM_SIZE && wParam != SIZE_MINIMIZED ) ||
		( msg == WM_EXITSIZEMOVE ) || ( msg == WM_EXITMENULOOP ) ||
		( msg == WM_ACTIVATEAPP && wParam == TRUE ) ||
		( msg == WM_SHOWWINDOW && wParam == TRUE ) )
	{
		render = true;
		if ( input )
		{
			input->Grab();
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

void Input(float delta)
{
	input->Poll();

	if ( input->KeyDown(DIK_ESCAPE) )
	{
		input->Drop();
		render = false;
		return;
	}

	float defaultSpeed = 20.0f; // two meters
	float frameTime = delta; //engine->GetFrameDelta();

	D3DXVECTOR3 translate(0, 0, 0);

	if ( input->KeyDown(DIK_W) )
	{
		translate.z += defaultSpeed * frameTime;
	}
	if ( input->KeyDown(DIK_S) )
	{
		translate.z -= defaultSpeed * frameTime;
	}
	if ( input->KeyDown(DIK_A) )
	{
		translate.x -= defaultSpeed * frameTime;
	}
	if ( input->KeyDown(DIK_D) )
	{
		translate.x += defaultSpeed * frameTime;
	}

	camera->Translate(translate);
	camera->Rotate(input->MouseX() / 10.0f, input->MouseY() / 10.0f);
}

VOID Cleanup()
{
	engine->DestroyRenderObject(obj);

	delete engine;

	render = false;
}
