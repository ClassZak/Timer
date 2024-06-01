#include "Form.h"
namespace DeclarativeClasses
{
#pragma region Constructors
Form::Form()
{

}

Form::Form(int w, int h) : Form::Form()
{
	_width = w;
	_height = h;
}

Form::Form(HANDLER_CONTAINER handlers)
{
	_handlers = handlers;
}

Form::Form(int w, int h, HANDLER_CONTAINER handlers) : Form::Form(w, h)
{
	_handlers = handlers;
}

Form::Form(const std::function<BOOL(int, int, void*)>& function)
{
	_resizeFunction = function;
}

Form::Form(HANDLER_CONTAINER handlers,const std::function<BOOL(int, int, void*)>& function)
	: Form::Form(function)
{
	_handlers = handlers;
}

Form::Form
(
	int w, int h,
	HANDLER_CONTAINER handles,
	const std::function<BOOL(int, int, void*)>& function
) : Form::Form(w, h, handles)
{
	_resizeFunction = function;
}

Form::Form(Form& other)
	: Form::Form(other._width, other._height, other._handlers, other._resizeFunction)
{
}
#pragma endregion
#pragma region Destructors
Form::~Form()
{
}
#pragma endregion
#pragma region Methods
void Form::SetResizeMethod(const std::function<BOOL(int, int, void*)>& function) 
{
	_resizeFunction = function;
}

void Form::Resize()
{
	_resizeFunction(_width, _height, &_handlers);
}

void Form::SetNewSize(int w, int h)
{
	_width = w;
	_height = h;
}
bool Form::AddItem(std::string group, std::string name, const HWND* handle)
{
	try
	{
		_handlers.at(group).push_back(std::pair<std::string, HWND>(name, *handle));
		return EXIT_SUCCESS;
	}
	catch (const std::exception&)
	{
		return EXIT_FAILURE;
	}
}
bool Form::RemoveItem(std::string group, std::string name)
{
	try
	{
		_handlers.at(group)
			.erase
			(
				std::find_if
				(
					_handlers.at(group).begin(),
					_handlers.at(group).end(),
					[&name](std::pair<std::string, HWND> pair)-> bool
					{
						return pair.first == name;
					}
				)
			);
		return EXIT_SUCCESS;
	}
	catch(const std::exception& ex)
	{
		return EXIT_FAILURE;
	}
}
void Form::Clear()
{
	_handlers.clear();
}
const HANDLER_CONTAINER& Form::GetHandlers()const
{
	return _handlers;
}
#pragma endregion
}

