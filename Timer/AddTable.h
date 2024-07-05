#pragma once
#include "ATable.h"
#include <array>
#include <vector>
#include <string>

namespace DeclarativeClasses
{
class AddTable :
    public ATable
{
private:
	BITMAP MainBitmapBuffer{};
	BITMAP TempBitmapBuffer{};

	std::vector<std::array<std::string, 4u>> tableRows;
	std::array<int, 3> columnsPositions{40,0,0};
	const int ROW_HEIGHT=15;

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

    void CreateSelf(const WNDCLASSEXW* wClass, const CreateWindowArgs& args)override;
    void CreateSelf(const CreateWindowArgs& args)override;

private:
	inline POINT GetSelectedIndex(POINT mousePos);
	inline void ResetColumnsPositions();
	void FillNumbers();
	void DeleteEmptyRows();
};

}

