#pragma once
#include <Windows.h>
#include <list>
#include <map>
#include <string>
#include <functional>


class Form
{
public:
	Form();
	
private:
	std::list<std::map<std::string, std::list<HWND>>> handlers_{};
	int width_ = 0, height_ = 0;
	std::function<BOOL> resizeFunction_ = nullptr;
};

