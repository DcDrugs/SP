// PicRunner.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ProjecTask.h"
#include <memory>
#include <WinUser.h>

#define MAX_LOADSTRING 100

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


Speed speedPic = { 3.0, 3.0 }; // скорость движения
float scalePic = 0.2;    // размер картинки в % от оригинала
LPCTSTR namePic = TEXT("Pic.bmp");





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
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTASK));
    wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR2));
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECTASK);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void operator+=(Point& lhs, const Speed& rhs)
{
    lhs.x += rhs.xSpeed;
    lhs.y += rhs.ySpeed;
}

class BitMapController
{
private:
    bool isLocated = false;
    HANDLE hBitmap;
    HDC hCompatibleDC;
    BITMAP Bitmap;
    Point pos = { 10,10 };
    Speed speed;
public:

    BitMapController(LPCTSTR name, Speed speed, float scale)
    {
        hCompatibleDC = NULL;

        hBitmap = (HBITMAP)LoadImage((HINSTANCE)NULL, name,
            IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
        GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
        double d1 = sqrt(1.0 * Bitmap.bmWidth * Bitmap.bmWidth + 1.0 * Bitmap.bmHeight * Bitmap.bmHeight);
        double sinAlf = Bitmap.bmHeight / d1;
        double d2 = d1 * scale;
        double newHeight = d2 * sinAlf;
        double newWidth = d2 * sqrt(1 - 1.0 * sinAlf * sinAlf);
        hBitmap = LoadImage((HINSTANCE)NULL, name, IMAGE_BITMAP, (int)newWidth, (int)newHeight, LR_LOADFROMFILE);
        GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
        if (!hBitmap) {
            MessageBox(NULL, TEXT("File not found!"), TEXT("Error"), MB_OK);
        }
        this->speed = speed;
    }

    void SetStartPos(Point pos)
    {
        if (!isLocated)
            this->pos = { pos.x - Bitmap.bmWidth / 2, pos.y - Bitmap.bmHeight / 2 };
    }

    bool CanDraw()
    {
        return isLocated;
    }

    bool PlaceBitmap(HDC hdc)
    {
        if (isLocated)
            return false;
        hCompatibleDC = CreateCompatibleDC(hdc);
        SelectObject(hCompatibleDC, hBitmap);
        isLocated = true;
        return isLocated;
    }

    bool ClearBitMap()
    {
        if (!isLocated)
            return false;
        DeleteDC(hCompatibleDC);
        isLocated = false;
        return isLocated;
    }

    bool MoveBitMap(HDC hdc, HWND hWnd)
    {
        if (!isLocated)
            return false;
        DrawBitMap(hdc, pos, SRCCOPY);
        speed = GetNewSpeed(hWnd, speed);
        pos += speed;
        return true;
    }

    void DrawBitMap(HDC hdc)
    {
        DrawBitMap(hdc, pos, SRCCOPY);
    }

private:
    void DrawBitMap(HDC hdc, Point pos, DWORD rop)
    {
        BitBlt(hdc, (int)pos.x, (int)pos.y, Bitmap.bmWidth, Bitmap.bmHeight, hCompatibleDC, 0, 0, rop);
    }

    Speed GetNewSpeed(HWND hWnd, Speed lastSpeed)
    {
        auto firstPoint = pos;
        Point lastPoint = { pos.x + Bitmap.bmWidth, pos.y + Bitmap.bmHeight };
        RECT Rect;
        GetClientRect(hWnd, &Rect);
        if (Rect.bottom <= lastPoint.y && lastSpeed.ySpeed > 0)
            lastSpeed.ySpeed *= -1;
        if (firstPoint.y <= Rect.left && lastSpeed.ySpeed < 0)
            lastSpeed.ySpeed *= -1;
        if (Rect.right <= lastPoint.x && lastSpeed.xSpeed > 0)
            lastSpeed.xSpeed *= -1;
        if (firstPoint.x <= Rect.top && lastSpeed.xSpeed < 0)
            lastSpeed.xSpeed *= -1;
        return lastSpeed;
    }


};

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
std::unique_ptr<BitMapController> Pic;
bool isMove = false, isPlace = false, isButtonPlaceDown = false;
unsigned ntime;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SetTimer(hWnd, 1, 30, NULL);
    switch (message)
    {
    case WM_CREATE:
    {
        HMENU bar = GetMenu(hWnd);
        HMENU subMenu = CreateMenu();

        AppendMenu(subMenu, MF_STRING, IDM_MOVE, L"MOVE");
        AppendMenu(subMenu, MF_STRING, IDM_CLEAR, L"CLEAR");
        AppendMenu(subMenu, MF_STRING, IDM_PLACE, L"PLACE");

        AppendMenu(bar, MF_POPUP, (UINT_PTR) subMenu, L"ACTION");

        SetMenu(hWnd, bar);
        Pic = std::make_unique<BitMapController>(namePic, speedPic, scalePic);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        if (isMove == false && isButtonPlaceDown == true)
        {
            float x = LOWORD(lParam);
            float y = HIWORD(lParam);
            Pic->SetStartPos({ x, y });
            isPlace = true;
            isButtonPlaceDown = false;
        }
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_CLEAR:
        {
            Pic->ClearBitMap();
            isMove = false;
            break;
        }
        case IDM_MOVE:
        {
            if (!isButtonPlaceDown && Pic->CanDraw())
                isMove = !isMove;
            break;
        }
        case IDM_PLACE:
        {
            isButtonPlaceDown = true;
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
    }
    break;
    case WM_TIMER:
        ntime++;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        if (isPlace)
        {
            Pic->PlaceBitmap(hdc);
            isPlace = false;
        }

        if (Pic->CanDraw())
            Pic->DrawBitMap(hdc);

        if (isMove)
        {
            Pic->MoveBitMap(hdc, hWnd);
        }

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
