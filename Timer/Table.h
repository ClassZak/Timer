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
	LRESULT CALLBACK Proc(HWND, UINT, WPARAM, LPARAM);
	
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		DeclarativeClasses::Table* pThis = NULL;

		if (msg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DeclarativeClasses::Table*)pCreate->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->SetWindowHandler(hWnd);
		}
		else
		{
			pThis = (DeclarativeClasses::Table*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->Proc(pThis->GetWindowHandler(),msg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	void CreateSelf(const WNDCLASSEXW* wClass,const CreateWindowArgs& args);
	void CreateSelf(const CreateWindowArgs& args);


	const HWND& GetWindowHandler();
	void SetWindowHandler(HWND hwnd);
protected:
	HWND _thisWindow{};
	HWND _horScrollBar{};
	HWND _vertScrollBar{};

	void ResizeScrollBars();
};
}


