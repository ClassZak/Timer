#include "ControlForm.h"
namespace DeclarativeClasses
{
#pragma region Constructors and destructors
DeclarativeClasses::ControlForm::ControlForm()
	: Form::Form()
{
}

DeclarativeClasses::ControlForm::ControlForm(int w, int h)
	: Form::Form(w,h)
{
}

DeclarativeClasses::ControlForm::ControlForm(HANDLER_CONTAINER handlers)
	: Form::Form(handlers)
{
}

DeclarativeClasses::ControlForm::ControlForm(int w, int h, HANDLER_CONTAINER handlers)
	: Form::Form(w,h, handlers)
{
}

DeclarativeClasses::ControlForm::ControlForm(std::function<BOOL(int, int, void*)>& function)
	: Form::Form(function)
{
}

DeclarativeClasses::ControlForm::ControlForm(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function)
	: Form::Form(handlers, function)
{
}

DeclarativeClasses::ControlForm::ControlForm
(
	int w, int h,
	HANDLER_CONTAINER handles,
	std::function<BOOL(int, int, void*)>& function
)
	: Form::Form(w,h,handles,function)
{
}

DeclarativeClasses::ControlForm::ControlForm(Form& other)
	: Form::Form(other)
{
}
ControlForm::ControlForm(ControlForm& other)
	: Form::Form((Form&)other)
{
}
#pragma endregion
LRESULT ControlForm::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return _messageHandler(hWnd, message, wParam, lParam,Form::GetHandlers());
}
HWND ControlForm::GetItem(std::string group, std::string name)
{
	try
	{
		return std::find_if
		(
			Form::GetHandlers().at(group).begin(),
			Form::GetHandlers().at(group).end(),
			[&name](const std::pair<std::string,HWND>& pair)->bool
			{
				return pair.first == name;
			}
		)->second;
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
}
}

