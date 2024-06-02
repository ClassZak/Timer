#pragma once
#include <Windows.h>
#include <list>
#include <map>
#include <string>
#include <functional>
namespace DeclarativeClasses
{
	namespace Functions
	{
		namespace ResizeFunctions
		{
			extern std::function<BOOL(int w, int h, void* handlers)> L1;
		}
	}
}
