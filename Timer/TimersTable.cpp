#include "TimersTable.h"
namespace DeclarativeClasses
{
	const std::array<int, 3> TimersTable::COLUMNS_WIDTHS = { 16,16,25 };
	std::mutex TimersTable::timerMutex = std::mutex();

#pragma region Constructors
	TimersTable::TimersTable(UINT cols, UINT rows)
		: ATable(cols, rows)
	{
	}
	TimersTable::TimersTable(UINT cols, UINT rows, int w, int h)
		: ATable(cols, rows, w, h)
	{
	}
	TimersTable::TimersTable(UINT cols, UINT rows, HANDLER_CONTAINER handlers)
		: ATable(cols, rows, handlers)
	{
	}
	TimersTable::TimersTable(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers)
		: ATable(cols, rows, w, h, handlers)
	{
	}
	TimersTable::TimersTable(UINT cols, UINT rows, std::function<BOOL(int, int, void*)>& function)
		: ATable(cols, rows, function)
	{
	}
	TimersTable::TimersTable
	(UINT cols, UINT rows, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
		: ATable(cols, rows, handlers, function)
	{
	}
	TimersTable::TimersTable
	(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
		: ATable(cols, rows, w, h, handlers, function)
	{
	}
	TimersTable::TimersTable(UINT cols, UINT rows, Form& other)
		: ATable(cols, rows, other)
	{
	}
	TimersTable::TimersTable(UINT cols, UINT rows, ControlForm& other)
		: ATable(cols, rows, other)
	{
	}
	TimersTable::TimersTable(UINT cols, UINT rows, ATable& other)
		: ATable(cols, rows, other)
	{
	}




	TimersTable::TimersTable(UINT cols, UINT rows, TimersTable& other)
		: TimersTable(cols, rows, (ATable&)other)
	{
	}
#pragma endregion
#pragma region Proccesses
	void TimersTable::CreateSelf(const CreateWindowArgs& args)
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

		SetWindowLongPtrA(_thisWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		SetTimer(_thisWindow, NULL, 1000, UpdateTimersTime);

		isInitilized = true;
	}
	LRESULT TimersTable::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN | ES_READONLY,
					0,
					-40,
					0,
					0,
					_thisWindow,
					(HMENU)EDIT_WINDOW,
					GetModuleHandleA(NULL),
					this
				);
				LOGFONT editLF{};
				SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &editLF, 0);
				editLF.lfWeight = FW_NORMAL;

				HFONT hFont = CreateFontIndirect(&editLF);

				SendMessageA(edit, WM_SETFONT, (WPARAM)hFont, TRUE);
				SetWindowSubclass(edit, &ATable::EditProcStatic<TimersTable>, EDIT_WINDOW, (DWORD_PTR)this);


				m_editWindow.editWindow = edit;
				break;
			}
			case WM_LBUTTONDOWN:
			{
				if (HIWORD(lParam) > this->timers.size()*ROW_HEIGHT)
				{
					HideEditWindow();
					SetFocus(GetParent(hWnd));
				}
				else
				{
					if (LOWORD(lParam) < COLUMNS_WIDTHS[0] + COLUMNS_WIDTHS[1])
					{
						std::size_t rowId = HIWORD(lParam) / ROW_HEIGHT;

						if (rowId > timers.size())
							break;

						int col = LOWORD(lParam) >= COLUMNS_WIDTHS[0];


						if (col)
						{
							timers[rowId].paused = timers[rowId].triggered ? false : !timers[rowId].paused;

							RECT updateRect
							{
								(!col) ? 0 : COLUMNS_WIDTHS[0],
								(LONG)(rowId * ROW_HEIGHT),
								(!col) ? COLUMNS_WIDTHS[0] : COLUMNS_WIDTHS[0]+ COLUMNS_WIDTHS[1],
								(LONG)(rowId * ROW_HEIGHT + ROW_HEIGHT)
							};

							InvalidateRect(hWnd, &updateRect, TRUE);
						}
						else
						{
							timers[rowId].stopped = true;
							SendMessageA(GetParent(hWnd), WM_COMMAND, TIMER_STOPPED, timers[rowId].number);

							timersNumbers.erase(timers[rowId].number);
							StopTimer(rowId);


							RECT updateRect
							{
								0,(LONG)(rowId * ROW_HEIGHT),
								_width,(LONG)((timers.size() + 2) * ROW_HEIGHT)
							};
							InvalidateRect(hWnd, &updateRect, TRUE);
							break;
						}
					}
					else
					{
						POINT el = GetSelectedIndex({ LOWORD(lParam),HIWORD(lParam) });
						if (el.y >= timers.size())
							break;


						RECT editPos = GetSelectedCellRect(el);


						MoveWindow
						(
							m_editWindow.editWindow,
							editPos.left, editPos.top,
							editPos.right - editPos.left, editPos.bottom - editPos.top,
							TRUE
						);
						m_editWindow.position = el;
						switch (el.x)
						{
							case 0:
							{
								std::string editData;
								editData = std::to_string(timers[el.y].number);
								SetWindowTextA(m_editWindow.editWindow, editData.c_str());
								break;
							}
							case 1:
							{
								SetWindowTextA(m_editWindow.editWindow, timers[el.y].name.c_str());
								break;
							}
							case 2:
							{
								SetWindowTextA(m_editWindow.editWindow, timers[el.y].description.c_str());
								break;
							}
							case 3:
							{
								std::string editData;
								editData = TimeToString(timers[el.y].time);
								SetWindowTextA(m_editWindow.editWindow, editData.c_str());
								break;
							}
							default:
							{
								throw;
								break;
							}
						}
						SendMessageA
						(
							m_editWindow.editWindow,
							EM_SETSEL,
							GetWindowTextLengthA(m_editWindow.editWindow),
							GetWindowTextLengthA(m_editWindow.editWindow)
						);

						SetFocus(m_editWindow.editWindow);
					}
				}



				break;
			}
			case WM_SIZE:
			{
				if (!(LOWORD(lParam) and HIWORD(lParam)))
					break;

				SetNewSize(LOWORD(lParam), HIWORD(lParam));

				int remainingWidth = LOWORD(lParam) - CONSERVATIVE_WIDTH;
				columnsPositions[0] = CONSERVATIVE_WIDTH + remainingWidth / 3;
				columnsPositions[1] = columnsPositions[0] + remainingWidth / 3;


				HideEditWindow();
				SetFocus(hWnd);
				break;
			}
			case WM_PAINT:
			{
				Draw();
				break;
			}
			default:
				break;
		}


		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
	LRESULT TimersTable::EditProc
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		TimersTable* addTable = reinterpret_cast<TimersTable*>(dwRefData);
		
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}
#pragma region Drawing
	inline void TimersTable::Draw()
	{
		if (this)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(_thisWindow, &ps);

			HGDIOBJ oldPen;
			HPEN gridPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
			oldPen = SelectObject(hdc, gridPen);
			HBRUSH background = CreateSolidBrush(RGB(220, 220, 220));
			HBRUSH timeBackground= CreateSolidBrush(RGB(240, 240, 240));

			if(timers.size())
			{
				DrawStopButtons(hdc);
				DrawPauseButtons(hdc);

				COLORREF oldBackColor = SetBkColor(hdc, RGB(220, 220, 220));

				LOGFONT lf{};
				SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
				lf.lfWeight = FW_NORMAL;
				HFONT font = CreateFontIndirect(&lf);
				HGDIOBJ oldFont = SelectObject(hdc, font);


				{
					// For number, name and for description
					RECT descriptionRect
					{
						COLUMNS_WIDTHS[0] + COLUMNS_WIDTHS[1],
						0,
						columnsPositions[1],
						ROW_HEIGHT * timers.size() < (std::size_t)_height ? (LONG)(ROW_HEIGHT * timers.size()) : _height
					};

					FillRect(hdc, &descriptionRect, background);

					descriptionRect.left = columnsPositions[1];
					descriptionRect.right = _width;

					FillRect(hdc, &descriptionRect, timeBackground);
				}


				DrawNumbers(hdc);

				DrawTimersDescriptions(hdc);
				SetBkColor(hdc, RGB(240,240,240));

				{
					HGDIOBJ oldBackground=
					SelectObject(hdc, timeBackground);
					DrawTimersTime(hdc);
					SelectObject(hdc, oldBackground);
				}

				SetBkColor(hdc, oldBackColor);

				DrawRowLine(hdc);
				DrawColumnsLines(hdc);

				DeleteObject(font);
			}

			MoveToEx(hdc, 1, 1, NULL);
			LineTo(hdc, _width-1, 1);
			LineTo(hdc, _width-1, _height-1);
			LineTo(hdc, 1, _height-1);
			LineTo(hdc, 1, 0);

			

			DeleteObject(gridPen);
			DeleteObject(background);
			DeleteObject(timeBackground);

			EndPaint(_thisWindow, &ps);
		}
	}
	inline void TimersTable::DrawStopButtons(HDC& hdc)
	{
		HGDIOBJ oldBrush;
		HBRUSH blackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HBRUSH whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);

		oldBrush = SelectObject(hdc, blackBrush);

		RECT button{ 0,0,COLUMNS_WIDTHS[0],ROW_HEIGHT };
		RECT stopRect{};
		for (std::size_t i = 0; i != timers.size(); ++i)
		{
			stopRect = button;

			if (!timers[i].stopped)
			{
				stopRect.left += 4;
				stopRect.top += 4;
				stopRect.right -= 4;
				stopRect.bottom -= 4;
			}

			FillRect(hdc, &button, whiteBrush);
			FillRect(hdc, &stopRect, blackBrush);

			button.top += ROW_HEIGHT;
			button.bottom += ROW_HEIGHT;
		}




		SelectObject(hdc, oldBrush);
	}
	inline void TimersTable::DrawPauseButtons(HDC& hdc)
	{
		HGDIOBJ oldBrush;
		HBRUSH blackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HBRUSH whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);

		oldBrush = SelectObject(hdc, blackBrush);

		RECT button{ COLUMNS_WIDTHS[0],0,COLUMNS_WIDTHS[0]+COLUMNS_WIDTHS[1],ROW_HEIGHT };
		for (std::size_t i = 0; i != timers.size() and button.top<_height; ++i)
		{
			FillRect(hdc, &button, whiteBrush);

			if (timers[i].paused)
			{
				HPEN blackPen = (HPEN)GetStockObject(BLACK_PEN);
				HGDIOBJ oldPen = SelectObject(hdc,blackPen);


				POINT points[3] =
				{
					{
						button.left + 4,
						button.top + 4
					},
					{
						button.left + (button.right - button.left) / 2,
						button.top + (button.bottom - button.top) / 2
					},
					{
						button.left + 4,
						button.bottom - 4
					}
				};

				Polygon(hdc, points, 3);


				SelectObject(hdc,oldPen);
			}
			else
			{
				RECT leftPart
				{
					button.left + 4,
					button.top + 4,
					button.left + (button.right - button.left) / 2 - 1,
					button.bottom - 4
				},
					rightPart
				{
					button.left + (button.right - button.left) / 2 + 1,
					button.top + 4,
					button.right - 4,
					button.bottom - 4
				};

				FillRect(hdc, &leftPart, blackBrush);
				FillRect(hdc, &rightPart, blackBrush);
			}


			button.top += ROW_HEIGHT;
			button.bottom += ROW_HEIGHT;
		}




		SelectObject(hdc, oldBrush);
	}
	inline void TimersTable::DrawRowLine(HDC& hdc)
	{
		int height = ROW_HEIGHT;
		for (std::size_t i = 0; i != timers.size() and height<_height; ++i, height+= ROW_HEIGHT)
		{
			MoveToEx(hdc, 0, height, NULL);
			LineTo(hdc, _width, height);
		}
	}
	inline void TimersTable::DrawColumnsLines(HDC& hdc)
	{
		int height = _height > ROW_HEIGHT * timers.size() ? (int)(ROW_HEIGHT * timers.size()) : _height;

		MoveToEx(hdc, COLUMNS_WIDTHS[0], 0, NULL);
		LineTo(hdc, COLUMNS_WIDTHS[0], height);

		MoveToEx(hdc, COLUMNS_WIDTHS[0] + COLUMNS_WIDTHS[1], 0, NULL);
		LineTo(hdc, COLUMNS_WIDTHS[0] + COLUMNS_WIDTHS[1], height);

		MoveToEx(hdc, CONSERVATIVE_WIDTH, 0, NULL);
		LineTo(hdc, CONSERVATIVE_WIDTH, height);



		MoveToEx(hdc, columnsPositions[0], 0, NULL);
		LineTo(hdc, columnsPositions[0], height);
		
		MoveToEx(hdc, columnsPositions[1], 0, NULL);
		LineTo(hdc, columnsPositions[1], height);

	}
	inline void TimersTable::DrawNumbers(HDC& hdc)
	{
		RECT rect
		{
			COLUMNS_WIDTHS[0] + COLUMNS_WIDTHS[1] + 5,
			0,
			CONSERVATIVE_WIDTH,
			ROW_HEIGHT
		};
		for (std::size_t i = 0; i != timers.size() and rect.top < _height; ++i)
		{
			std::string number = std::to_string(timers[i].number);
			DrawTextA(hdc, number.c_str(), (int)number.length(), &rect, NULL);

			rect.top += ROW_HEIGHT;
			rect.bottom += ROW_HEIGHT;
		}
	}
	inline void TimersTable::DrawTimersDescriptions(HDC& hdc)
	{
		// Draw names
		RECT textRect{ CONSERVATIVE_WIDTH +5,0,columnsPositions[0],ROW_HEIGHT };
		for (std::size_t i = 0; i != timers.size() and textRect.top < _height; ++i)
		{
			DrawTextA(hdc, timers[i].name.c_str(), (int)timers[i].name.size(), &textRect, NULL);

			textRect.top += ROW_HEIGHT;
			textRect.bottom += ROW_HEIGHT;
		}
		// Draw descriptions
		textRect = { columnsPositions[0] + 5,0,columnsPositions[1], ROW_HEIGHT };
		for (std::size_t i = 0; i != timers.size() and textRect.top < _height; ++i)
		{
			DrawTextA(hdc, timers[i].description.c_str(), (int)timers[i].description.size(), &textRect, NULL);

			textRect.top += ROW_HEIGHT;
			textRect.bottom += ROW_HEIGHT;
		}
	}
	inline void TimersTable::DrawTimersTime(HDC& hdc)
	{	
		// Draw time
		HBRUSH alarmBrush = CreateSolidBrush(RGB(240, 70, 70));
		HGDIOBJ oldBrush=SelectObject(hdc,alarmBrush);

		bool isAlarm = false;

		RECT textRect = { columnsPositions[1] + 5,0,_width, ROW_HEIGHT };
		for (std::size_t i = 0; i != timers.size() and textRect.top < _height; ++i)
		{
			textRect.left -= 5;
			if (!timers[i].time.tm_sec and !timers[i].time.tm_min and !timers[i].time.tm_hour)
			{
				FillRect(hdc, &textRect, alarmBrush);

				if (!isAlarm)
				{
					SetBkColor(hdc, RGB(240, 70, 70));
					isAlarm = true;
				}
			}
			else
			{
				FillRect(hdc, &textRect, (HBRUSH)oldBrush);

				if (isAlarm)
				{
					SetBkColor(hdc, RGB(240, 240, 240));
					isAlarm = false;
				}
			}
			textRect.left += 5;

			std::string timeStr = TimeToString(timers[i].time);
			DrawTextA(hdc, timeStr.c_str(), (int)timeStr.size(), &textRect, NULL);

			textRect.top += ROW_HEIGHT;
			textRect.bottom += ROW_HEIGHT;
		}

		DeleteObject(alarmBrush);
		SelectObject(hdc, oldBrush);
	}
	inline void TimersTable::DrawRow(HDC& hdc,const std::size_t& rowId)
	{
		if (rowId >= timers.size())
			throw;


		MoveToEx(hdc, 1, (int)((rowId + 1) * ROW_HEIGHT), NULL);
		LineTo(hdc, _width - 1, (int)((rowId + 1) * ROW_HEIGHT));



		LOGFONT lf{};
		SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
		lf.lfWeight = FW_NORMAL;
		HGDIOBJ oldFont;

		HFONT font = CreateFontIndirect(&lf);
		oldFont = SelectObject(hdc, font);



		RECT rect{};

		for (int i = 0; i != 3; ++i)
		{
			rect.left = COLUMNS_WIDTHS[i];
			rect.top = (LONG)(rowId * ROW_HEIGHT);
			rect.right = (i == 2) ? columnsPositions[0] :
				COLUMNS_WIDTHS[static_cast<std::array<int, 3Ui64>::size_type>(i) + 1];
			rect.bottom = rect.top + ROW_HEIGHT;

			if (i != 2)
			{
				HGDIOBJ oldBrush;
				HBRUSH background = CreateSolidBrush(RGB(70, 70, 70));
				HBRUSH blackBrussh = CreateSolidBrush(RGB(0, 0, 0));

				oldBrush = SelectObject(hdc,background);

				FillRect(hdc, &rect, background);

				if (!i)
				{
					RECT stopRect{ 0 + 2,rect.top + 2,COLUMNS_WIDTHS[0] - 2,rect.bottom - 2 };
					FillRect(hdc, &stopRect, blackBrussh);
				}
				else
				{
					RECT pauseRect1{ 0 + 2,rect.top + 2,COLUMNS_WIDTHS[0] / 2 - 2,rect.bottom - 2 };
					RECT pauseRect2{ COLUMNS_WIDTHS[0] + 2,rect.top + 2,COLUMNS_WIDTHS[0] / 2 - 2,rect.bottom - 2 };

					FillRect(hdc, &pauseRect1, blackBrussh);
					FillRect(hdc, &pauseRect2, blackBrussh);
				}




				DeleteObject(blackBrussh);
				DeleteObject(background);
				SelectObject(hdc, oldBrush);
			}
			else
			{

			}
		}
		DeleteObject(font);
		SelectObject(hdc, oldFont);
	}
#pragma endregion
#pragma endregion
#pragma region Timer threads
	void TimersTable::UpdateTimersTime(HWND hWnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
	{
		TimersTable* table = reinterpret_cast<TimersTable*> (GetWindowLongPtrA(hWnd, GWLP_USERDATA));

		for (std::size_t i = 0; i != table->timers.size(); ++i)
		{
			if (!table->timers[i].time.tm_hour and !table->timers[i].time.tm_min and !table->timers[i].time.tm_sec)
			{
				if (!table->timers[i].triggered)
				{
					table->timers[i].triggered = true;
					table->timers[i].paused = false;

					RECT updateRect
					{
						COLUMNS_WIDTHS[0],
						(LONG)(i * ROW_HEIGHT),
						COLUMNS_WIDTHS[0] + COLUMNS_WIDTHS[1],
						(LONG)(i * ROW_HEIGHT + ROW_HEIGHT)
					};

					InvalidateRect(hWnd, &updateRect, TRUE);


					ThreadTimerStruct* threadStruct = new ThreadTimerStruct[1];
					*threadStruct = { reinterpret_cast<void*>(table), i };

					CreateThread(NULL, sizeof(TimersTable) * 2, AlarmPlay, (void*)threadStruct, 0, NULL);
					
				}
			}
			else
			{
				if (table->timers[i].paused)
					continue;

				if (--table->timers[i].time.tm_sec == -1)
				{
					table->timers[i].time.tm_sec = 59;
					if (--table->timers[i].time.tm_min == -1)
					{
						table->timers[i].time.tm_min = 59;
						--table->timers[i].time.tm_hour;
					}
				}

				RECT updateRect
				{
					table->columnsPositions[1],
					0,
					table->_width,
					table->timers.size() * TimersTable::ROW_HEIGHT
				};
				InvalidateRect(table->_thisWindow, &updateRect, TRUE);

				if (table->m_editWindow.position.x == 3 and table->m_editWindow.position.y != -1)
				{
					std::string editData;
					editData = TimeToString(table->timers[table->m_editWindow.position.y].time);
					SetWindowTextA(table->m_editWindow.editWindow, editData.c_str());
				}
			}
		}
	}
	DWORD TimersTable::AlarmPlay(void* lParam)
	{
		if (lParam == NULL)
			throw;

		ThreadTimerStruct* timerStruct = reinterpret_cast<ThreadTimerStruct*>(lParam);
		TimersTable* table = reinterpret_cast<TimersTable*>(timerStruct->timersTable);
		TimerStruct timer = table->timers[timerStruct->number];


		std::size_t id = timerStruct->number;
		unsigned long long number = timer.number;

		try
		{
			PlaySoundA(ALARM_SOUND_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

			while (true)
			{
				timerMutex.lock();
				if (!table->timers.size())
				{
					PlaySoundA(NULL, NULL, 0);
					timerMutex.unlock();
					return EXIT_SUCCESS;
				}
				if (id >= table->timers.size()-1)
				{
					id = -1;
					for (std::size_t i = 0; i != table->timers.size(); ++i)
						if (table->timers[i].number == number)
						{
							id = i;
							break;
						}

					if (id == -1)
					{
						PlaySoundA(NULL, NULL, 0);
						timerMutex.unlock();
						return EXIT_SUCCESS;
					}
				}
				else
					if (table->timers[id].number != number)
					{
						id = -1;
						for (std::size_t i = 0; i != table->timers.size(); ++i)
							if (table->timers[i].number == number)
							{
								id = i;
								break;
							}

						if (id == -1)
						{
							PlaySoundA(NULL, NULL, 0);
							timerMutex.unlock();
							return EXIT_SUCCESS;
						}
					}

				if (table->timers[id].stopped or !table->timers[id].triggered)
				{
					PlaySoundA(NULL, NULL, 0);
					timerMutex.unlock();
					return EXIT_SUCCESS;
				}

				if (!table->timers.size())
					break;

				timerMutex.unlock();
				Sleep(1000);
			}
		}
		catch (const std::exception&)
		{
			PlaySoundA(NULL, NULL, 0);
			timerMutex.unlock();
			return EXIT_SUCCESS;
		}

		PlaySoundA(NULL, NULL, 0);
		timerMutex.unlock();
		return EXIT_SUCCESS;
	}
#pragma endregion

	void TimersTable::AddRow(const std::array<std::string, 4>& data)
	{
		std::string timeString = data[3];
		TimerStruct timeStruct(data[1], data[2], StringToTime(timeString), std::to_unsigned_number(data[0]));

		timerMutex.lock();
		timers.push_back(timeStruct);
		timersNumbers.insert(timeStruct.number);
		timerMutex.unlock();
	}
	void TimersTable::ClearRows()
	{
		timers.clear();
	}
	void TimersTable::StopTimer(const std::size_t number)
	{
		timerMutex.lock();

		if (number > timers.size())
			throw;

		timers[number].stopped = true;
		timers[number].triggered = false;
		timers.erase(timers.begin() + number);

		timerMutex.unlock();
	}

	inline POINT TimersTable::GetSelectedIndex(POINT mousePos)
	{
		return
		{
			mousePos.x < COLUMNS_WIDTHS[0] + COLUMNS_WIDTHS[1] ? -1 :
			(mousePos.x <= CONSERVATIVE_WIDTH ? 0 :
			(mousePos.x <= columnsPositions[0] ? 1 :
			(mousePos.x <= columnsPositions[1] ? 2 : 3))),
			mousePos.y / ROW_HEIGHT
		};
	}
	inline RECT TimersTable::GetSelectedCellRect(POINT pos)
	{
		return
		{
			(!pos.x) ? CONSERVATIVE_WIDTH - COLUMNS_WIDTHS[2] :
			(pos.x==1 ? CONSERVATIVE_WIDTH : columnsPositions[pos.x - 2]),

			pos.y*ROW_HEIGHT,

			(pos.x==0) ? CONSERVATIVE_WIDTH : ((pos.x == 3) ? _width : columnsPositions[pos.x-1]),
			(pos.y+1)*ROW_HEIGHT
		};
	}
	void TimersTable::HideEditWindow()
	{
		MoveWindow(m_editWindow.editWindow, 0, -40, 0, 0, TRUE);
		m_editWindow.position.x = m_editWindow.position.y = -1;
	}
}