#include "Table.h"
namespace DeclarativeClasses
{
#pragma region Constructors and destructors
Table::Table(int w, int h) : ControlForm(w,h)
{
}
Table::Table(HANDLER_CONTAINER handlers) : ControlForm(handlers)
{
}
Table::Table(int w, int h, HANDLER_CONTAINER handlers) : ControlForm(w,h, handlers)
{
}
Table::Table(std::function<BOOL(int, int, void*)>& function) : ControlForm(function)
{
}
Table::Table(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function) : ControlForm(handlers, function)
{
}
Table::Table(int w, int h, HANDLER_CONTAINER handles, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(w,h,handles,function)
{
}
Table::Table(Form& other) : ControlForm(other)
{
}
Table::Table(ControlForm& other) : ControlForm(other)
{
}
Table::Table(Table& other) : ControlForm(((ControlForm&)(other)))
{
}
#pragma endregion
void Table::CreateSelf(const WNDCLASSEXW* wClass, const CreateWindowArgs& args)
{
	RegisterClassExW(wClass);

	CreateSelf(args);
}
void Table::CreateSelf(const CreateWindowArgs& args)
{
	_thisWindow =
	CreateWindowExW
	(
		args.dwExStyle, 
		args.lpClassName, 
		args.lpWindowName, 
		args.dwStyle, 
		args.X, args.Y, 
		args.nWidth, args.nHeight, 
		args.hWndParent, 
		args.hMenu, 
		args.hInstance, 
		args.lpParam
	);
	
	HANDLER_CONTAINER* handlers = &Form::GetHandlers();

}

const HWND& Table::GetWindowHandler()
{
	return _thisWindow;
}

void Table::SetWindowHandler(HWND hwnd)
{
	_thisWindow = hwnd;
}


LRESULT Table::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

}




