#pragma once

#include <windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "WinApp.h"
class Input{
public:
	//using namespace Microsoft::WRL の代わり
	template <class T> using ComPtr =  Microsoft::WRL::ComPtr<T>;

	void Initialize(WinApp* winApp);
	void Update();

	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
private:
	ComPtr<IDirectInputDevice8> keyboard;
	ComPtr<IDirectInput8> directInput;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	WinApp* winApp_ = nullptr; //協力関係
};