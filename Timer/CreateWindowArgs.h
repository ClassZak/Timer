#pragma once
#include <Windows.h>
struct CreateWindowArgs
{
	DWORD dwExStyle;
	LPCWSTR lpClassName;
	LPCWSTR lpWindowName;
	DWORD dwStyle;
	int X;
	int Y;
	int nWidth;
	int nHeight;
	HWND hWndParent;
	HMENU hMenu;
	HINSTANCE hInstance;
	LPVOID lpParam;
};
