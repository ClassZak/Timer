// Timer.cpp : Определяет точку входа для приложения.
//
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#pragma comment(lib, "comctl32.lib")
#include "Timer.h"
#include <math.h>
#include <stdexcept>
#include <string>
#include "Circle.h"
#include "Clock.h"
#include "Form.h"
#include "ControlForm.h"
#include <ctime>


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ListViewSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR IdSubclass, DWORD_PTR RefData);
LRESULT CALLBACK    TempListViewEditorProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR IdSubclass, DWORD_PTR RefData);


std::wstring GetAllListViewItems(HWND hwndListView);





Clock clockObj(100, { 100 + 3,100 + 3 });
DeclarativeClasses::ControlForm form;

RECT windowRect;
HWND buttonTableEdit;


#pragma region Table elements
CONST HMENU TEMP_BUTTON_ID = (HMENU)5000;
CONST HMENU LISTVIEW_LEFT_ID = (HMENU)2000;
BOOL TempEditorEnabled = FALSE;
#pragma endregion







int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TIMER, szWindowClass, MAX_LOADSTRING);

    WNDCLASSEXW wcex{ NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIMER_ICON));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TIMER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_TIMER_ICON));

    RegisterClassExW(&wcex);


    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIMER));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, IDS_DEFAULT_WINDOW_WIDTH, IDS_DEFAULT_WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &windowRect);






        // Инициализация ListView
        HWND hWndListView = CreateWindow(WC_LISTVIEW,
            L"",
            WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_VISIBLE,
            0, 0, 100, 100,
            hWnd,
            (HMENU)3,
            hInst,
            NULL);

        // Добавление столбцов
        LVCOLUMN lvColumn;
        lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        lvColumn.cx = 100; // ширина столбца

        // Заголовки столбцов
        const wchar_t* columnTitles[4] = { L"Заголовок 1", L"Заголовок 2", L"Заголовок 3", L"Заголовок 4" };
        for (int i = 0; i < 4; ++i) {
            lvColumn.pszText = (LPWSTR)columnTitles[i];
            ListView_InsertColumn(hWndListView, i, &lvColumn);
        }

        // Добавление элементов и подпредметов
        LVITEM lvItem;
        lvItem.mask = LVIF_TEXT;
        lvItem.iSubItem = 0;
        lvItem.iItem = 0;
        lvItem.pszText = (wchar_t*)L"Элемент 1";
        ListView_InsertItem(hWndListView, &lvItem);

        ListView_SetItemText(hWndListView, 0, 1, (wchar_t*)L"Подпредмет 1");
        ListView_SetItemText(hWndListView, 0, 2, (wchar_t*)L"Подпредмет 2");
        ListView_SetItemText(hWndListView, 0, 3, (wchar_t*)L"Подпредмет 3");
        LVITEM lvItem2;
        lvItem2.mask = LVIF_TEXT;
        lvItem2.iItem = 1; // индекс строки
        lvItem2.iSubItem = 0; // индекс подпредмета
        lvItem2.pszText = (wchar_t*)L"Новый элемент";
        ListView_InsertItem(hWndListView, &lvItem2);

        // Добавление подпредметов для новой строки
        ListView_SetItemText(hWndListView, 1, 1, (wchar_t*)L"Подпредмет 1");
        ListView_SetItemText(hWndListView, 1, 2, (wchar_t*)L"Подпредмет 2");
        ListView_SetItemText(hWndListView, 1, 3, (wchar_t*)L"Подпредмет 3");

        // Делаем первую ячейку редактируемой
        ListView_EditLabel(hWndListView, 1);














        HWND button = CreateWindowExA
        (
            0L,
            "button",
            "Добавить",
            WS_VISIBLE | WS_CHILD | ES_CENTER | BS_PUSHBUTTON,
            0, windowRect.bottom - 110,
            60, 110,
            hWnd, (HMENU)IDS_ADD_BUTTON, NULL, NULL
        );
        LOGFONT lf;
        lf.lfHeight = 10;
        SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
        HFONT hFont = CreateFontIndirect(&lf);
        SendMessage(button, WM_SETFONT, (WPARAM)hFont, TRUE);








        HWND hWndListView1 = CreateWindowEx
        (
            WS_EX_CLIENTEDGE,
            WC_LISTVIEW,
            NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            60, windowRect.bottom - 110, windowRect.right / 2 - 60, 110,
            hWnd,
            (HMENU)1,
            hInst,
            NULL
        );

        // Создаем вторую ListView
        HWND hWndListView2 = CreateWindowEx
        (
            WS_EX_CLIENTEDGE,
            WC_LISTVIEW,
            NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
            windowRect.right / 2, 0, windowRect.right / 2, windowRect.bottom,
            hWnd,
            (HMENU)2,
            hInst,
            NULL
        );

        // Инициализация шрифтов и других параметров для ListView
        LOGFONT lf2;
        lf2.lfHeight = 10;
        SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf2, 0);
        HFONT hFont2 = CreateFontIndirect(&lf2);
        SendMessage(hWndListView1, WM_SETFONT, (WPARAM)hFont2, TRUE);
        SendMessage(hWndListView2, WM_SETFONT, (WPARAM)hFont2, TRUE);

        // Добавляем столбцы в первую ListView
        LVCOLUMN lvc;
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        lvc.cx = 100;
        lvc.pszText = (wchar_t*)L"Column 1";
        ListView_InsertColumn(hWndListView1, 0, &lvc);
        lvc.pszText = (wchar_t*)L"Column 2";
        ListView_InsertColumn(hWndListView1, 1, &lvc);

        // Добавляем столбцы во вторую ListView
        lvc.pszText = (wchar_t*)L"Column A";
        ListView_InsertColumn(hWndListView2, 0, &lvc);
        lvc.pszText = (wchar_t*)L"Column B";
        ListView_InsertColumn(hWndListView2, 1, &lvc);

        // Пример добавления элементов в первую ListView
        LVITEM lvi;
        lvi.mask = LVIF_TEXT;
        lvi.iItem = 0;
        lvi.iSubItem = 0;
        lvi.pszText = (wchar_t*)L"Item 1";
        ListView_InsertItem(hWndListView1, &lvi);
        lvi.iSubItem = 1;
        lvi.pszText = (wchar_t*)L"SubItem 1.1";
        ListView_SetItem(hWndListView1, &lvi);

        // Пример добавления элементов во вторую ListView
        lvi.iItem = 0;
        lvi.iSubItem = 0;
        lvi.pszText = (wchar_t*)L"Item A";
        ListView_InsertItem(hWndListView2, &lvi);
        lvi.iSubItem = 1;
        lvi.pszText = (wchar_t*)L"SubItem A.1";
        ListView_SetItem(hWndListView2, &lvi);



        ListView_SetOutlineColor(hWndListView1, RGB(255, 0, 0));
        ListView_SetOutlineColor(hWndListView2, RGB(255, 0, 0));





        form.AddItem("buttons", "add", &button);
        form.AddItem("listViews", "addListView", &hWndListView1);
        form.AddItem("listViews", "showListView", &hWndListView2);
        form.SetNewSize(windowRect.right, windowRect.bottom);
        form.SetResizeMethod(DeclarativeClasses::Functions::ResizeFunctions::L1);
        try
        {
            form.Resize();
        }
        catch (std::exception&)
        {
            MessageBoxExW(hWnd, L"Ошибка функции изменения размера", L"Ошибка выполнения", MB_ICONERROR, NULL);
            DestroyWindow(hWnd);
        }



        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDS_ADD_BUTTON:
        {
            std::wstring items = GetAllListViewItems(form.GetItem("listViews", "addListView"));
            MessageBoxW(hWnd, items.c_str(), L"Items in ListView 1", MB_OK);
            break;
        }
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_SIZE:
    {
        try
        {
            GetClientRect(hWnd, &windowRect);
            form.SetNewSize(windowRect.right, windowRect.bottom);
            form.Resize();
        }
        catch (std::exception&)
        {
            MessageBoxExW(hWnd, L"Ошибка функции изменения размера", L"Ошибка выполнения", MB_ICONERROR, NULL);
            DestroyWindow(hWnd);
        }
        break;
    }
    case WM_GETMINMAXINFO: //Получили сообщение от Винды
    {
        MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
        POINT Min = { 600, 400 };
        pInfo->ptMinTrackSize = Min;
        return 0;
        break;
    }
    case WM_KEYDOWN:
    {
        /*
        if (wParam == VK_RETURN)
        {
            // Получаем текст из Edit
            wchar_t szText[256];
            GetWindowText(hWnd, szText, 256);

            // Получаем индекс выбранного элемента и подпредмета
            int iItem = ...; // Здесь должен быть код для получения индекса элемента
            int iSubItem = ...; // Здесь должен быть код для получения индекса подпредмета

            // Обновляем текст в ListView
            ListView_SetItemText(GetParent(hWnd), iItem, iSubItem, szText);

            // Закрываем Edit
            DestroyWindow(hWnd);
        }
        else if (wParam == VK_ESCAPE)
        {
            // Закрываем Edit без сохранения изменений
            DestroyWindow(hWnd);
        }
        break;*/
    }
   
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        RECT windowRect;
        GetClientRect(hWnd, &windowRect);
        HDC hdc = BeginPaint(hWnd, &ps);


        clockObj.SetWindowHandle(&hWnd);
        clockObj.StartUpdateThread();
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBM =
            CreateCompatibleBitmap(hdc,
                clockObj.GetRect().right - clockObj.GetRect().left + 6,
                clockObj.GetRect().bottom - clockObj.GetRect().top + 6);
        SelectObject(memDC, memBM);

        // Рисуем на внутреннем буфере
        clockObj.Draw(&memDC);

        // Копируем внутренний буфер на экран
        BitBlt(hdc, 15, 15,
            clockObj.GetRect().right - clockObj.GetRect().left + 6,
            clockObj.GetRect().bottom - clockObj.GetRect().top + 6, memDC, 0, 0, SRCCOPY);

        // Освобождаем ресурсы
        DeleteObject(memBM);
        DeleteDC(memDC);


        EndPaint(hWnd, &ps);
        break;
    }
    case WM_NOTIFY:
    {
        LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
        if (lpnmitem->hdr.code == NM_DBLCLK)
        {
            // Получаем координаты ячейки
            LVHITTESTINFO hitTestInfo = { 0 };
            hitTestInfo.pt = lpnmitem->ptAction;
            ListView_SubItemHitTest(lpnmitem->hdr.hwndFrom, &hitTestInfo);

            // Создаем элемент управления Edit
            RECT rcSubItem;
            ListView_GetSubItemRect(lpnmitem->hdr.hwndFrom, hitTestInfo.iItem, hitTestInfo.iSubItem, LVIR_BOUNDS, &rcSubItem);
            HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                rcSubItem.left, rcSubItem.top, rcSubItem.right - rcSubItem.left, rcSubItem.bottom - rcSubItem.top,
                lpnmitem->hdr.hwndFrom, (HMENU)45, GetModuleHandle(NULL), NULL);

            // Настраиваем и показываем элемент управления Edit
            SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
            SetFocus(hEdit);

            // Обработка завершения редактирования
            // ...
        }
        break;
    }
    


    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}








std::wstring GetAllListViewItems(HWND hwndListView)
{
    int itemCount = ListView_GetItemCount(hwndListView);
    int columnCount = Header_GetItemCount(ListView_GetHeader(hwndListView));
    std::wstring allItems;
    wchar_t itemText[256];

    for (int i = 0; i < itemCount; ++i)
    {
        for (int j = 0; j < columnCount; ++j)
        {
            itemText[0] = L'\0';
            ListView_GetItemText(hwndListView, i, j, itemText, 256);
            allItems += itemText;
            allItems += L"\t"; // Добавляем символ табуляции между элементами колонки
        }
        allItems += L"\n"; // Добавляем символ новой строки после каждой строки
    }
    return allItems;
}



#pragma region Subitems' processes
LRESULT ListViewSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR IdSubclass, DWORD_PTR RefData)
{
    switch(message)
    {
        case WM_HSCROLL:
        case WM_VSCROLL:
        {
            if (GetFocus() == GetDlgItem(GetParent(hWnd), 5000))
                SetFocus(hWnd);
            break;
        }
        case WM_NCDESTROY:
        {
            ::RemoveWindowSubclass(hWnd, ListViewSubclassProc, IdSubclass);
            break;
        }
    }

    return DefSubclassProc(hWnd,message,wParam,lParam);
}
LRESULT TempListViewEditorProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR IdSubclass, DWORD_PTR RefData)
{
    switch (message)
    {
        case WM_GETDLGCODE:
        {
            return (DLGC_WANTALLKEYS | DefSubclassProc(hWnd, message, wParam, lParam));
        }
        case WM_KILLFOCUS:
        {
            TempEditorEnabled = FALSE;
            ShowWindow(hWnd, SW_HIDE);
            break;
        }
        case WM_CHAR:
        {
            switch (wParam)
            {
                case VK_RETURN:
                case VK_ESCAPE:
                {
                    return 0L;
                }
            }
            break;
        }
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_RETURN:
                {
                    HWND hWndLV = GetDlgItem(GetParent(hWnd), (int)LISTVIEW_LEFT_ID);

                    RECT rc{0,0,0,0};
                    GetClientRect(hWnd, &rc);

                    MapWindowPoints(hWnd, hWndLV, (LPPOINT)&rc, (sizeof(RECT) / sizeof(POINT)));
                    LVHITTESTINFO lvhti{};
                    lvhti.pt.x = rc.left;
                    lvhti.pt.y = rc.top;

                    //List
                }
            }
        }
    }

    return DefSubclassProc(hWnd, message, wParam, lParam);
}
#pragma endregion
