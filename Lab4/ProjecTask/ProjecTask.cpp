// lr2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ProjecTask.h"
#include <vector>
#include <string>
#include <algorithm>


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
    LoadStringW(hInstance, IDC_PROJECTASK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTASK));

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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_PROJECTASK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECTASK);
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

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, 700, 450, NULL, NULL, hInstance, NULL);

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

struct Speed
{
    float xSpeed;
    float ySpeed;
};
struct Point
{
    float x;
    float y;
};
const Point nullPoint{ -1, -1 };

bool operator!=(const Point& lhs, const Point& rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}


Point operator+(const Point& lhs, const Speed& rhs)
{
    return { lhs.x + rhs.xSpeed, lhs.y + rhs.ySpeed };
}

Point operator+=(Point& lhs, const Speed& rhs)
{
    return lhs = lhs + rhs;
}

class BitMapController
{
private:
    std::vector<HANDLE> hBitmap;
    BITMAP Bitmap;
    Point pos = { 10,10 };
    Point target = nullPoint;
    Speed speed;

    int currBmp = 0;
    int reverse = false;

public:

    BitMapController(int size, Speed speed, float scale)
    {
        auto shBitmap = LoadImage((HINSTANCE)NULL, TEXT("1.bmp"),
            IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
        
        
        GetObject(shBitmap, sizeof(BITMAP), &Bitmap);
        double d1 = sqrt(1.0 * Bitmap.bmWidth * Bitmap.bmWidth + 1.0 * Bitmap.bmHeight * Bitmap.bmHeight);
        double sinAlf = Bitmap.bmHeight / d1;
        double d2 = d1 * scale;
        double newHeight = d2 * sinAlf;
        double newWidth = d2 * sqrt(1 - 1.0 * sinAlf * sinAlf);
        
        hBitmap = std::vector<HANDLE>(size);

        for (size_t i = 0; i < hBitmap.size(); i++) {
            std::string name = std::to_string(i + 1) + ".bmp";

            std::wstring widestr = std::wstring(name.begin(), name.end());

            hBitmap[i] = LoadImage((HINSTANCE)NULL, widestr.c_str(),
                IMAGE_BITMAP, (int)newWidth, (int)newHeight, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
        }


       
        if (!shBitmap) {
            MessageBox(NULL, TEXT("File not found!"), TEXT("Error"), MB_OK);
        }
        this->speed = speed;
    }

    void SetStartPos(Point pos)
    {
        this->pos = {pos.x, pos.y};
    }

    void SetTarget(Point target) {
        this->target = target;
        this->target.x -= 50;
        this->target.y -= 50;
    }

    bool TryMove()
    {
        if (target == nullPoint)
            return false;

        if (abs(target.x + speed.xSpeed) > pos.x && abs(target.x - speed.xSpeed) < pos.x
            && abs(target.y + speed.ySpeed) > pos.y && abs(target.y - speed.ySpeed) < pos.y) {
            target = nullPoint;
            return false;
        }
        else
            return true;
    }

    void MoveBitMap(HDC hdc, HWND hWnd)
    {
        if (TryMove())
        {
            currBmp = (currBmp + 1) % hBitmap.size();
            pos = GetNewPos(pos, target);
        }
        DrawBitMap(hdc, pos);
    }

private:
    void DrawBitMap(HDC hdc, Point pos)
    {
        HDC hCompatibleDC;
        hCompatibleDC = CreateCompatibleDC(hdc);
        SelectObject(hCompatibleDC, hBitmap[currBmp]);
        if (reverse)
            StretchBlt(hdc, pos.x, pos.y, 100, 100, hCompatibleDC, Bitmap.bmWidth, 0, -Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
        else
            StretchBlt(hdc, pos.x, pos.y, 100, 100, hCompatibleDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
        DeleteDC(hCompatibleDC);
    }


    Point GetNewPos(Point oldpos, Point target) {
        if (abs(oldpos.x - target.x) >= speed.xSpeed) {
            if (target.x > oldpos.x) {
                oldpos.x += speed.xSpeed;
                reverse = false;
            }
            else {
                oldpos.x -= speed.xSpeed;
                reverse = true;
            }
        }
        if (abs(oldpos.y - target.y) >= speed.ySpeed) {
            if (target.y > oldpos.y)
                oldpos.y += speed.ySpeed;
            else
                oldpos.y -= speed.ySpeed;
        }
        return oldpos;
    }
}controller(16, {10, 10}, 1.0);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (message)
    {
    case WM_CREATE:
    {
        controller.SetStartPos({ 0,0 });
        InvalidateRect(hWnd, NULL, true);
        break;
    }
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        switch (wmEvent)
        {
        case BN_CLICKED:
        default:
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
    case WM_LBUTTONDOWN:
    {
        controller.SetTarget({ (float)LOWORD(lParam), (float)HIWORD(lParam) });
        SetTimer(hWnd, NULL, 100, NULL);
        break;
    }
    case WM_TIMER:
    {
        if (controller.TryMove()) {
            InvalidateRect(hWnd, NULL, true);
            SetTimer(hWnd, NULL, 100, NULL);
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        controller.MoveBitMap(hdc, hWnd);
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
