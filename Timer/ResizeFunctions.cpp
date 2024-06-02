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

					


					HWND addButtonH = std::find_if
					(
						realHandlers->at("buttons").begin(),
						realHandlers->at("buttons").end(),
						[](std::pair<std::string, HWND>& pair)->bool
						{
							return pair.first == "Add";
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

					return EXIT_SUCCESS;
				}
			);
		}
	}
}