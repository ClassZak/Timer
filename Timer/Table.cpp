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
Table::Table(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(handlers, function)
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
	HANDLER_CONTAINER* handlers = &Form::GetHandlers();

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

	
	for (int i = 0; i != 15; ++i)
		for (int j = 0; j !=40; ++j)
		{
			int id = 40 * i + j;
			HWND edit =
			CreateWindowExW
			(
				0L,
				L"edit",
				(std::to_wstring(i) + L" " + std::to_wstring(j)).c_str(),
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				40 * i, 15 * j,
				40, 15,
				_thisWindow,
				(HMENU)id,
				args.hInstance,
				NULL
			);
			LOGFONT lf{};
			lf.lfHeight = 12;
			SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
			HFONT hFont = CreateFontIndirect(&lf);
			SendMessage(edit, WM_SETFONT, (WPARAM)hFont, TRUE);


			Form::AddItem
			(
				"textbox",
				std::to_string(i) + " " + std::to_string(j),
				&edit
			);
		}
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
			BOOL r = ShowScrollBar(hWnd, SB_BOTH, TRUE);
			r = SetScrollRange(hWnd, SB_HORZ, this->minHScrollbarPos, this->maxHScrollbarPos, TRUE);
			r = SetScrollRange(hWnd, SB_VERT, this->minVScrollbarPos, this->maxVScrollbarPos, TRUE);

			r = SetScrollPos(hWnd, SB_HORZ, this->hScrollbarPos, TRUE);
			r = SetScrollPos(hWnd, SB_VERT, this->vScrollbarPos, TRUE);
			break;
		}
		case WM_HSCROLL:
		{
			switch (LOWORD(wParam))
			{
				case SB_PAGERIGHT:
				{
					hScrollbarPos += 1;
					break;
				}
				case SB_LINERIGHT:
				{
					hScrollbarPos += 1;
					break;
				}


				case SB_PAGELEFT:
				{
					hScrollbarPos -= 1;
					break;
				}
				case SB_LINELEFT:
				{
					hScrollbarPos -= 1;
					break;
				}


				case SB_BOTTOM:
				{
					hScrollbarPos = maxVScrollbarPos;
					break;
				}
				case SB_TOP:
				{
					hScrollbarPos = minVScrollbarPos;
					break;
				}


				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
				{
					hScrollbarPos = HIWORD(wParam);
					break;
				}
			}

			if (hScrollbarPos > maxHScrollbarPos)
				hScrollbarPos = maxHScrollbarPos;
			if (hScrollbarPos < minHScrollbarPos)
				hScrollbarPos = minHScrollbarPos;
			
			SetScrollPos(hWnd, SB_HORZ, hScrollbarPos, TRUE);

			RECT r{};
			GetClientRect(hWnd, &r);
			InvalidateRect(hWnd, &r, TRUE);
			break;
		}
		case WM_VSCROLL:
		{
			switch (LOWORD(wParam))
			{
				case SB_PAGEUP:
				{
					vScrollbarPos -= 1;
					break;
				}
				case SB_LINEUP:
				{
					vScrollbarPos -= 1;
					break;
				}


				case SB_PAGEDOWN:
				{
					vScrollbarPos += 1;
					break;
				}
				case SB_LINEDOWN:
				{
					vScrollbarPos += 1;
					break;
				}


				case SB_TOP:
				{
					vScrollbarPos = maxVScrollbarPos;
					break;
				}
				case SB_BOTTOM:
				{
					vScrollbarPos = minVScrollbarPos;
					break;
				}


				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
				{
					vScrollbarPos = HIWORD(wParam);
					break;
				}
			}

			if (vScrollbarPos > maxVScrollbarPos)
				vScrollbarPos = maxVScrollbarPos;
			if (vScrollbarPos < minVScrollbarPos)
				vScrollbarPos = minVScrollbarPos;

			SetScrollPos(hWnd, SB_VERT, vScrollbarPos, TRUE);

			RECT r{};
			GetClientRect(hWnd, &r);
			InvalidateRect(hWnd, &r, TRUE);
			break;
		}
		case WM_SIZE:
		{
			RECT clientRect{};
			GetClientRect(hWnd, &clientRect);
			SetNewSize(clientRect.right, clientRect.bottom);

			int newWidth = LOWORD(lParam); // Новая ширина окна
			int newHeight = HIWORD(lParam); // Новая высота окна


			RECT windowRect{};
			GetWindowRect(hWnd, &windowRect);


			maxHScrollbarPos = (windowRect.right + windowRect.left) / clientRect.right;
			maxVScrollbarPos = (windowRect.bottom + windowRect.top) / clientRect.bottom;




			SCROLLINFO si{};
			si.cbSize = sizeof(si);
			si.fMask = SIF_PAGE;
			si.nPage = (windowRect.bottom + windowRect.top) / clientRect.bottom / maxVScrollbarPos; // Установите размер ползунка равным 1/5 высоты окна
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE); // Обновите вертикальный ползунок

			si.nPage = (windowRect.right + windowRect.left) / clientRect.right / maxHScrollbarPos; // Установите размер ползунка равным 1/5 ширины окна
			SetScrollInfo(hWnd, SB_HORZ, &si, TRUE); // Обновите горизонтальный ползунок


			BOOL succes=
			SetScrollRange(hWnd, SB_HORZ, this->minHScrollbarPos, this->maxHScrollbarPos, TRUE);
			succes=SetScrollRange(hWnd, SB_VERT, this->minVScrollbarPos, this->maxVScrollbarPos, TRUE);


			succes = SetScrollPos(hWnd, SB_HORZ, this->hScrollbarPos, TRUE);
			succes = SetScrollPos(hWnd, SB_VERT, this->vScrollbarPos, TRUE);

			InvalidateRect(hWnd, &clientRect, TRUE);
			break;
		}
		case WM_PAINT:
		{
			if (this)
			{
				PAINTSTRUCT ps{};
				HDC hdc = BeginPaint(hWnd, &ps);

				EndPaint(hWnd, &ps);
			}
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
}




