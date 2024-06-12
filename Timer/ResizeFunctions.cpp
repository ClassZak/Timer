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
					}
					

					return EXIT_SUCCESS;
				}
			);
		}
	}
}