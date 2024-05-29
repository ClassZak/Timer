#pragma once
#include <Windows.h>
#include <list>
#include <map>
#include <string>
#include <functional>

namespace DeclarativeClasses
{
typedef std::list<std::map<std::string, std::list<std::pair<std::string, HWND>>>> HANDLER_CONTAINER;

class Form
{
public:
	Form();
	Form(int w, int h);
	Form(HANDLER_CONTAINER handlers);
	Form(int w, int h, HANDLER_CONTAINER handlers);
	Form(std::function<BOOL(int, int, void*)>& function);
	Form(HANDLER_CONTAINER handlers,std::function<BOOL(int, int, void*)>& function);
	Form(int w, int h, HANDLER_CONTAINER handles,std::function<BOOL(int, int, void*)>& function);

	Form(Form& other);


	~Form();



	void SetResizeMethod(std::function<BOOL(int,int, void*)>& function);
	void Resize();
	void SetNewSize(int w, int h);
private:
	HANDLER_CONTAINER handlers_{};
	int width_ = 0, height_ = 0;
	std::function<BOOL(int, int, void*)> resizeFunction_;
};
}



