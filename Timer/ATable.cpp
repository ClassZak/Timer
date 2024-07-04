#include "ATable.h"
#include "Functions.h"
namespace DeclarativeClasses
{
#pragma region Constructors and destructors
#pragma region Protected
ATable::ATable(int w, int h) : ControlForm(w, h)
{
}
ATable::ATable(HANDLER_CONTAINER handlers) : ControlForm(handlers)
{
}
ATable::ATable(int w, int h, HANDLER_CONTAINER handlers) : ControlForm(w, h, handlers)
{
}
ATable::ATable(std::function<BOOL(int, int, void*)>& function) : ControlForm(function)
{
}
ATable::ATable(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(handlers, function)
{
}
ATable::ATable(int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(w, h, handlers, function)
{
}
ATable::ATable(Form& other) : ControlForm(other)
{
}
ATable::ATable(ControlForm& other) : ControlForm(other)
{
}
ATable::ATable(ATable& other) : ControlForm(((ControlForm&)(other)))
{
}

#pragma endregion
#pragma region Public
ATable::ATable(UINT cols, UINT rows) : ATable()
{
	SetColumnsAndRows(cols, rows);
}


ATable::ATable(UINT cols, UINT rows, int w, int h) : ATable(w, h)
{
	SetColumnsAndRows(cols, rows);
}
ATable::ATable(UINT cols, UINT rows, HANDLER_CONTAINER handlers) : ControlForm(handlers)
{
	SetColumnsAndRows(cols, rows);
}
ATable::ATable(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers) : ControlForm(w, h, handlers)
{
	SetColumnsAndRows(cols, rows);
}
ATable::ATable(UINT cols, UINT rows, std::function<BOOL(int, int, void*)>& function) : ControlForm(function)
{
	SetColumnsAndRows(cols, rows);
}
ATable::ATable(UINT cols, UINT rows, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(handlers, function)
{
	SetColumnsAndRows(cols, rows);
}
ATable::ATable(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: ControlForm(w, h, handlers, function)
{
	SetColumnsAndRows(cols, rows);
}
ATable::ATable(UINT cols, UINT rows, Form& other) : ControlForm(other)
{
	SetColumnsAndRows(cols, rows);
}
ATable::ATable(UINT cols, UINT rows, ControlForm& other) : ControlForm(other)
{
	SetColumnsAndRows(cols, rows);
}
ATable::ATable(UINT cols, UINT rows, ATable& other) : ControlForm(((ControlForm&)(other)))
{
	SetColumnsAndRows(cols, rows);
}











#pragma endregion
#pragma endregion
const HWND& ATable::GetWindowHandler()
{
	return _thisWindow;
}

void ATable::SetWindowHandler(HWND hwnd)
{
	_thisWindow = hwnd;
}

BOOL ATable::SetHeaders(const std::vector<std::string>& headers)
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

BOOL ATable::SetHeaders(const std::list<std::string>& headers)
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

void ATable::SetColumnsAndRows(UINT cols, UINT rows)
{
	_columns = cols;
	_rows = rows;
}


inline bool ATable::CellIsLeft(UINT& id) const
{
	UINT row = id / _columns;
	UINT column = id - _columns * row;
	return column==1;
}

inline bool ATable::CellIsRight(UINT& id) const
{
	UINT row = id / _columns;
	UINT column = id - _columns * row;
	return column==_columns-1;
}

inline bool ATable::CellIsTop(UINT& id) const
{
	return (id / _columns)==0;
}

inline bool ATable::CellIsBottom(UINT& id)
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

std::pair<UINT, UINT> ATable::IdToPair(UINT id) const
{
	UINT row = id / _columns;
	UINT col = id - row * _columns;

	return std::pair<UINT, UINT>(row,col);
}

void ATable::IdToPair(UINT id, UINT* row, UINT* col) const
{
	*row = id / _columns;
	*col = id - (*row) * _columns;
}

UINT ATable::PairToId(UINT row, UINT col) const
{
	return row*_columns+col;
}

void ATable::SortByEnteredCell(UINT id, HWND cell)
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


void ATable::ResetFocus(UINT id, Direction direction)
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

void ATable::KillCellsFocus()
{
	SetFocus(_thisWindow);
	_selectedEdit = -1;
}

}



