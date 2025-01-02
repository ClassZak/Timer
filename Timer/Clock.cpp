#include "Clock.h"
#include  <stdexcept>
#include <string>

#pragma region Constructors
Clock::Clock()
{
    InitTime();
}

Clock::Clock(POINT center) : Circle::Circle(center)
{
    InitTime();
}

Clock::Clock(float radius) : Circle::Circle(radius)
{
    InitTime();
}

Clock::Clock(POINT center, float radius) : Circle::Circle(center,radius)
{
    InitTime();
}

Clock::Clock(float radius, POINT center) : Circle::Circle(center, radius)
{
    InitTime();
}

Clock::Clock(const Clock& other) : Circle::Circle((Circle)other)
{
	this->rawTime = other.rawTime;
	this->TimeInfo = other.TimeInfo;
}
#pragma endregion
#pragma region Destructors
Clock::~Clock()
{
    work = FALSE;

    DeleteObject(clockBackgroundBrush);
    DeleteObject(backgroundBrush);
    DeleteObject(clockBorderPen);
    DeleteObject(hourLabelPen);
    DeleteObject(minuteLabelPen);
    DeleteObject(secondArrowPen);
}
#pragma endregion
#pragma region Updating
void Clock::StartUpdateThread()
{
    if (hWnd == nullptr)
        throw std::runtime_error("Window handle not included");

    if (!work)
    {
        RECT* rect = new RECT;
        *rect = FindSelfRect();

        UpdateArrowsArgumentsStruct* params=new UpdateArrowsArgumentsStruct;
        *params=
        {
            &rawTime,
            &TimeInfo,
            hWnd,
            rect,
            &work
        };
        CreateThread(NULL,
            1000,
            Clock::UpdateArrows,
            (void*)params,
            0,
            NULL);
        work = TRUE;
    }
}

void Clock::SetWindowHandle(HWND* hWnd)
{
    if(!work)
    this->hWnd = hWnd;
}











DWORD __stdcall Clock::UpdateArrows(void* lParam)
{
    UpdateArrowsArgumentsStruct* args = (UpdateArrowsArgumentsStruct*)(lParam);
    HWND handle = *args->hWnd;



    {
        time(args->rawTime);
        args->TimeInfo = localtime(args->rawTime);

        SYSTEMTIME st;
        GetLocalTime(&st);
        while (st.wMilliseconds>=20)
            GetLocalTime(&st);

        RedrawWindow(handle, args->rect, NULL, RDW_INVALIDATE);
    }
    

    while (*args->work)
    {
        Sleep(1000);
        time(args->rawTime);
        args->TimeInfo = localtime(args->rawTime);
        

        RedrawWindow(handle, args->rect, NULL, RDW_INVALIDATE);
        PostMessageA(handle, WM_PAINT, NULL, NULL);
    }
    delete args->rect;

    return EXIT_SUCCESS;
}
#pragma endregion
void Clock::InitTime()
{
    time(&rawTime);
    TimeInfo = *localtime(&rawTime);
}


void Clock::Draw(HDC* hdc)
{
    if (hWnd == nullptr)
        throw std::runtime_error("Window handle not included");

    InitTime();

    HGDIOBJ oldBrush = SelectObject(*hdc, backgroundBrush);
    HGDIOBJ oldPen = SelectObject(*hdc, clockBorderPen);
    
    const RECT& clockCircleRect = GetRect();
    RECT back
    { 
        clockCircleRect.left-3,
        clockCircleRect.top-3,
        clockCircleRect.right+3,
        clockCircleRect.bottom+3
    };
    FillRect(*hdc, &back, backgroundBrush);
    SelectObject(*hdc, clockBackgroundBrush);


    
    Ellipse(*hdc, clockCircleRect.left, clockCircleRect.top,
        clockCircleRect.right, clockCircleRect.bottom);

    
    SelectObject(*hdc, hourLabelPen);
    for (UCHAR i = 0; i != 12; ++i)
    {
        double angle = (!i) ? 0 : M_PI * 2. * i / 12;
        POINT p = GetRingPoint(angle);
        MoveToEx(*hdc, p.x, p.y, NULL);
        LineTo(*hdc, p.x, p.y);
        p = GetRingPoint(angle, -20);
        LineTo(*hdc, p.x, p.y);
    }
    SelectObject(*hdc, minuteLabelPen);
    for (UCHAR i = 0; i != 60; ++i)
    {
        if (!(i % 5))
            continue;
        double angle = (!i) ? 0 : M_PI * 2. * i / 60;
        POINT p = GetRingPoint(angle);
        MoveToEx(*hdc, p.x, p.y, NULL);
        LineTo(*hdc, p.x, p.y);
        p = GetRingPoint(angle, -15);
        LineTo(*hdc, p.x, p.y);
    }

    //digits
    SetBkColor(*hdc, RGB(224, 224, 224));
    {
        RECT tRect{ 0,0,0,0 };
        for (UCHAR i = 1; i <= 12; ++i)
        {
            double angle = (!i) ? 0 : M_PI * 2. * i / 12 - M_PI_2;
            POINT p = GetRingPoint(angle, -40);


            tRect.left = p.x + -10;
            tRect.top = p.y - 5;
            tRect.right = tRect.left + 15;
            tRect.bottom = tRect.top + 15;

            DrawTextA(*hdc, Clock::CLOCK_DIGITS[i-1].c_str(), 2, &tRect, NULL);
        }
    }
    SetBkColor(*hdc, RGB(0, 0, 0));

    SelectObject(*hdc, oldBrush);
    SelectObject(*hdc, clockBorderPen);
    Ellipse(*hdc,
        GetCenter().x - 3, GetCenter().y - 3,
        GetCenter().x + 3, GetCenter().y + 3);


        
    //arrows
    //hours
    SelectObject(*hdc, hourLabelPen);
    {
        POINT p1, p2;
        p1 = GetRingPoint(
            ((!(TimeInfo.tm_hour % 12)) ?
                -M_PI_2 : M_PI * 2 * (TimeInfo.tm_hour % 12) / 12 - M_PI_2) +
            M_PI / 12. / 60. * TimeInfo.tm_min,
            -GetRadius() * 5 / 8);
        p2 = GetRingPoint(
            ((!(TimeInfo.tm_hour % 12)) ?
                -M_PI_2 : M_PI * 2 * (TimeInfo.tm_hour % 12) / 12 - M_PI_2) + M_PI +
            M_PI / 12. / 60. * TimeInfo.tm_min,
            -GetRadius() * 7 / 8);
        MoveToEx(*hdc, p1.x, p1.y, NULL);
        LineTo(*hdc, p2.x, p2.y);
    }

    //minutes
    SelectObject(*hdc, minuteLabelPen);
    {
        POINT p1, p2;
        p1 = GetRingPoint(
            ((!(TimeInfo.tm_min)) ?
                -M_PI_2 : M_PI * 2 * (TimeInfo.tm_min) / 60 - M_PI_2),
            -GetRadius() * 1/3);
        p2 = GetRingPoint(
            ((!(TimeInfo.tm_min)) ?
                -M_PI_2 : M_PI * 2 * (TimeInfo.tm_min) / 60 - M_PI_2) + M_PI,
            -GetRadius() * 5/6);
        MoveToEx(*hdc, p1.x, p1.y, NULL);
        LineTo(*hdc, p2.x, p2.y);
    }

    //seconds
    SelectObject(*hdc, secondArrowPen);
    {
        POINT p1, p2;
        p1 = GetRingPoint(
            ((!(TimeInfo.tm_sec)) ?
                -M_PI_2 : M_PI * 2 * (TimeInfo.tm_sec) / 60 - M_PI_2),
            -GetRadius() * 1 / 4);
        p2 = GetRingPoint(
            ((!(TimeInfo.tm_sec)) ?
                -M_PI_2 : M_PI * 2 * (TimeInfo.tm_sec) / 60 - M_PI_2) + M_PI,
            -GetRadius() * 5 / 8);
        MoveToEx(*hdc, p1.x, p1.y, NULL);
        LineTo(*hdc, p2.x, p2.y);
    }



    SelectObject(*hdc, oldBrush);
    SelectObject(*hdc, oldPen);
}





const std::string Clock::CLOCK_DIGITS[12] =
{
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "11",
    "12",
};