#pragma once
#include "ControlForm.h"
#include "CreateWindowArgs.h"
#include "DirectionOfCells.h"
#include <CommCtrl.h>
namespace DeclarativeClasses
{
class ATable : public ControlForm
{
#pragma region Constructors and destructors
#pragma region Base constructors

public:
#pragma region Public
	ATable(UINT cols, UINT rows);
	ATable(UINT cols, UINT rows,int w,int h);
	ATable(UINT cols, UINT rows, HANDLER_CONTAINER handlers);
	ATable(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers);
	ATable(UINT cols, UINT rows, std::function<BOOL(int, int, void*)>& function);
	ATable(UINT cols, UINT rows, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	ATable(UINT cols, UINT rows, int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	ATable(UINT cols, UINT rows, Form& other);
	ATable(UINT cols, UINT rows, ControlForm& other);
	ATable(UINT cols, UINT rows, ATable& other);

#pragma endregion


protected:
	ATable() :ControlForm(){}
	ATable(int w, int h);
	ATable(HANDLER_CONTAINER handlers);
	ATable(int w, int h, HANDLER_CONTAINER handlers);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	ATable(std::function<BOOL(int, int, void*)>& function);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	ATable(HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);
	/// <summary>
	/// </summary>
	/// <param name="function">
	/// Функция для управления размерами объектов
	/// </param>
	ATable(int w, int h, HANDLER_CONTAINER handlers, std::function<BOOL(int, int, void*)>& function);

	ATable(Form& other);
	ATable(ControlForm& other);
	ATable(ATable& other);

#pragma endregion
public:
	~ATable()=default;
#pragma endregion
public:
	virtual LRESULT CALLBACK Proc(HWND, UINT, WPARAM, LPARAM)=0;
	
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		DeclarativeClasses::ATable* pThis = NULL;

		if (msg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DeclarativeClasses::ATable*)pCreate->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->SetWindowHandler(hWnd);
		}
		else
		{
			pThis = (DeclarativeClasses::ATable*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
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

	virtual void CreateSelf(const WNDCLASSEXW* wClass,const CreateWindowArgs& args)=0;
	virtual void CreateSelf(const CreateWindowArgs& args)=0;

	const HWND& GetWindowHandler();
	void SetWindowHandler(HWND hwnd);
	BOOL SetHeaders(const std::vector<std::string>& headers);
	BOOL SetHeaders(const std::list<std::string>& headers);

	void SetColumnsAndRows(UINT cols, UINT rows);

protected:
	NUMBERED_HANDLER_CONTAINER cells;
	HWND _thisWindow{};

	UINT _columns =1;
	UINT _rows=1;
	INT _selectedEdit = -1;

	bool isInitilized = false;



	inline bool CellIsLeft(UINT& id) const;
	inline bool CellIsRight(UINT& id) const;
	inline bool CellIsTop(UINT& id) const;
	inline bool CellIsBottom(UINT& id);

	std::pair<UINT, UINT> IdToPair(UINT id) const;
	void IdToPair(UINT id,UINT* row,UINT* col) const;
	UINT PairToId(UINT row, UINT col) const;
	void SortByEnteredCell(UINT id,HWND cell);

	void ResetFocus(UINT id, Direction direction);
	void KillCellsFocus();
	static LRESULT CALLBACK EditProc
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		ATable* tablePtr =reinterpret_cast<ATable*>(dwRefData);

		if (msg == WM_KEYDOWN)
		{
			if(
				wParam == VK_RETURN or
				wParam == VK_TAB
			)
			{
				tablePtr->SortByEnteredCell((UINT)uIdSubclass, hWnd);
				tablePtr->KillCellsFocus();

				if((wParam == VK_RETURN and ((GetKeyState(VK_CONTROL) & 0x8000) or (GetKeyState(VK_SHIFT) & 0x8000))) or
					wParam == VK_TAB)
				tablePtr->ResetFocus
				((UINT)uIdSubclass, wParam == VK_RETURN ? DeclarativeClasses::Down : DeclarativeClasses::Right);

				return 0;
			}

			if ((GetKeyState(VK_CONTROL) & 0x8000) or (GetKeyState(VK_SHIFT) & 0x8000))
			{
				if (
					wParam == VK_LEFT or
					wParam == VK_UP or
					wParam == VK_RIGHT or
					wParam == VK_DOWN
				)
				{
					tablePtr->SortByEnteredCell((UINT)uIdSubclass, hWnd);
					tablePtr->KillCellsFocus();
					tablePtr->ResetFocus((UINT)uIdSubclass, (DeclarativeClasses::Direction)(wParam - 0x24));
					
					return 0;
				}
			}
		}

		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}
};
}


