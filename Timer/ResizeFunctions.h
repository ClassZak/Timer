#pragma once
#include <functional>
#include <Windows.h>
#include <list>
#include <map>
#include <string>
namespace DeclarativeClasses
{
	typedef std::list<std::map<std::string, std::list<std::pair<std::string, HWND>>>> HANDLER_CONTAINER;


	std::function<BOOL(int w, int h, void* handlers)> L1
	(
		[](int w, int h, void* handlers)->BOOL
		{




			return EXIT_SUCCESS; 
		}
	);
}