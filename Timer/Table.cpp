#include "Table.h"
namespace DeclarativeClasses
{
#pragma region Constructors and destructors
#pragma region Protected
Table::Table(int w, int h) : ControlForm(w, h)
{
}
Table::Table(HANDLER_CONTAINER handlers) : ControlForm(handlers)
{
}
Table::Table(int w, int h, HANDLER_CONTAINER handlers) : ControlForm(w, h, handlers)
{
}
Table::Table(std::function<BOOL(int, int, void*)>& function) : ControlForm(function)
{
}
Table::Table(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(handlers, function)
{
}
Table::Table(int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(w, h, handlers, function)
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

#pragma region Public
Table::Table(UINT cols, UINT rows) : Table()
{
	SetColumnsAndRows(cols, rows);
}


Table::Table(UINT cols, UINT rows, int w, int h) : ControlForm(w, h)
{
	SetColumnsAndRows(cols, rows);
}
Table::Table(UINT cols, UINT rows, HANDLER_CONTAINER handlers) : ControlForm(handlers)
{
	SetColumnsAndRows(cols, rows);
}
Table::Table(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers) : ControlForm(w, h, handlers)
{
	SetColumnsAndRows(cols, rows);
}
Table::Table(UINT cols, UINT rows, std::function<BOOL(int, int, void*)>& function) : ControlForm(function)
{
	SetColumnsAndRows(cols, rows);
}
Table::Table(UINT cols, UINT rows, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(handlers, function)
{
	SetColumnsAndRows(cols, rows);
}
Table::Table(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(w, h, handlers, function)
{
	SetColumnsAndRows(cols, rows);
}
Table::Table(UINT cols, UINT rows, Form& other) : ControlForm(other)
{
	SetColumnsAndRows(cols, rows);
}
Table::Table(UINT cols, UINT rows, ControlForm& other) : ControlForm(other)
{
	SetColumnsAndRows(cols, rows);
}
Table::Table(UINT cols, UINT rows, Table& other) : ControlForm(((ControlForm&)(other)))
{
	SetColumnsAndRows(cols, rows);
}











#pragma endregion
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
					40 * j, 15+15 * i,
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
				"textBox",
				std::to_string(id),
				&edit
			);
		}

	Form::SetResizeMethod(DeclarativeClasses::Functions::ResizeFunctions::ResizeAddTable);
	Form::SetNewSize(args.nWidth, args.nHeight);
	_resizeFunction(args.nWidth, args.nHeight, (void*)&Form::GetHandlers());

	isInitilized = true;
}

const HWND& Table::GetWindowHandler()
{
	return _thisWindow;
}

void Table::SetWindowHandler(HWND hwnd)
{
	_thisWindow = hwnd;
}

BOOL Table::SetHeaders(const std::vector<std::string>& headers)
{
	if (headers.size() != _columns)
		return EXIT_FAILURE;

	auto itStart = Form::GetHandlers().at("textBoxHeader").begin();
	auto itEnd= Form::GetHandlers().at("textBoxHeader").end();
	auto vectorIt = headers.begin();
	for (auto& it = itStart; it != itEnd; ++it,++vectorIt)
		SetWindowTextA(it->second, vectorIt->c_str());

	return EXIT_SUCCESS;
}

BOOL Table::SetHeaders(const std::list<std::string>& headers)
{
	if(headers.size()!= _columns)
		return EXIT_FAILURE;

	auto itStart = Form::GetHandlers().at("textBoxHeader").begin();
	auto itEnd = Form::GetHandlers().at("textBoxHeader").end();
	auto listIt= headers.begin();
	for (auto& it = itStart; it != itEnd; ++it, ++listIt)
		SetWindowTextA(it->second, listIt->c_str());

	return EXIT_FAILURE;
}

void Table::SetColumnsAndRows(UINT cols, UINT rows)
{
	_columns = cols;
	_rows = rows;
}


LRESULT Table::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			if (((UINT)loword) < _columns * _rows)
			{
				if (selectedEdit != -1 and selectedEdit!=loword)
				{
					if (isInitilized)
					{
						HANDLER_CONTAINER hCont = Form::GetHandlers();

						try
						{
							int editId = 4;

							auto editIt = hCont.at("textBox").begin();
							for (; editId != selectedEdit; ++editId, ++editIt);

							SendMessage(editIt->second, EM_SETSEL, 0, 0);
							selectedEdit = -1;
						}
						catch (const std::exception&)
						{
						}
					}
				}

				selectedEdit = loword;
			}
				

		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam==VK_RETURN)
		{
			if (selectedEdit != -1)
			{
				selectedEdit = -1;

				int editId = 4;
				auto editIt = Form::GetHandlers().at("textBox").begin();
				for (; editId != selectedEdit; ++editId, ++editIt);

				SendMessage(editIt->second, EM_SETSEL, 0, 0);
				selectedEdit = -1;
			}
		}
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
		RECT clientRect{};
		GetClientRect(hWnd, &clientRect);



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



