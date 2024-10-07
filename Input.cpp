#include "Input.h"

#include <cassert>
//#include <wrl.h>
//using namespace Microsoft::WRL;

//#define DIRECTINPUT_VERSION 0x0800
//#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


void Input::Initialize(HINSTANCE hInstance, HWND hwnd) {

	HRESULT result;

	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

}

void Input::Update() {
	//keyboard->Acquire();

	//BYTE key[256] = {};
	//keyboard->GetDeviceState(sizeof(key), key);

	//if (key[DIK_0]) {
	//	OutputDebugStringA("hit 0\n");
	//}
}