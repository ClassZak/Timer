#include "Form.h"
namespace DeclarativeClasses
{
#pragma region Constructors
Form::Form()
{

}

Form::Form(int w, int h) : Form::Form()
{
	width_ = w;
	height_ = h;
}

Form::Form(HANDLER_CONTAINER handlers)
{
	handlers_ = handlers;
}

Form::Form(int w, int h, HANDLER_CONTAINER handlers) : Form::Form(w, h)
{
	handlers_ = handlers;
}

Form::Form(std::function<BOOL(int, int, void*)>& function)
{
	resizeFunction_ = function;
}

Form::Form(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: Form::Form(function)
{
	handlers_ = handlers;
}

Form::Form
(
	int w, int h,
	HANDLER_CONTAINER handles,
	std::function<BOOL(int, int, void*)>& function
) : Form::Form(w, h, handles)
{
	resizeFunction_ = function;
}

Form::Form(Form& other)
	: Form::Form(other.width_, other.height_, other.handlers_, other.resizeFunction_)
{
}
#pragma endregion
#pragma region Destructors
Form::~Form()
{
}
#pragma endregion
#pragma region Methods
void Form::SetResizeMethod(std::function<BOOL(int, int, void*)>& function)
{
	resizeFunction_ = function;
}

void Form::Resize()
{
	resizeFunction_(width_, height_, &handlers_);
}

void Form::SetNewSize(int w, int h)
{
	width_ = w;
	height_ = h;
}
#pragma endregion
}

