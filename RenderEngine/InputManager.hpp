
#pragma once

#include "Includes.hpp"

class InputManager
{
public:
	InputManager(HWND hWnd);
	~InputManager(void);

	void Grab();
	void Drop();

	void Poll();

	bool KeyDown(DWORD key);
	bool KeyPress(DWORD key);

	bool MouseDown(DWORD button);

	LONG MouseX();
	LONG MouseY();

private:
	HWND mHwnd;
	LPDIRECTINPUT mDInput;
	LPDIRECTINPUTDEVICE mKeyboard, mMouse;

	DIMOUSESTATE mLastMouseState, mMouseState;
	char mLastKeyboardState[256], mKeyboardState[256];
};
