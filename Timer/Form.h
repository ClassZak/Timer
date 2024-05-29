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
	Form(int w, int h);
	Form(std::list<std::map<std::string, std::list<HWND>>> handlers);
	Form(int w, int h, std::list<std::map<std::string, std::list<HWND>>> handlers);
	Form(std::function<BOOL(void)>& function);
	Form(std::list<std::map<std::string, std::list<HWND>>> handlers, std::function<BOOL(void)>& function);
	Form(int w, int h, std::list<std::map<std::string, std::list<HWND>>> handles, std::function<BOOL(void)>& function);

	Form(Form& other);


	~Form();



	void SetResizeMethod(std::function<BOOL(void)>& function);
	void Resize();
	void SetNewSize(int x, int y);
private:
	std::list<std::map<std::string, std::list<HWND>>> handlers_{};
	int width_ = 0, height_ = 0;
	std::function<BOOL(void)> resizeFunction_;
};

