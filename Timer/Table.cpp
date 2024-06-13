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
	_x = args.X;
	_y = args.Y;
	_width = args.nWidth;
	_height = args.nHeight;

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
	_horScrollBar =
	CreateWindowExW
	(
		0L,
		L"SCROLLBAR",
		L"",
		SBS_HORZ | WS_VISIBLE | WS_CHILD,
		0,args.nHeight-20,
		args.nWidth-20,20,
		_thisWindow,
		NULL,
		args.hInstance,
		this
	);
	_vertScrollBar =
	CreateWindowExW
	(
		0L,
		L"SCROLLBAR",
		L"",
		SBS_VERT | WS_VISIBLE | WS_CHILD,
		args.nWidth-20,0,
		20,args.nHeight-20,
		_thisWindow,
		NULL,
		args.hInstance,
		this
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


void Table::ResizeScrollBars()
{
	RECT hR
	{
		0,_height - 20,
		_width - 20,20
	},
	vR
	{
		_width - 20,0,
		20,_height - 20
	};

	MoveWindow(_horScrollBar, hR.left, hR.top, hR.right, hR.bottom, TRUE);
	MoveWindow(_vertScrollBar, vR.left, vR.top, vR.right, vR.bottom, TRUE);
}


LRESULT Table::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			
			break;
		}
		case WM_VSCROLL:
		{

			break;
		}
		case WM_HSCROLL:
		{

			break;
		}
		case WM_SIZE:
		{
			RECT r{};
			GetClientRect(hWnd, &r);
			SetNewSize(r.right, r.bottom);

			ResizeScrollBars();
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			HDC hdc = BeginPaint(hWnd, &ps);
			if (this)
			{
				RECT r{ _width - 20,_height - 20,_width,_height };
				HBRUSH brush = CreateSolidBrush(RGB(240, 240, 240));

				HGDIOBJ oldBrush = SelectObject(hdc,brush);
				FillRect(hdc, &r, brush);

				DeleteObject(brush);
				SelectObject(hdc, oldBrush);
			}

			EndPaint(hWnd,&ps);
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

}




