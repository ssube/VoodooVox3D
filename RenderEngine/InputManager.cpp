
#include "Includes.hpp"
#include "InputManager.hpp"

InputManager::InputManager(void)
{
	HRESULT hrCreate = DirectInput8Create(NULL, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDInput, NULL);
	if ( FAILED(hrCreate) )
	{
		//MessageBox
		return;
	}

	HRESULT hrKey = mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	HRESULT hrMouse = mDInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
}

InputManager::~InputManager(void)
{
	if ( mKeyboard )
	{
		mKeyboard->Release();
	}
	if ( mMouse )
	{
		mMouse->Release();
	}
	if ( mDInput )
	{
		mDInput->Release();
	}
}

void InputManager::Grab()
{
	mKeyboard->Acquire();
	mMouse->Acquire();
}

void InputManager::Drop()
{
	mKeyboard->Unacquire();
	mMouse->Unacquire();
}
