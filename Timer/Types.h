#pragma once
#include <map>
#include <string>
#include <list>
#include <set>
#include <Windows.h>
namespace DeclarativeClasses
{
	typedef std::map<std::string, std::list<std::pair<std::string, HWND>>> HANDLER_CONTAINER;
	typedef std::set<std::pair<UINT, HWND>> NUMBERED_HANDLER_CONTAINER;
}