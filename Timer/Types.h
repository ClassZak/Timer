#pragma once
#include <map>
#include <string>
#include <list>
#include <Windows.h>
namespace DeclarativeClasses
{
	typedef std::map<std::string, std::list<std::pair<std::string, HWND>>> HANDLER_CONTAINER;
}