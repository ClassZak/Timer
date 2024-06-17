#pragma once
#include <Windows.h>
#include <list>
#include <map>
#include <string>
#include <functional>
#include "AForm.h"
#include "ResizeFunctions.h"
#include "Types.h"


namespace DeclarativeClasses
{
/// <summary>
/// Класс для управления размерами объектов
/// </summary>
class Form : AForm
{
public:
#pragma region Constructors and destructors
	Form();
	Form(int w, int h);
	Form(HANDLER_CONTAINER handlers);
	Form(int w, int h, HANDLER_CONTAINER handlers);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	Form(const std::function<BOOL(int, int, void*)>& function);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	Form(HANDLER_CONTAINER handlers,const std::function<BOOL(int, int, void*)>& function);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	Form(int w, int h, HANDLER_CONTAINER handles,const std::function<BOOL(int, int, void*)>& function);
	Form(Form& other);


	~Form();
#pragma endregion
	void SetResizeMethod(const std::function<BOOL(int,int, void*)>& function)override;
	void Resize()override;
	void SetNewSize(int w, int h)override;

	bool AddItem(std::string group,std::string name, const HWND* handle);
	bool RemoveItem(std::string group,std::string name);
	void Clear();
private:
	HANDLER_CONTAINER _handlers{};
	
protected:
	/// <summary>
	/// Функция для управления размерами объектов
	/// </summary>
	std::function<BOOL(int, int, void*)> _resizeFunction =
	[](int w,int h,void* handlers)->BOOL
	{
		return EXIT_FAILURE;
	};
	HANDLER_CONTAINER& GetHandlers();

	int _width = 0, _height = 0;
	int _x = 0; int _y = 0;
};
}



