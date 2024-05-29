#include "Form.h"

Form::Form()
{

}

Form::Form(int w, int h) : Form::Form()
{
	width_ = w;
	height_ = h;
}

Form::Form(std::list<std::map<std::string, std::list<HWND>>> handlers)
{
	handlers_ = handlers;
}

Form::Form(int w, int h, std::list<std::map<std::string, std::list<HWND>>> handlers) : Form::Form(w,h)
{
	handlers_ = handlers;
}

Form::Form(std::function<BOOL(void)>& function)
{
	resizeFunction_ = function;
}

Form::Form(std::list<std::map<std::string, std::list<HWND>>> handlers, std::function<BOOL(void)>& function)
	: Form::Form(function)
{
	handlers_ = handlers;
}

Form::Form(int w, int h, std::list<std::map<std::string, std::list<HWND>>> handles, std::function<BOOL(void)>& function)
	: Form::Form(w,h,handles)
{
	resizeFunction_ = function;
}

Form::Form(Form& other)
	: Form::Form(other.width_, other.height_,other.handlers_,other.resizeFunction_)
{
}

Form::~Form()
{
}

void Form::SetResizeMethod(std::function<BOOL(void)>& function)
{
}

void Form::Resize()
{
}

void Form::SetNewSize(int x, int y)
{
}
