
#include "Includes.hpp"
#include "InputManager.hpp"

InputManager::InputManager(HWND hWnd)
{
	HRESULT hrCreate = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDInput, NULL);
	if ( FAILED(hrCreate) )
	{
		//MessageBox
		return;
	}

	HRESULT hrKey = mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	HRESULT hrMouse = mDInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);

	mKeyboard->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE|DISCL_FOREGROUND);
	mKeyboard->SetDataFormat(&c_dfDIKeyboard);

	mMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE|DISCL_FOREGROUND);
	mMouse->SetDataFormat(&c_dfDIMouse);

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
	mKeyboard->Acquire();
	mMouse->Acquire();
}

void InputManager::Drop()
{
	mKeyboard->Unacquire();
	mMouse->Unacquire();
}

void InputManager::Poll()
{
	memcpy(mLastKeyboardState, mKeyboardState, 256);
	memcpy(&mLastMouseState, &mMouseState, sizeof(mMouseState));

	//mKeyboard->Poll();
	mKeyboard->GetDeviceState(256, mKeyboardState);

	//mMouse->Poll();
	mMouse->GetDeviceState(sizeof(mMouseState), &mMouseState);
}

bool InputManager::KeyDown(DWORD key)
{
	return mKeyboardState[key] & 0x80;
}

bool InputManager::KeyPress(DWORD key)
{
	return ( mKeyboardState[key] & 0x80 ) && !( mLastKeyboardState[key] & 0x80 );
}

LONG InputManager::MouseX()
{
	return mMouseState.lX;
}

LONG InputManager::MouseY()
{
	return mMouseState.lY;
}