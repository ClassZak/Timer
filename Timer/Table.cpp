#include "Table.h"
#include "Functions.h"
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

#pragma endregion
#pragma region Public
Table::Table(UINT cols, UINT rows) : Table()
{
	SetColumnsAndRows(cols, rows);
}


Table::Table(UINT cols, UINT rows, int w, int h) : Table(w, h)
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


inline bool Table::CellIsLeft(UINT& id) const
{
	UINT row = id / _columns;
	UINT column = id - _columns * row;
	return column==1;
}

inline bool Table::CellIsRight(UINT& id) const
{
	UINT row = id / _columns;
	UINT column = id - _columns * row;
	return column==_columns-1;
}

inline bool Table::CellIsTop(UINT& id) const
{
	return (id / _columns)==0;
}

inline bool Table::CellIsBottom(UINT& id)
{
	try
	{
		RECT rect{};


		auto elIt = std::find_if
		(
			Form::GetHandlers().at("textBox").begin(),
			Form::GetHandlers().at("textBox").end(),
			[&id](const std::pair<std::string, HWND>& pair) -> bool
			{
				unsigned int id_ = std::to_unsigned_number(pair.first);

				return id_ == id;
			}
		);
		if (elIt == Form::GetHandlers().at("textBox").end())
			throw;

		GetClientRect(elIt->second, &rect);
		MapWindowPoints(elIt->second, GetParent(elIt->second), (LPPOINT)&rect, 2);

		return rect.bottom >= _height;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

std::pair<UINT, UINT> Table::IdToPair(UINT id) const
{
	UINT row = id / _columns;
	UINT col = id - row * _columns;

	return std::pair<UINT, UINT>(row,col);
}

void Table::IdToPair(UINT id, UINT* row, UINT* col) const
{
	*row = id / _columns;
	*col = id - (*row) * _columns;
}

UINT Table::PairToId(UINT row, UINT col)
{
	return row*_columns+col;
}

void Table::SortByEnteredCell(UINT id, HWND cell)
{
	UINT row, col;
	{
		std::pair<UINT, UINT> coords = IdToPair(id);
		row = coords.first;
		col = coords.second;
	}

	if (!(row-1) or !col)
		return;

	UINT needId =_columns+col+1;


	char buff[0x100] = "";
	GetWindowTextA(cell, buff, 0xFE);

	if (buff[0]=='\0' or buff == "")
		return;

	UINT needRow = 1;
	bool rowIsEmpty = true;

	for (UINT i = row-1; i != 0; --i)
	{
		for (UINT j = 1; j != _columns; ++j)
		{
			UINT needId=PairToId(i,j);
			auto it =  std::find_if
			(
				cells.begin(),
				cells.end(),
				[needId](const std::pair<UINT, HWND>& pair)->bool
				{
					return pair.first == needId;
				}
			);

			if (it == cells.end())
				throw;

			GetWindowTextA(it->second, buff, 0xFE);
			if (buff[0] != '\0' and buff != "")
			{
				rowIsEmpty = false;
				break;
			}
		}
		if (!rowIsEmpty)
		{
			needRow = i + 1;
			break;
		}
	}
	if (needRow == row)
		return;
	

	{
		GetWindowTextA(cell, buff, 0xFE);

		UINT needId=PairToId(needRow, col);
		auto it = std::find_if
		(
			cells.begin(),
			cells.end(),
			[needId](const std::pair<UINT, HWND>& pair)->bool
			{
				return pair.first == needId;
			}
		);
		if (it == cells.end())
			throw;


		SetWindowTextA(it->second, buff);
		SetWindowTextA(cell, "");
	}
}


void Table::ResetFocus(UINT id, Direction direction)
{
	UINT row, col,needId;
	IdToPair(id, &row, &col);


	switch (direction)
	{
		case DeclarativeClasses::Right:
		{
			needId = (col == _columns - 1) ? PairToId(row + 1, 1) : PairToId(row,col+1);
			auto it = std::find_if
			(
				cells.begin(),
				cells.end(),
				[needId](const std::pair<UINT,HWND>& pair)->bool
				{
					return pair.first == needId;
				}
			);

			if (it == cells.end() or CellIsBottom(id) or CellIsBottom(needId))
				return;

			SetFocus(it->second);

			break;
		}
		case DeclarativeClasses::Left:
		{
			needId = (col == 1) ? PairToId(row - 1, _columns-1) : PairToId(row, col - 1);
			auto it = std::find_if
			(
				cells.begin(),
				cells.end(),
				[needId](const std::pair<UINT, HWND>& pair)->bool
				{
					return pair.first == needId;
				}
			);

			if (it == cells.end() or CellIsTop(needId))
				return;

			SetFocus(it->second);

			break;
		}
		case DeclarativeClasses::Up:
		{
			needId = PairToId(row - 1, col);
			auto it = std::find_if
			(
				cells.begin(),
				cells.end(),
				[needId](const std::pair<UINT, HWND>& pair)->bool
				{
					return pair.first == needId;
				}
			);
			if (it == cells.end() or CellIsTop(needId))
				return;

			SetFocus(it->second);

			break;
		}
		case DeclarativeClasses::Down:
		{
			needId = PairToId(row + 1, col);
			auto it = std::find_if
			(
				cells.begin(),
				cells.end(),
				[needId](const std::pair<UINT, HWND>& pair)->bool
				{
					return pair.first == needId;
				}
			);
			if (it == cells.end() or CellIsBottom(needId))
				return;

			SetFocus(it->second);

			break;
		}
		default:
			break;
	}
}

void Table::KillCellsFocus()
{
	SetFocus(_thisWindow);
	_selectedEdit = -1;
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
			_selectedEdit = loword;
		

		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam==VK_RETURN)
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

}



