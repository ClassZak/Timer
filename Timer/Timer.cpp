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








Clock clockObj(100, { 100+3,100 +3});
DeclarativeClasses::Form form;

RECT windowRect;








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
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
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


        HWND button = CreateWindowExA
        (
            0L,
            "button",
            "Добавить",
            WS_VISIBLE | WS_CHILD | ES_CENTER | BS_PUSHBUTTON,
            0, windowRect.bottom-110,
            60, 110,
            hWnd, NULL, NULL, NULL
        );
        LOGFONT lf;
        lf.lfHeight = 10;
        SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
        HFONT hFont = CreateFontIndirect(&lf);
        SendMessage(button, WM_SETFONT, (WPARAM)hFont, TRUE);








        HWND hWndListView1 = CreateWindowEx
        (
            0,
            WC_LISTVIEW,
            NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT,
            60, windowRect.bottom - 110, windowRect.right / 2-60, 110,
            hWnd,
            (HMENU)1,
            hInst,
            NULL
        );

        // Создаем вторую ListView
        HWND hWndListView2 = CreateWindowEx
        (
            0,
            WC_LISTVIEW,
            NULL,
            WS_CHILD | WS_VISIBLE | LVS_REPORT,
            windowRect.right/2,0, windowRect.right / 2, windowRect.bottom,
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
        lvc.pszText =(wchar_t*) L"Column 1";
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






        form.AddItem("buttons","add",&button);
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
                clockObj.GetRect().right - clockObj.GetRect().left+6, 
                clockObj.GetRect().bottom - clockObj.GetRect().top+6);
        SelectObject(memDC, memBM);

        // Рисуем на внутреннем буфере
        clockObj.Draw(&memDC);

        // Копируем внутренний буфер на экран
        BitBlt(hdc, 15, 15, 
            clockObj.GetRect().right - clockObj.GetRect().left+6, 
            clockObj.GetRect().bottom - clockObj.GetRect().top+6, memDC, 0, 0, SRCCOPY);

        // Освобождаем ресурсы
        DeleteObject(memBM);
        DeleteDC(memDC);

            
        EndPaint(hWnd, &ps);
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
