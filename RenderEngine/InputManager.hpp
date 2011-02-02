
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

	LONG MouseX();
	LONG MouseY();

private:
	LPDIRECTINPUT mDInput;
	LPDIRECTINPUTDEVICE mKeyboard, mMouse;

	DIMOUSESTATE mLastMouseState, mMouseState;
	char mLastKeyboardState[256], mKeyboardState[256];
};
