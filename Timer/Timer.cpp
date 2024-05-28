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
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);








Clock clockObj(100, { 100+3,100 +3});
BOOL AppIsRun = TRUE;








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

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIMER));
    DialogBoxParamW(NULL, MAKEINTRESOURCE(IDD_CHILD_DIALOG), NULL, WndProc,0L);
    // Цикл основного сообщения:
    while (AppIsRun);

    return EXIT_SUCCESS;
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







LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
    {
        int baseunitX, baseunitY;
        HDC hdc = GetDC(NULL);
        baseunitX = LOWORD(GetDialogBaseUnits());
        baseunitY = HIWORD(GetDialogBaseUnits());
        int cx = MulDiv(460, baseunitX, 4);
        int cy = MulDiv(200, baseunitY, 8);
        ReleaseDC(NULL, hdc);
        SetWindowPos(hWnd, NULL, 500, 5000, cx,cy, SWP_NOZORDER | SWP_NOMOVE);
        // Инициализация элементов управления
        return TRUE;
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        RECT windowRect;
        GetClientRect(hWnd, &windowRect);
        HDC hdc = BeginPaint(hWnd, &ps);


        //FillRect(hdc, &windowRect, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));


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
        break;
    case WM_DESTROY:
        AppIsRun = FALSE;
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return FALSE;
}