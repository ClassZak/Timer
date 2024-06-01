#include "ControlForm.h"
namespace DeclarativeClasses
{
#pragma region Constructors and destructors
#pragma region Base constructors
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
	: Form::Form(other)
{
}
#pragma endregion
DeclarativeClasses::ControlForm::~ControlForm()
{
}
#pragma endregion
LRESULT ControlForm::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return _messageHandler(hWnd, message, wParam, lParam,Form::GetHandlers());
}
}

