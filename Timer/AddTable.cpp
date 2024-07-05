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

	inline POINT AddTable::GetSelectedIndex(POINT mousePos)
	{
		return
		{
			mousePos.x<= columnsPositions[0] ? 0 :
			(mousePos.x<= columnsPositions[1] ? 1 :
			(mousePos.x<=columnsPositions[2] ? 2 : 3)),
			mousePos.y / 15
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
		columnsPositions[0] = 40;
		columnsPositions[1] = (int)trunc((_width - 40) / 3.f) + 40;
		columnsPositions[2] = ((int)trunc((_width - 40) / 3.f) << 1) + 40;
	}

	void AddTable::FillNumbers()
	{
		std::array<std::string, 4> arr;

		for (int h = ROW_HEIGHT,i=1; h <= _height; h += ROW_HEIGHT,++i)
		{
			if (i < tableRows.size())
				continue;

			arr[0] = std::to_string(i);
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
				tableRows[i][1] == "" and
				tableRows[i][2] == "" and
				tableRows[i][3] == "" and
				(i*ROW_HEIGHT)>_height
			)
				tableRows.pop_back();
			else
				break;
		}
	}
	
	
	
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
		case WM_LBUTTONDOWN:
		{
			POINT el = GetSelectedIndex({ LOWORD(lParam) , HIWORD(lParam) });


			m_editWindow.position = el;
			RECT r = GetSelectedCellRect(el);
			r.right -= r.left;
			r.bottom -= r.top;

			if (m_editWindow.editWindow == NULL)
			{
				HWND edit= CreateWindowExA
				(
					0L,
					"edit",
					tableRows[el.y][el.x].c_str(),
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
				editLF.lfWeight = !el.y ? FW_BOLD : FW_NORMAL;

				HFONT hFont = CreateFontIndirect(&editLF);

				SendMessageA(edit, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
			else
			{
				MoveWindow
				(
					m_editWindow.editWindow,
					r.left,
					r.top+1,
					r.right-1,
					r.bottom-1,
					FALSE
				);
				SetWindowTextA(m_editWindow.editWindow, tableRows[el.y][el.x].c_str());

				LOGFONT editLF{};
				editLF.lfHeight = 12;
				SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &editLF, 0);
				editLF.lfWeight = !el.y ? FW_BOLD : FW_NORMAL;

				HFONT hFont = CreateFontIndirect(&editLF);

				SendMessageA(m_editWindow.editWindow, WM_SETFONT, (WPARAM)hFont, TRUE);
			}
			if (!el.y)
			{
				LONG_PTR style = GetWindowLongPtr(m_editWindow.editWindow, GWL_STYLE); // Получаем текущий стиль окна

				
				style &= ~ES_LEFT; // Delete ES_LEFT
				style |= ES_CENTER | ES_READONLY; // Add header style

				SetWindowLongPtr(m_editWindow.editWindow, GWL_STYLE, style);
			}
			else
			{
				LONG_PTR style = GetWindowLongPtr(m_editWindow.editWindow, GWL_STYLE); // Получаем текущий стиль окна


				style &= ~ES_CENTER | ES_READONLY; // Delete header style
				style |= ES_LEFT; // Add ES_LEFT

				SetWindowLongPtr(m_editWindow.editWindow, GWL_STYLE, style);
			}



			SetFocus(m_editWindow.editWindow);
			

			InvalidateRect(hWnd, NULL, TRUE);

			//MessageBoxA(hWnd, (std::to_string(el.x) + ", " + std::to_string(el.y)).c_str(), "", MB_OK);
			break;
		}
		case WM_SIZE:
		{
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
			if (this)
			{
				PAINTSTRUCT ps{};
				HDC hdc = BeginPaint(hWnd, &ps);


				HGDIOBJ oldPen,oldBrush;
				HPEN gridPen = CreatePen(PS_SOLID,2,RGB(100, 100, 100));
				HBRUSH headersBrush = CreateSolidBrush(RGB(220,220,220));
				oldPen=SelectObject(hdc, gridPen);
				oldBrush = SelectObject(hdc, headersBrush);

				// Header background
				RECT rect{ 1,1,_width,ROW_HEIGHT };
				FillRect(hdc, &rect, headersBrush);
				rect = { 0,ROW_HEIGHT,40,_height };
				FillRect(hdc, &rect, headersBrush);

				{
					// Draw cells text
					RECT r{0,0,columnsPositions[0],15};
					HGDIOBJ oldFont;
					HFONT headerFont{};


					LOGFONT lf{};
					lf.lfHeight = 12;
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
							r.left = columnsPositions[0]/2;
						else
						{
							r.left = columnsPositions[i - 1] + (int)trunc((_width - 40) / 3.f) / 2;
							r.right = ((i != 3) ? columnsPositions[i] : _width) + (int)trunc((_width - 40) / 3.f);
						}

						TextOutA(hdc, r.left, r.top, tableRows[0][i].c_str(), (int)tableRows[0][i].length());
					}


					// Draw Numbers
					DeleteObject(headerFont);

					lf.lfWeight = FW_NORMAL;
					headerFont= CreateFontIndirect(&lf);
					SelectObject(hdc,headerFont);
					SetTextAlign(hdc, TA_LEFT);

					for (std::size_t i = 1; i != tableRows.size(); ++i)
					{
						r =
						{ 
							3,
							(ROW_HEIGHT * (int)i),
							columnsPositions[0],
							(ROW_HEIGHT + ROW_HEIGHT*(int)(i))
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
				LineTo(hdc, this->_width-1, 1);
				LineTo(hdc, this->_width-1, this->_height-1);
				LineTo(hdc, 1, this->_height-1);
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

				EndPaint(hWnd, &ps);
			}
			break;
		}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT AddTable::EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		AddTable* tablePtr = reinterpret_cast<AddTable*>(dwRefData);


		/*if (msg == WM_KEYDOWN)
		{
			if (
				wParam == VK_RETURN or
				wParam == VK_TAB
				)
			{
				tablePtr->SortByEnteredCell((UINT)uIdSubclass, hWnd);
				tablePtr->KillCellsFocus();

				if ((wParam == VK_RETURN and ((GetKeyState(VK_CONTROL) & 0x8000) or (GetKeyState(VK_SHIFT) & 0x8000))) or
					wParam == VK_TAB)
					tablePtr->ResetFocus
					((UINT)uIdSubclass, wParam == VK_RETURN ? DeclarativeClasses::Down : DeclarativeClasses::Right);

				return 0;
			}

			if ((GetKeyState(VK_CONTROL) & 0x8000) or (GetKeyState(VK_SHIFT) & 0x8000))
			{
				if
				(
					wParam == VK_LEFT or
					wParam == VK_UP or
					wParam == VK_RIGHT or
					wParam == VK_DOWN
				)
				{
					tablePtr->SortByEnteredCell((UINT)uIdSubclass, hWnd);
					tablePtr->KillCellsFocus();
					tablePtr->ResetFocus((UINT)uIdSubclass, (DeclarativeClasses::Direction)(wParam - 0x24));

					return 0;
				}
			}
		}*/

		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}
}