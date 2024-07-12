#pragma once
#include "ATable.h"
#include "TimerStruct.h"
#include "EditWindowStruct.h"
#include <array>

namespace DeclarativeClasses
{
class TimersTable :
	public ATable
{
private:
	std::vector<TimerStruct> timers;
	std::vector<TimerStruct*> triggeredTimers;
	EditWindowStruct m_editWindow{ NULL,{NULL,NULL},FALSE };

	std::array<int, 2> columnsPositions{ 0,0 };

	const int ROW_HEIGHT = 16;
	const int EDIT_WINDOW = 101;
	const int CONSERVATIVE_WIDTH = 57;
	const std::array<int,3> COLUMNS_WIDTHS = { 16,16,25 };

public:
	TimersTable(UINT cols, UINT rows);
	TimersTable(UINT cols, UINT rows, int w, int h);
	TimersTable(UINT cols, UINT rows, HANDLER_CONTAINER handlers);
	TimersTable(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers);
	TimersTable(UINT cols, UINT rows, std::function<BOOL(int, int, void*)>& function);
	TimersTable(UINT cols, UINT rows, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	TimersTable
	(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	TimersTable(UINT cols, UINT rows, Form& other);
	TimersTable(UINT cols, UINT rows, ControlForm& other);
	TimersTable(UINT cols, UINT rows, ATable& other);

	TimersTable(UINT cols, UINT rows, TimersTable& other);


	LRESULT CALLBACK Proc(HWND, UINT, WPARAM, LPARAM)override;
	LRESULT CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR)override;


	void CreateSelf(const CreateWindowArgs& args)override;

	void AddRow(const std::array<std::string,4>& data);
	void ClearRows();
	void StopTimer(const std::size_t number);
	inline void Draw();
	inline void DrawRow(HDC& hdc,const std::size_t& rowId = 0);
	inline void DrawStopButtons(HDC& hdc);
	inline void DrawPauseButtons(HDC& hdc);
	inline void DrawRowLine(HDC& hdc);
	inline void DrawColumnsLines(HDC& hdc);
	inline void DrawNumbers(HDC& hdc);
	inline void DrawTimersDescriptions(HDC& hdc);
	inline void DrawTimersTime(HDC& hdc);

	inline POINT GetSelectedIndex(POINT mousePos);
	inline RECT GetSelectedCellRect(POINT pos);

	void HideEditWindow();
};
}

