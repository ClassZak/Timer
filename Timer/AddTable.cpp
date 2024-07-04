#include "AddTable.h"
namespace DeclarativeClasses
{
#pragma region Constructors
	AddTable::AddTable(UINT cols, UINT rows)
		: ATable(cols,rows)
	{
	}
	AddTable::AddTable(UINT cols, UINT rows, int w, int h)
		: ATable(cols, rows,w,h)
	{
	}
	AddTable::AddTable(UINT cols, UINT rows, HANDLER_CONTAINER handlers)
		: ATable(cols, rows,handlers)
	{
	}
	AddTable::AddTable(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers)
		: ATable(cols, rows, w, h,handlers)
	{
	}
	AddTable::AddTable(UINT cols, UINT rows, std::function<BOOL(int, int, void*)>& function)
		: ATable(cols, rows,function)
	{
	}
	AddTable::AddTable(UINT cols, UINT rows, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
		: ATable(cols, rows,handlers,function)
	{
	}
	AddTable::AddTable
	(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
		: ATable(cols, rows,w,h,handlers,function)
	{
	}
	AddTable::AddTable(UINT cols, UINT rows, Form& other)
		: ATable(cols, rows,other)
	{
	}
	AddTable::AddTable(UINT cols, UINT rows, ControlForm& other)
		: ATable(cols, rows,other)
	{
	}
	AddTable::AddTable(UINT cols, UINT rows, ATable& other)
		: ATable(cols, rows,other)
	{
	}




	AddTable::AddTable(UINT cols, UINT rows, AddTable& other)
		: AddTable(cols,rows,(ATable&)other)
	{
	}
#pragma endregion
	LRESULT CALLBACK AddTable::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CREATE:
		{
			break;
		}
		case WM_COMMAND:
		{
			int loword = LOWORD(wParam);
			if (loword >= 0 and loword <= 3)
			{
				auto handleIt = GetHandlers().at("textBoxHeader").begin();
				for (UINT i = 0; i != LOWORD(wParam); ++i, ++handleIt);

				char str[0x100] = "";

				GetWindowTextA(handleIt->second, str, 0xFE);
				MessageBoxA(hWnd, str, "Click", MB_OK);
			}
			else
				_selectedEdit = loword;


			break;
		}
		case WM_KEYDOWN:
		{
			if (wParam == VK_RETURN)
				KillCellsFocus();
			break;
		}
		case WM_SIZE:
		{
			SetNewSize(LOWORD(lParam), HIWORD(lParam));
			try
			{
				_resizeFunction(LOWORD(lParam), HIWORD(lParam), (void*)&Form::GetHandlers());
			}
			catch (const std::exception&)
			{

			}


			SetFocus(hWnd);
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
	void AddTable::CreateSelf(const WNDCLASSEXW* wClass, const CreateWindowArgs& args)
	{
		RegisterClassExW(wClass);

		CreateSelf(args);
	}
	void AddTable::CreateSelf(const CreateWindowArgs& args)
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

		for (int i = 0; i != _columns; ++i)
		{
			int id = i;
			HWND edit =
				CreateWindowExW
				(
					0L,
					L"static",
					NULL,
					WS_CHILD | WS_VISIBLE | SS_CENTER | WS_BORDER | SS_NOTIFY,
					40 * i, 0,
					40, 15,
					_thisWindow,
					(HMENU)id,
					args.hInstance,
					NULL
				);
			LOGFONT lf{};
			lf.lfHeight = 12;
			SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
			lf.lfWeight = FW_BOLD;

			HFONT hFont = CreateFontIndirect(&lf);
			SendMessage(edit, WM_SETFONT, (WPARAM)hFont, TRUE);

			Form::AddItem
			(
				"textBoxHeader",
				std::to_string(id),
				&edit
			);
		}


		for (int i = 0; i != _rows; ++i)
			for (int j = 0; j != _columns; ++j)
			{
				int id = _columns * i + j + _columns;
				HWND edit =
					CreateWindowExW
					(
						0L,
						L"edit",
						(!j) ? std::to_wstring(i + 1).c_str() : L"",
						WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ((!j) ? ES_READONLY : 0ul),
						40 * j, 15 + 15 * i,
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

				SetWindowSubclass(edit, &EditProc, id, (DWORD_PTR)this);
				Form::AddItem
				(
					"textBox",
					std::to_string(id),
					&edit
				);
				cells.insert(std::pair<UINT, HWND>(id, edit));
			}

		Form::SetResizeMethod(DeclarativeClasses::Functions::ResizeFunctions::ResizeAddTable);
		Form::SetNewSize(args.nWidth, args.nHeight);
		_resizeFunction(args.nWidth, args.nHeight, (void*)&Form::GetHandlers());

		isInitilized = true;
	}

}