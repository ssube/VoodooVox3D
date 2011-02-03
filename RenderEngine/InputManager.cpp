
#include "Includes.hpp"
#include "InputManager.hpp"

InputManager::InputManager(HWND hWnd)
	: mHwnd(hWnd)
{
	HRESULT hrCreate = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDInput, NULL);
	if ( FAILED(hrCreate) )
	{
		//MessageBox
		return;
	}

	HRESULT hrKey = mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	if ( FAILED(hrKey) )
	{
		return;
	}

	HRESULT hrMouse = mDInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	if ( FAILED(hrMouse) )
	{
		return;
	}

	mKeyboard->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE|DISCL_FOREGROUND);
	mKeyboard->SetDataFormat(&c_dfDIKeyboard);

	mMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE|DISCL_FOREGROUND);
	mMouse->SetDataFormat(&c_dfDIMouse);

	mKeyboard->Acquire();
	mMouse->Acquire();

	memset(mKeyboardState, 0, 256);
	memset(mLastKeyboardState, 0, 256);
	memset(&mMouseState, 0, sizeof(mMouseState));
	memset(&mLastMouseState, 0, sizeof(mLastMouseState));
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
	if ( mKeyboard )
	{
		mKeyboard->Unacquire();
		mKeyboard->SetCooperativeLevel(mHwnd, DISCL_EXCLUSIVE|DISCL_FOREGROUND);
		mKeyboard->Acquire();
	}

	if ( mMouse )
	{
		mMouse->Unacquire();
		mMouse->SetCooperativeLevel(mHwnd, DISCL_EXCLUSIVE|DISCL_FOREGROUND);
		mMouse->Acquire();
	}
}

void InputManager::Drop()
{
	if ( mKeyboard )
	{
		mKeyboard->Unacquire();
		mKeyboard->SetCooperativeLevel(mHwnd, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
		mKeyboard->Acquire();
	}

	if ( mMouse )
	{
		mMouse->Unacquire();
		mMouse->SetCooperativeLevel(mHwnd, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
		mMouse->Acquire();
	}
}

void InputManager::Poll()
{
	memcpy(mLastKeyboardState, mKeyboardState, 256);
	memcpy(&mLastMouseState, &mMouseState, sizeof(mMouseState));

	if ( mKeyboard )
	{
		//mKeyboard->Poll();
		mKeyboard->GetDeviceState(256, mKeyboardState);
	}

	if ( mMouse )
	{
		//mMouse->Poll();
		mMouse->GetDeviceState(sizeof(mMouseState), &mMouseState);
	}
}

bool InputManager::KeyDown(DWORD key)
{
	return mKeyboardState[key] & 0x80;
}

bool InputManager::KeyPress(DWORD key)
{
	return ( mKeyboardState[key] & 0x80 ) && !( mLastKeyboardState[key] & 0x80 );
}

bool InputManager::MouseDown(DWORD button)
{
	return mMouseState.rgbButtons[button];
}

LONG InputManager::MouseX()
{
	return mMouseState.lX;
}

LONG InputManager::MouseY()
{
	return mMouseState.lY;
}