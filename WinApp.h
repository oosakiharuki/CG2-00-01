#include<Windows.h>

#pragma once

class WinApp
{	
public://静寂メンバ関数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://メンバ関数
	void Initialize();
	void Update();


private:
	WNDCLASS wc{};

};