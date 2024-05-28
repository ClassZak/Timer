// Timer.cpp : Определяет точку входа для приложения.
//
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "Timer.h"
#include <math.h>
#include <string>
#include "Circle.h"
#include "Clock.h"
#include <ctime>


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);








Clock clockObj(100, { 100+3,100 +3});
Clock clockObj2(100, { 100 + 3,100 + 3 });








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
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIMER));

    MSG msg;

    DialogBox(hInstance, MAKEINTRESOURCE(IDD_CHILD_DIALOG), NULL, ChildDialogProc);
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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIMER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TIMER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

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
      CW_USEDEFAULT, 0, 600, 400, nullptr, nullptr, hInstance, nullptr);

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
        }
        break;
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
        }
        break;
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







LRESULT CALLBACK ChildDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        // Инициализация элементов управления
        return TRUE;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        RECT windowRect;
        GetClientRect(hWnd, &windowRect);
        HDC hdc = BeginPaint(hWnd, &ps);


        //FillRect(hdc, &windowRect, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));


        clockObj2.SetWindowHandle(&hWnd);
        clockObj2.StartUpdateThread();
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBM =
            CreateCompatibleBitmap(hdc,
                clockObj2.GetRect().right - clockObj2.GetRect().left + 6,
                clockObj2.GetRect().bottom - clockObj2.GetRect().top + 6);
        SelectObject(memDC, memBM);

        // Рисуем на внутреннем буфере
        clockObj2.Draw(&memDC);

        // Копируем внутренний буфер на экран
        BitBlt(hdc, 15, 15,
            clockObj2.GetRect().right - clockObj2.GetRect().left + 6,
            clockObj2.GetRect().bottom - clockObj2.GetRect().top + 6, memDC, 0, 0, SRCCOPY);

        // Освобождаем ресурсы
        DeleteObject(memBM);
        DeleteDC(memDC);


        EndPaint(hWnd, &ps);
        break;
    }
        
    case WM_COMMAND:
    {
        int buttonWindowID = LOWORD(wParam);
        switch (buttonWindowID)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDC_ADD_BUTTON:
        {
            MessageBox(hWnd, L"Функционал ещё не добавлен", L"Ошибка", MB_OK | MB_ICONERROR);
            break;
        }
        default:
            break;
        }
        return 0;
        break;
    }
        
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return 0;
    }
    return FALSE;
}