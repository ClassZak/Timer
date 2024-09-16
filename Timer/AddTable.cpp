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
	void AddTable::CreateSelf(const CreateWindowArgs& args)
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

		ResetColumnsPositions();


		std::array<std::string, 4> header =
		{
			"№",
			"Имя",
			"Описание",
			"Время"
		};
		tableRows.push_back(header);
		FillNumbers();


		isInitilized = true;
	}

#pragma region Indexes
	inline POINT AddTable::GetSelectedIndex(POINT mousePos)
	{
		return
		{
			mousePos.x<= columnsPositions[0] ? 0 :
			(mousePos.x<= columnsPositions[1] ? 1 :
			(mousePos.x<=columnsPositions[2] ? 2 : 3)),
			mousePos.y / ROW_HEIGHT
		};
	}

	inline RECT AddTable::GetSelectedCellRect(POINT pos)
	{
		RECT result{};
		result.left = (pos.x) ? columnsPositions[static_cast<std::size_t>(pos.x)-1] : 0;
		result.right = (pos.x == 3) ? _width : columnsPositions[static_cast<std::size_t>(pos.x)];
		result.top = ROW_HEIGHT * pos.y;
		result.bottom = result.top + ROW_HEIGHT;

		return result;
	}

	inline void AddTable::ResetColumnsPositions()
	{
		columnsPositions[0] = FIRST_COLUMN_WIDTH;
		columnsPositions[1] = (int)trunc((_width - FIRST_COLUMN_WIDTH) / 3.f) + FIRST_COLUMN_WIDTH;
		columnsPositions[2] = ((int)trunc((_width - FIRST_COLUMN_WIDTH) / 3.f) << 1) + FIRST_COLUMN_WIDTH;
	}
#pragma endregion
	inline void AddTable::DrawCellsData(HDC& hdc)
	{
		RECT r{};

		LOGFONT lf{};
		lf.lfHeight = 12;
		SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
		lf.lfWeight = FW_NORMAL;
		UINT prevAlign = SetTextAlign(hdc, TA_LEFT);

		

		HFONT font=CreateFontIndirect(&lf);
		HGDIOBJ oldFont=SelectObject(hdc,font);

		for (std::size_t i = 1; i != tableRows.size(); ++i)
		{
			for (std::size_t j = 1; j != 4; ++j)
			{
				r.left = columnsPositions[j - 1]+5;
				r.right = ((j != 3) ? columnsPositions[j] : _width);

				r.top = static_cast<LONG>(ROW_HEIGHT * i);
				r.bottom = r.top + ROW_HEIGHT;

				DrawTextA(hdc, tableRows[i][j].c_str(), (int)tableRows[i][j].length(), &r, NULL);
			}
		}

		DeleteObject(font);
		SelectObject(hdc, oldFont);
		SetTextAlign(hdc, prevAlign);
	}
#pragma region Row filling
	void AddTable::FillNumbers()
	{
		std::array<std::string, 4> arr;

		for
		(
			int h = (int)(ROW_HEIGHT * tableRows.size()),i = (int)tableRows.size(),j = (int)tableRows.size();
			h <= _height;
			h += ROW_HEIGHT,++i,++j
		)
		{
			if (i < tableRows.size())
				continue;

			while (selectedNumbers.find(j) != selectedNumbers.end())
				++j;
			
			for(std::size_t l=0; l!= tableRows.size(); ++l)
			{
				for (std::size_t k = 0; k != tableRows.size(); ++k)
				{
					if (std::to_unsigned_number(tableRows[k][0]) == j)
					{
						++j;
						break;
					}
				}
			}

			arr[0] = std::to_string(j);
			arr[1] = arr[2] = arr[3] = "";

			tableRows.push_back(arr);
		}
	}

	void AddTable::DeleteEmptyRows()
	{
		for (size_t i = tableRows.size()-1; i >=0 and tableRows.size(); --i)
		{
			if
			(
				RowIsEmpty(i) and
				(i*ROW_HEIGHT)>_height
			)
				tableRows.pop_back();
			else
				break;
		}
	}
	int AddTable::GetFirstEmptyRow(int limitRow)
	{
		for (int i = 0; i != tableRows.size() and i != limitRow; ++i)
			if (RowIsEmpty(i))
				return i;

		return -1;
	}
#pragma endregion
	void AddTable::ResetFocus()
	{
		if (this->m_editWindow.editWindow)
			MoveWindow(this->m_editWindow.editWindow, 0, -40, 0, 0, TRUE);

		if(m_editWindow.isEnable)
		if (m_editWindow.editWindow)
		{
			InsertString();
			m_editWindow.isEnable = FALSE;
		}
		SortCells();

		InvalidateRect(_thisWindow, NULL, TRUE);
		SetFocus(_thisWindow);
	}

	void AddTable::InputString(const std::string& string)
	{
		if (!m_editWindow.isEnable)
			return;

		int row = GetFirstEmptyRow(m_editWindow.position.y);
		if (row == -1)
			row = m_editWindow.position.y;

		if (row < tableRows.size())
			tableRows[row][m_editWindow.position.x] = string;
	}

	bool AddTable::RowIsEmpty(const std::size_t i)
	{
		return tableRows[i][1]=="" and tableRows[i][2] == "" and tableRows[i][3] == "";
	}

	void AddTable::MoveEditWindow(POINT& newPosition)
	{
		if (m_editWindow.position.x and m_editWindow.position.y)
		{
			if (m_editWindow.editWindow)
			{
				InsertString();
				SortCells();
			}
		}
		m_editWindow.isEnable = TRUE;

		m_editWindow.position = newPosition;
		RECT r = GetSelectedCellRect(newPosition);
		r.right -= r.left;
		r.bottom -= r.top;


		if (!m_editWindow.editWindow)
		{
			HWND edit = CreateWindowExA
			(
				0L,
				"edit",
				tableRows[newPosition.y][newPosition.x].c_str(),
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN,
				r.left,
				r.top,
				r.right,
				r.bottom,
				_thisWindow,
				(HMENU)EDIT_WINDOW,
				GetModuleHandle(NULL),
				NULL
			);
			m_editWindow.editWindow = edit;


			LOGFONT editLF{};
			editLF.lfHeight = 12;
			SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &editLF, 0);
			editLF.lfWeight = !newPosition.y ? FW_BOLD : FW_NORMAL;

			HFONT hFont = CreateFontIndirect(&editLF);

			SendMessageA(edit, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		else
		{
			MoveWindow
			(
				m_editWindow.editWindow,
				r.left, r.top,
				r.right, r.bottom,
				FALSE
			);
			SetWindowTextA(m_editWindow.editWindow, tableRows[newPosition.y][newPosition.x].c_str());

			LOGFONT editLF{};
			editLF.lfHeight = 12;
			SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &editLF, 0);
			editLF.lfWeight = !newPosition.y ? FW_BOLD : FW_NORMAL;

			HFONT hFont = CreateFontIndirect(&editLF);

			SendMessageA(m_editWindow.editWindow, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		if (!newPosition.y or !newPosition.x)
		{
			SendMessage(m_editWindow.editWindow, EM_SETREADONLY, TRUE, 0);

			LONG_PTR style = GetWindowLongPtrA(m_editWindow.editWindow, GWL_STYLE); // Get current style

			if (!newPosition.y)
			{
				style &= ~ES_LEFT; // Delete ES_LEFT
				style |= ES_CENTER;// Add header style

				SetWindowLongPtrA(m_editWindow.editWindow, GWL_STYLE, style);
			}
		}
		else
		{
			SendMessage(m_editWindow.editWindow, EM_SETREADONLY, FALSE, 0);

			LONG_PTR style = GetWindowLongPtrA(m_editWindow.editWindow, GWL_STYLE); // Get current style


			style &= ~ES_CENTER; // Delete header style
			style |= ES_LEFT; // Add ES_LEFT

			SetWindowLongPtrA(m_editWindow.editWindow, GWL_STYLE, style);
		}

		SortCells();
		SetFocus(m_editWindow.editWindow);

		this->m_editWindow.position = newPosition;
		InvalidateRect(_thisWindow, NULL, TRUE);

		SendMessageA
		(
			m_editWindow.editWindow,
			EM_SETSEL,
			GetWindowTextLengthA(m_editWindow.editWindow),
			GetWindowTextLengthA(m_editWindow.editWindow)
		);
	}

	inline bool AddTable::CellIsLeft(POINT& id)
	{
		return id.x == 1;
	}
	inline bool AddTable::CellIsRight(POINT& id)
	{
		return id.x == 3;
	}
	inline bool AddTable::CellIsTop(POINT& id)
	{
		return id.y == 1;
	}
	inline bool AddTable::CellIsBottom(POINT& id)
	{
		return id.y * ROW_HEIGHT + ROW_HEIGHT >= _height;
	}

	void AddTable::MoveEditWindowTo(POINT& newPosition, DeclarativeClasses::Direction direction)
	{
		POINT pos = newPosition;

		if (!pos.x or !pos.y)
		{
			pos = { 1,1 };
			MoveEditWindow(pos);
			return;
		}

		switch (direction)
		{
			case DeclarativeClasses::Left:
			{
				bool isLeft = CellIsLeft(pos), isTop=CellIsTop(pos);
				if (isLeft and isTop)
					return;
				else if (isLeft)
				{
					pos.x = 3;
					--pos.y;

					MoveEditWindow(pos);
					break;
				}

				--pos.x;
				MoveEditWindow(pos);
				break;
			}
			case DeclarativeClasses::Up:
			{
				bool isLeft = CellIsLeft(pos), isTop = CellIsTop(pos);
				if (isLeft and isTop)
					return;
				else if (isTop)
				{
					pos = { 1,1 };
					MoveEditWindow(pos);
					break;
				}

				--pos.y;
				MoveEditWindow(pos);
				break;
			}
			case DeclarativeClasses::Right:
			{
				bool isRight = CellIsRight(pos), isBottom = CellIsBottom(pos);
				if (isRight and isBottom)
					return;
				else if (isRight)
				{
					pos.x = 1;
					++pos.y;

					MoveEditWindow(pos);
					break;
				}

				++pos.x;
				MoveEditWindow(pos);
				break;
			}
			case DeclarativeClasses::Down:
			{
				bool isRight = CellIsRight(pos), isBottom = CellIsBottom(pos);
				if (isRight and isBottom)
					return;
				else if (isBottom)
				{
					pos.x = 3;
					MoveEditWindow(pos);
					break;
				}

				++pos.y;
				MoveEditWindow(pos);
				break;
			}
			default:
				break;
		}
	}

	void AddTable::SortCells()
	{
		// 3 - column with time 
		// 2 - column with description
		// 1 - column with name

		for (std::size_t i = 1; i != tableRows.size(); ++i)
		{
			if (!tableRows[i][1].length() and !tableRows[i][2].length())
				tableRows[i][3] = "";

			if (RowIsEmpty(i))
				tableRows.erase(tableRows.begin() + i--);
		}

		for (std::size_t i = 1; i != tableRows.size(); ++i)
		{
			if(!tableRows[i][1].length() and !tableRows[i][2].length())
				continue;

			if (tableRows[i][3] == "")
				tableRows[i][3] = "00:00:00";
			else
			{
				for (std::size_t j = 0; j != tableRows[i][3].length(); ++j)
				{
					if (tableRows[i][3][j] == '.')
						tableRows[i][3][j] = ':';
				}



				for (std::size_t j=0;j!= tableRows[i][3].length();++j)
				{
					if (!(tableRows[i][3][j] >= '0' and tableRows[i][3][j] <= '9' or tableRows[i][3][j] == ':'))
						tableRows[i][3].erase(tableRows[i][3].begin()+j--);
				}

				bool haveSeparators = tableRows[i][3].find(':') != -1;

				if (tableRows[i][3].length() == 8)
				{
					if (tableRows[i][3][2] == ':' and tableRows[i][3][5] == ':')
					{
						unsigned long number;

						if (tableRows[i][3][0] != '0')
						{
							number = std::to_unsigned_number(tableRows[i][3].substr(0, 2));
							if (number > 23)
								tableRows[i][3] = std::to_string(23) + tableRows[i][3].substr(2);
						}
						if (tableRows[i][3][3] != '0')
						{
							number = std::to_unsigned_number(tableRows[i][3].substr(3, 2));
							if (number > 59)
							{
								tableRows[i][3].erase(3ui64, 2ui64);
								tableRows[i][3].insert(3ui64, std::to_string(59));
							}
						}
						if (tableRows[i][3][6] != '0')
						{
							number = std::to_unsigned_number(tableRows[i][3].substr(6));
							if (number > 59)
							{
								tableRows[i][3].erase(6ui64, 2ui64);
								tableRows[i][3] += std::to_string(59);
							}
						}


						continue;
					}
				}
				if (haveSeparators)
				{
					for (std::size_t j = 0; j != tableRows[i][3].length(); ++j)
					{
						if (tableRows[i][3][j] == ':')
							tableRows[i][3].erase(j--);
					}

					if (tableRows[i][3].length() > 6)
						tableRows[i][3] = tableRows[i][3].substr(0, 6);
					else
						if (tableRows[i][3].length() < 6)
							while (tableRows[i][3].length() < 6)
								tableRows[i][3] = "0" + tableRows[i][3];

					tableRows[i][3].insert(2ui64, ":");
					tableRows[i][3].insert(5ui64, ":");
				}
				else
				{
					//tm timeStr{};

					for (std::size_t j = 0; j != tableRows[i][3].length(); ++j)
						if (tableRows[i][3][j] == '0')
							tableRows[i][3].erase(j--,1);
						else
							break;

					if (tableRows[i][3].length() == 0)
					{
						tableRows[i][3] = "00:00:00";
						continue;
					}

					tm timeStr = NumberToTime(std::to_unsigned_number(tableRows[i][3]));

					tableRows[i][3] =
						((timeStr.tm_hour > 9) ? std::to_string(timeStr.tm_hour) :
							(std::string("0") + std::to_string(timeStr.tm_hour))) + ":" +
						((timeStr.tm_min > 9) ? std::to_string(timeStr.tm_min) :
							(std::string("0") + std::to_string(timeStr.tm_min))) + ":" +
						((timeStr.tm_sec > 9) ? std::to_string(timeStr.tm_sec) :
							(std::string("0") + std::to_string(timeStr.tm_sec)));
				}
			}
		}





		for (std::size_t i = 1,j=1; i != tableRows.size(); ++i,++j)
		{
			while (selectedNumbers.find(j)!= selectedNumbers.end())
				++j;

			tableRows[i][0] = std::to_string(j);
		}
		for (std::size_t i = 1; i != tableRows.size(); ++i)
			if (!RowIsEmpty(i))
				if (tableRows[i][1] == "")
				{
					unsigned long long end = 0xFFFFFFFFFFFFFFFF - selectedNumbers.size();
					for (unsigned long long j = 1; j != end; ++j)
					{
						std::string chekingName= std::string("Таймер") + std::to_string(j);
						bool tableHasName = false;
						for (std::size_t k = 1; k < tableRows.size(); ++k)
						{
							if(tableRows[k][1] == chekingName)
							{
								tableHasName = true;
								break;
							}


							if
							(
								selectedNumbers.find(j) !=
								selectedNumbers.end()
							)
							{
								tableHasName = true;
								break;
							}
						}

						if (!tableHasName)
						{
							tableRows[i][1] = chekingName;
							break;
						}
					}
				}

		FillNumbers();
	}



	inline void AddTable::InsertString()
	{
		char buff[0xFF]{};
		GetWindowTextA(m_editWindow.editWindow, buff, sizeof(buff));

		InputString(std::string(buff));
	}
#pragma region Proccesses
	LRESULT CALLBACK AddTable::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CREATE:
		{
			HWND edit = CreateWindowExA
			(
				0L,
				"edit",
				"",
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN,
				0,
				-40,
				0,
				0,
				_thisWindow,
				(HMENU)EDIT_WINDOW,
				GetModuleHandleA(NULL),
				NULL
			);
			m_editWindow.editWindow = edit;

			SetWindowSubclass(edit, &ATable::EditProcStatic<AddTable>, EDIT_WINDOW, (DWORD_PTR)this);
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
		case WM_LBUTTONDOWN:
		{
			POINT el = GetSelectedIndex({ LOWORD(lParam) , HIWORD(lParam) });
			MoveEditWindow(el);
			SendMessageA
			(
				m_editWindow.editWindow,
				EM_SETSEL,
				GetWindowTextLengthA(m_editWindow.editWindow),
				GetWindowTextLengthA(m_editWindow.editWindow)
			);
			SortCells();
			break;
		}
		case WM_SIZE:
		{
			if(LOWORD(lParam) and HIWORD(lParam))
			SetNewSize(LOWORD(lParam), HIWORD(lParam));

			ResetColumnsPositions();
			if (isInitilized)
			{
				FillNumbers();
				DeleteEmptyRows();
			}
			SetFocus(hWnd);
			break;
		}
		case WM_PAINT:
		{
			Draw();
			break;
		}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT AddTable::EditProc
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		AddTable* tablePtr = reinterpret_cast<AddTable*>(dwRefData);

		if (msg == WM_KEYDOWN)
		{
			if
			(
				wParam == VK_RETURN or
				wParam == VK_TAB or
				wParam >= VK_LEFT and wParam <= VK_DOWN
			)
			{
				if (wParam == VK_TAB)
				{
					if((GetKeyState(VK_CONTROL) & 0x800))
					{
						ResetFocus();
						SendMessageA(GetParent(_thisWindow), WM_SETFOCUS, NULL, 1);
					}
					else
						MoveEditWindowTo(this->m_editWindow.position, Direction::Right);
				}
				if (wParam == VK_RETURN)
				{
					if (GetKeyState(VK_CONTROL) & 0x8000)
					{
						ResetFocus();
						SendMessageA(GetParent(_thisWindow),WM_SETFOCUS,NULL,1);
					}
					else
						MoveEditWindowTo(this->m_editWindow.position, Direction::Down);
				}
				if (wParam >= VK_LEFT and wParam <= VK_DOWN)
					if ((GetKeyState(VK_SHIFT) & 0x8000) or (GetKeyState(VK_CONTROL) & 0x8000))
						MoveEditWindowTo(this->m_editWindow.position, (Direction)(wParam - VK_LEFT + 1));
			}
		}

		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}


	inline void AddTable::Draw()
	{
		if (this)
		{
			PAINTSTRUCT ps{};
			HDC hdc = BeginPaint(_thisWindow, &ps);


			HGDIOBJ oldPen, oldBrush;
			HPEN gridPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
			HBRUSH headersBrush = CreateSolidBrush(RGB(220, 220, 220));
			oldPen = SelectObject(hdc, gridPen);
			oldBrush = SelectObject(hdc, headersBrush);

			// Header background
			RECT rect{ 1,1,_width,ROW_HEIGHT };
			FillRect(hdc, &rect, headersBrush);
			rect = { 0,ROW_HEIGHT,FIRST_COLUMN_WIDTH,_height };
			FillRect(hdc, &rect, headersBrush);

			{
				// Draw cells text
				RECT r{ 0,0,columnsPositions[0],15 };
				HGDIOBJ oldFont;
				HFONT headerFont{};


				LOGFONT lf{};
				SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
				lf.lfWeight = FW_BOLD;
				UINT prevAlign = SetTextAlign(hdc, TA_CENTER);

				headerFont = CreateFontIndirect(&lf);

				oldFont = SelectObject(hdc, headerFont);
				SetBkColor(hdc, RGB(220, 220, 220));

				// Draw headers
				for (std::size_t i = 0; i != 4; ++i)
				{
					if (!i)
						r.left = columnsPositions[0] / 2;
					else
					{
						r.left = columnsPositions[i - 1] + (int)trunc((_width - FIRST_COLUMN_WIDTH) / 3.f) / 2;
						r.right = ((i != 3) ? columnsPositions[i] : _width) +
							(int)trunc((_width - FIRST_COLUMN_WIDTH) / 3.f);
					}

					TextOutA(hdc, r.left, r.top, tableRows[0][i].c_str(), (int)tableRows[0][i].length());
				}
				SetBkColor(hdc, RGB(255, 255, 255));
				DrawCellsData(hdc);
				SetBkColor(hdc, RGB(220, 220, 220));

				// Draw Numbers
				DeleteObject(headerFont);

				lf.lfWeight = FW_NORMAL;
				headerFont = CreateFontIndirect(&lf);
				SelectObject(hdc, headerFont);
				SetTextAlign(hdc, TA_LEFT);

				for (std::size_t i = 1; i != tableRows.size(); ++i)
				{
					r =
					{
						5,
						(ROW_HEIGHT * (int)i),
						columnsPositions[0],
						(ROW_HEIGHT + ROW_HEIGHT * (int)(i))
					};


					TextOutA(hdc, r.left, r.top, tableRows[i][0].c_str(), (int)tableRows[i][0].length());
				}


				SetBkColor(hdc, RGB(0, 0, 0));
				SetTextAlign(hdc, prevAlign);

				SelectObject(hdc, oldFont);
				DeleteObject(headerFont);
			}

			// Draw grid
			MoveToEx(hdc, 1, 1, NULL);
			LineTo(hdc, this->_width - 1, 1);
			LineTo(hdc, this->_width - 1, this->_height - 1);
			LineTo(hdc, 1, this->_height - 1);
			LineTo(hdc, 1, 0);

			for (std::size_t i = 0; i != columnsPositions.size(); ++i)
			{
				MoveToEx(hdc, columnsPositions[i], 0, NULL);
				LineTo(hdc, columnsPositions[i], _height);
			}
			for (int h = 0; h < _height; h += ROW_HEIGHT)
			{
				MoveToEx(hdc, 0, h, NULL);
				LineTo(hdc, _width, h);
			}



			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);
			DeleteObject(gridPen);
			DeleteObject(headersBrush);

			EndPaint(_thisWindow, &ps);
		}
	}
#pragma endregion
#pragma region Vectors operations
	std::set<ULONGLONG>& AddTable::GetSelectedNumbers()
	{
		return selectedNumbers;
	}
	std::vector<std::array<std::string, 4u>>& AddTable::GetSelectedData()
	{
		return selectedData;
	}

	void AddTable::SelectFirst()
	{
		if (RowIsEmpty(1))
		{
			MessageBoxA
			(GetParent(_thisWindow), "Нет таймеров в таблице", "Заполните таблицу", MB_OK | MB_ICONWARNING);

			return;
		}

		selectedNumbers.insert(std::to_unsigned_number(tableRows[1][0]));

		std::array<std::string, 4u> selected =
		{ 
			tableRows[1][0],
			tableRows[1][1],
			tableRows[1][2],
			tableRows[1][3]
		};
		selectedData.push_back(selected);

		tableRows.erase(tableRows.begin() + 1);

		ResetFocus();
	}
#pragma endregion
}