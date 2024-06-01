#pragma once
#include "Form.h"
namespace DeclarativeClasses
{
/// <summary>
/// Класс с обработкой событий окна
/// </summary>
class ControlForm :
    public Form
{
public:
#pragma region Constructors and destructors
#pragma region Base constructors
	ControlForm();
	ControlForm(int w, int h);
	ControlForm(HANDLER_CONTAINER handlers);
	ControlForm(int w, int h, HANDLER_CONTAINER handlers);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	ControlForm(std::function<BOOL(int, int, void*)>& function);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	ControlForm(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	ControlForm(int w, int h, HANDLER_CONTAINER handles, std::function<BOOL(int, int, void*)>& function);

	ControlForm(Form& other);
	ControlForm(ControlForm& other);
#pragma endregion




	~ControlForm();
#pragma endregion
	LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	std::function<LRESULT(HWND, UINT, WPARAM, LPARAM,const HANDLER_CONTAINER&)> _messageHandler =
	[](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, const HANDLER_CONTAINER& handlers)->LRESULT
	{
		return DefWindowProcW(hWnd, message, wParam, lParam);
	};
	
};
}


