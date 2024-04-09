#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Circle.h"
#include <math.h>
#include <ctime>
#include <string>
#include <Windows.h>

class Clock : public Circle
{
public:
	Clock();
	Clock(POINT center);
	Clock(float radius);
	Clock(POINT center, float radius);
	Clock(float radius, POINT center);

	Clock(const Clock& other);


	~Clock();


	void Draw(HDC* hdc);
	void StartUpdateThread();
	void SetWindowHandle(HWND* hWnd);
private:
	void InitTime();
protected:
	time_t rawTime;
	tm TimeInfo;
	HWND* hWnd = nullptr;
	BOOL work = FALSE;



	HBRUSH clockBackgroundBrush = CreateSolidBrush(RGB(224, 224, 224));
	HBRUSH backgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
	HPEN clockBorderPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	HPEN hourLabelPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	HPEN minuteLabelPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HPEN secondArrowPen = reinterpret_cast<HPEN> (GetStockObject(BLACK_PEN));
	


	struct UpdateArrowsArgumentsStruct
	{
		time_t* rawTime;
		tm* TimeInfo;
		HWND* hWnd;
		RECT* rect;
		BOOL* work;
	};

	
	static DWORD WINAPI UpdateArrows(void* lParam);
	static const std::string CLOCK_DIGITS[12];
};

