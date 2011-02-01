
#pragma once

#include "Includes.hpp"

class InputManager
{
public:
	InputManager(void);
	~InputManager(void);

	void Grab();
	void Drop();

private:
	LPDIRECTINPUT mDInput;
	LPDIRECTINPUTDEVICEW mKeyboard, mMouse;
};
