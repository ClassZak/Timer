#pragma once
#include "ResizeFunctions.h"
#include "Types.h"
namespace DeclarativeClasses
{
	namespace Functions
	{
		namespace ResizeFunctions
		{
			extern std::function<BOOL(int w, int h, void* handlers)> L1
			(
				[](int w, int h, void* handlers)->BOOL
				{
					DeclarativeClasses::HANDLER_CONTAINER* realHandlers =
					static_cast<DeclarativeClasses::HANDLER_CONTAINER*>(handlers);

					

					{
						HWND addButtonH = std::find_if
						(
							realHandlers->at("buttons").begin(),
							realHandlers->at("buttons").end(),
							[](const std::pair<std::string, HWND>& pair)->bool
							{
								return pair.first == "add";
							}
						)->second;
						RECT addButtonRECT;
						GetClientRect(addButtonH, &addButtonRECT);
						MapWindowPoints(addButtonH, GetParent(addButtonH), (LPPOINT)(&addButtonRECT), 2);

						MoveWindow
						(
							addButtonH,
							addButtonRECT.left, addButtonRECT.top,
							addButtonRECT.right, h - addButtonRECT.top, FALSE
						);
					}



					{
						HWND addTable = std::find_if
						(
							realHandlers->at("tables").begin(),
							realHandlers->at("tables").end(),
							[](const std::pair<std::string, HWND>& pair)->bool
							{
								return pair.first == "addTable";
							}
						)->second;
						RECT addTableRect;
						GetClientRect(addTable, &addTableRect);
						MapWindowPoints(addTable, GetParent(addTable), (LPPOINT)(&addTableRect), 2);

						MoveWindow
						(
							addTable,
							addTableRect.left, addTableRect.top,
							w / 2 - addTableRect.left, h - addTableRect.top,
							FALSE
						);

						InvalidateRect(addTable,NULL, TRUE);
						InvalidateRect(GetParent(addTable),NULL, TRUE);
					}
					{
						HWND showListView = std::find_if
						(
							realHandlers->at("listViews").begin(),
							realHandlers->at("listViews").end(),
							[](const std::pair<std::string, HWND>& pair)->bool
							{
								return pair.first == "showListView";
							}
						)->second;
						RECT showListViewRECT;
						GetClientRect(showListView, &showListViewRECT);
						MapWindowPoints(showListView, GetParent(showListView), (LPPOINT)(&showListViewRECT), 2);

						MoveWindow
						(
							showListView,
							w / 2, showListViewRECT.top-2,
							w / 2, h - showListViewRECT.top+2, FALSE
						);

						InvalidateRect(showListView, NULL, TRUE);
					}
					

					return EXIT_SUCCESS;
				}
			);
			extern std::function<BOOL(int w, int h, void* handlers)> ResizeAddTable
			(
				[](int w, int h, void* handlers)->BOOL
				{
					HANDLER_CONTAINER* hContainer = static_cast<HANDLER_CONTAINER*>(handlers);


					int numberWidth = 40;
					int otherWidth = w - numberWidth;
					
					{
						auto it = hContainer->at("textBoxHeader").begin();
						++it;

						RECT rect{};
						for (int i=0; it != hContainer->at("textBoxHeader").end(); ++it,++i)
						{
							GetClientRect(it->second, &rect);
							MapWindowPoints(it->second, GetParent(it->second), (LPPOINT)&rect, 2);

							rect.left = 40 + otherWidth / 3 * i;
							rect.right = rect.left + otherWidth / 3;

							if (i == 2)
								rect.right += otherWidth % 3;

							MoveWindow
							(
								it->second,
								rect.left, rect.top-1,
								rect.right - rect.left, rect.bottom - rect.top+2,
								TRUE
							);
						}
					}
					{
						auto it = hContainer->at("textBox").begin();

						RECT rect{};
						for (int i=0; it != hContainer->at("textBox").end(); ++it,++i)
						{
							GetClientRect(it->second, &rect);
							MapWindowPoints(it->second, GetParent(it->second), (LPPOINT)&rect, 2);

							if ((i % 4))
							{
								rect.left = 40 + otherWidth / 3 * ((i-1)%4);
								rect.right = rect.left + otherWidth / 3;
							}
							else
							{
								rect.left = 0;
								rect.right = 40;
							}


							

							if (i %4== 3)
								rect.right += otherWidth % 3;
							MoveWindow
							(
								it->second,
								rect.left, rect.top,
								rect.right - rect.left, rect.bottom - rect.top,
								FALSE
							);
						}
					}




					return EXIT_SUCCESS;
				}
			);
		}
	}
}