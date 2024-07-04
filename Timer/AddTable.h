#pragma once
#include "ATable.h"
namespace DeclarativeClasses
{
class AddTable :
    public ATable
{
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
};

}

