#pragma once
#include "ControlForm.h"
#include "CreateWindowArgs.h"
namespace DeclarativeClasses
{
class Table : public ControlForm
{
public:
#pragma region Constructors and destructors
#pragma region Base constructors
	Table() : ControlForm()
	{
	}
	Table(int w, int h);
	Table(HANDLER_CONTAINER handlers);
	Table(int w, int h, HANDLER_CONTAINER handlers);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	Table(std::function<BOOL(int, int, void*)>& function);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	Table(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	Table(int w, int h, HANDLER_CONTAINER handles, std::function<BOOL(int, int, void*)>& function);

	Table(Form& other);
	Table(ControlForm& other);
	Table(Table& other);
#pragma endregion
	~Table()=default;
#pragma endregion
	static LRESULT CALLBACK Proc(HWND, UINT, WPARAM, LPARAM);

	void CreateSelf(const WNDCLASSEXW* wClass,const CreateWindowArgs& args);
	void CreateSelf(const CreateWindowArgs& args);


	const HWND& GetWindow();
protected:
	HWND _thisWindow{};
};
}


