#pragma once

#include "Interface_RenderEngine.hpp"

#define IMPORT_COMMON
#include "CommonTypes.hpp"

#ifndef IMPORT_RENDERENGINE
#   include <dinput.h>
#endif

using namespace Common;

class RENDERENGINE_API InputManager
{
public:
#ifndef IMPORT_RENDERENGINE
    InputManager(HWND hWnd);
    ~InputManager(void);
#endif

    void Grab();
    void Drop();

    void Poll();

    bool KeyDown(uint32 key);
    bool KeyPress(uint32 key);

    bool MouseDown(uint32 button);

    long MouseX();
    long MouseY();

#ifndef IMPORT_RENDERENGINE
private:
    HWND mHwnd;
    LPDIRECTINPUT mDInput;
    LPDIRECTINPUTDEVICE mKeyboard, mMouse;

    DIMOUSESTATE mLastMouseState, mMouseState;
    char mLastKeyboardState[256], mKeyboardState[256];
#endif
};
