
#include "Interface_RenderEngine.hpp"

#include "RenderObject.hpp"
#include "RenderEngine.hpp"
#include "Camera.hpp"
#include "InputManager.hpp"

#define IMPORT_WORLDMANAGER
#include "BlockDictionary.hpp"
#include "Block.hpp"
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
//int main(int argc, char * argv[])
{
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"BoxGame", NULL};

    RegisterClassEx(&wc);

    // Create the window
    HWND hWnd = CreateWindow(L"BoxGame", L"Box Game [DirectX Native]", WS_OVERLAPPEDWINDOW, 100, 300, 800, 600, GetDesktopWindow(), NULL, wc.hInstance, NULL);

    AllocConsole();

    engine = new RenderEngine(hWnd);
    
    world = World::Create(engine);

    camera = engine->GetCamera();
    camera->TranslateRaw(fvec3(WORLD_SIZE / 2.0f, WORLD_SIZE + 10.0f, WORLD_SIZE / 2.0f));

    input = new InputManager(hWnd);
    input->Grab();

    // ----------
    ShowWindow(hWnd, nShowCmd);
    //ShowWindow(hWnd, SW_SHOWNORMAL);
    BringWindowToTop(hWnd);
    UpdateWindow(hWnd);

    float inputDelay = 0.0f;
    float updateDelay = 0.0f;

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
            if ( updateDelay > 0.50f )
            {
                world->Update();
                updateDelay = 0.0f;
            } else {
                updateDelay += engine->GetFrameDelta();
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

    fvec3 translate(0, 0, 0);

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

    if ( translate.x != 0 || translate.y != 0 || translate.z != 0 )
    {
        translate = camera->Transform(translate);
        translate = world->UpdatePosition(camera->GetPosition(), translate);
        camera->SetPosition(translate);
        //camera->Translate(translate);
    }

    LONG mX = input->MouseX();
    LONG mY = input->MouseY();

    if ( mX != 0 || mY != 0 )
    {
        camera->Rotate(mX / 10.0f, mY / 10.0f);
    }
}

VOID Cleanup()
{
    engine->DestroyRenderObject(obj);

    delete engine;

    render = false;
}
