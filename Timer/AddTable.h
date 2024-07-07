#pragma once
#include "ATable.h"
#include "EditWindowStruct.h"
#include <array>
#include <vector>
#include <string>

#define EDIT_WINDOW 101

namespace DeclarativeClasses
{
class AddTable :
	public ATable
{
private:
	BITMAP MainBitmapBuffer{};
	BITMAP TempBitmapBuffer{};

	std::vector<std::array<std::string, 4u>> tableRows;
	EditWindowStruct m_editWindow{ NULL,{NULL,NULL},FALSE };
	std::array<int, 3> columnsPositions{25,0,0};
	const int ROW_HEIGHT=15;
	const int FIRST_COLUMN_WIDTH=25;

public:
	AddTable(UINT cols, UINT rows);
	AddTable(UINT cols, UINT rows, int w, int h);
	AddTable(UINT cols, UINT rows, HANDLER_CONTAINER handlers);
	AddTable(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers);
	AddTable(UINT cols, UINT rows, std::function<BOOL(int, int, void*)>& function);
	AddTable(UINT cols, UINT rows, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	AddTable
	(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	AddTable(UINT cols, UINT rows, Form& other);
	AddTable(UINT cols, UINT rows, ControlForm& other);
	AddTable(UINT cols, UINT rows, ATable& other);

	AddTable(UINT cols, UINT rows, AddTable& other);



	LRESULT CALLBACK Proc (HWND, UINT, WPARAM, LPARAM)override;
	LRESULT CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR)override;

	void CreateSelf(const WNDCLASSEXW* wClass, const CreateWindowArgs& args)override;
	void CreateSelf(const CreateWindowArgs& args)override;
	void ResetFocus();

	inline void Draw();

private:
	inline POINT GetSelectedIndex(POINT mousePos);
	inline RECT GetSelectedCellRect(POINT pos);
	inline void ResetColumnsPositions();
	inline void DrawCellsData(HDC& hdc);
	void FillNumbers();
	void DeleteEmptyRows();

	int GetFirstEmptyRow(int limitRow=0xFFFF);
	inline void InsertString();
	void InputString(const std::string& string);

	bool RowIsEmpty(const std::size_t i);
	void SortCells();
};

}

