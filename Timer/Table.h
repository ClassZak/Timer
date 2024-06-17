#pragma once
#include "ControlForm.h"
#include "CreateWindowArgs.h"
namespace DeclarativeClasses
{
class Table : public ControlForm
{
#pragma region Constructors and destructors
#pragma region Base constructors

public:
#pragma region Public
	Table(UINT cols, UINT rows);
	Table(UINT cols, UINT rows,int w,int h);
	Table(UINT cols, UINT rows, HANDLER_CONTAINER handlers);
	Table(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers);
	Table(UINT cols, UINT rows, std::function<BOOL(int, int, void*)>& function);
	Table(UINT cols, UINT rows, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	Table(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	Table(UINT cols, UINT rows, Form& other);
	Table(UINT cols, UINT rows, ControlForm& other);
	Table(UINT cols, UINT rows, Table& other);

#pragma endregion


protected:
	Table() :ControlForm(){}
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
	Table(int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);

	Table(Form& other);
	Table(ControlForm& other);
	Table(Table& other);

#pragma endregion
public:
	~Table()=default;
#pragma endregion
public:
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
	BOOL SetHeaders(const std::vector<std::string>& headers);
	BOOL SetHeaders(const std::list<std::string>& headers);

	void SetColumnsAndRows(UINT cols, UINT rows);

protected:
	HWND _thisWindow{};

	UINT _columns =1;
	UINT _rows=1;

	int selectedEdit=-1;
	bool isInitilized = false;
};
}


