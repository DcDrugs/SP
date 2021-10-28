// lr2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ProjecTask.h"
#include <utility>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

#define IDM_CLEAR 124
#define IDM_ADD 125

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


struct Point
{
    float x;
    float y;
};

class WindowsItem 
{
private:
    HWND hWnd;
    LPCTSTR name;
    Point retreat;
    SIZE real_size;
    SIZE size = { 0,0 };
public:

    WindowsItem(LPCTSTR name, SIZE size) : name(name)
    {
        this->real_size = size;
        retreat = { 0, 0 };
        hWnd = NULL;
    }

    SIZE GetRealSize()
    {
        return real_size;
    }

    void SetSize(SIZE size)
    {
        this->size = size;
    }

    SIZE GetSize()
    {
        if (size.cx == 0 && size.cy == 0)
            return real_size;
        else
            return size;
    }

    LPCTSTR GetName()
    {
        return name;
    }

    HWND GetHwnd()
    {
        return hWnd;
    }

    void SetHwnd(HWND hWnd)
    {
        this->hWnd = hWnd;
    }

    void SetRetreat(Point retreat)
    {
        this->retreat = retreat;
    }

    Point GetRetreat()
    {
        return retreat;
    }

    bool IsLoad()
    {
        return hWnd;
    }
};

class Button : public WindowsItem
{
public:
    Button(LPCTSTR name, SIZE size) : WindowsItem(name, size) {};
};

void AddButton(HWND hWnd, Button& button, HMENU hMenu, Point reatreat = { 0, 0 })
{
    button.SetRetreat(reatreat);
    RECT rc;
    GetClientRect(hWnd, &rc);

    LONG width = rc.right - rc.left;
    LONG height = rc.bottom - rc.top;

    button.SetHwnd(CreateWindow(TEXT("BUTTON"), button.GetName(),
        WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
        width - (int)button.GetRetreat().x - button.GetSize().cx,
        height - (int)button.GetRetreat().y - button.GetSize().cy,
        button.GetSize().cx, button.GetSize().cy, hWnd, hMenu, NULL, NULL));

    /*buttonClear = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
        100, 20, 60, 60, hWnd, (HMENU)BUTTON_CLEAR_ID, NULL, NULL);*/

}

void DestroyButton(Button& button)
{
    if (button.IsLoad())
    {
        DestroyWindow(button.GetHwnd());
        button.SetHwnd(0);
    }
}

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

class IObject
{
protected:
    Point pos;
    SIZE size;

public:
    IObject(Point position, SIZE s) {
        pos = position;
        size = s;
    }

    void virtual Draw(HDC hdc) = 0;
};


class Cloud : IObject
{
public:
    Cloud(Point position, SIZE s) : IObject(position, s) {}


    void Draw(HDC hdc) override {
        HBRUSH brush = CreateSolidBrush(RGB(52, 232, 235));
        SelectObject(hdc, brush);
        HPEN pen = CreatePen(BS_SOLID, 3, RGB(52, 232, 235));
        SelectObject(hdc, pen);
        Ellipse(hdc, pos.x, pos.y + size.cy / 20, pos.x + size.cx, pos.y + size.cy + size.cx / 10);
        int posX = pos.x + size.cx * 15 / 16;
        int posY = pos.y + size.cy * 15 / 16;
        Ellipse(hdc, pos.x + size.cx / 2, pos.y - size.cy * 6 / 16, posX + size.cx / 20, posY);
        Ellipse(hdc, pos.x + size.cx / 4, pos.y - size.cy / 6, posX, posY);
    }
};


class Tree : IObject
{
public:
    Tree(Point position, SIZE s) : IObject(position, s) {}


    void Draw(HDC hdc) override {
        HBRUSH brush = CreateSolidBrush(RGB(16, 137, 11));
        SelectObject(hdc, brush);
        HPEN pen = CreatePen(BS_SOLID, 3, RGB(0, 0, 0));
        SelectObject(hdc, pen);

        Ellipse(hdc, pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);


        brush = CreateSolidBrush(RGB(199, 78, 18));
        SelectObject(hdc, brush);
        POINT p1[4] = { pos.x + size.cx * 5 / 10, pos.y + size.cy * 3 / 4, 
                        pos.x + size.cx * 4 / 10, pos.y + size.cy * 2,
                        pos.x + size.cx * 6 / 10, pos.y + size.cy * 2,
                        pos.x + size.cx * 5 / 10, pos.y + size.cy * 3 / 4 };
        Polygon(hdc, p1, 4);
    }
};


class Sun : IObject
{
public:
    Sun(Point position, SIZE s) : IObject(position, s) {}


    void Draw(HDC hdc) override {
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
        SelectObject(hdc, brush);
        HPEN pen = CreatePen(BS_SOLID, 3, RGB(250, 250, 6));
        SelectObject(hdc, pen);

        Ellipse(hdc, pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);
    }
};

class Lake : IObject
{
public:
    Lake(Point position, SIZE s) : IObject(position, s) {}


    void Draw(HDC hdc) override {
        HBRUSH brush = CreateSolidBrush(RGB(12, 56, 235));
        SelectObject(hdc, brush);
        HPEN pen = CreatePen(BS_SOLID, 3, RGB(12, 56, 235));
        SelectObject(hdc, pen);

        Ellipse(hdc, pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);
        int posX = pos.x + size.cx * 15 / 16;
        int posY = pos.y + size.cy * 15 / 16;
        Ellipse(hdc, pos.x + size.cx / 2, pos.y - size.cy * 6 / 16, posX + size.cx / 20, posY);
        Ellipse(hdc, pos.x + size.cx / 3, pos.y - size.cy / 5, posX, posY);
        Ellipse(hdc, pos.x - size.cx / 2, pos.y + size.cy / 4, posX + size.cx / 3, posY + size.cy);
        Ellipse(hdc, pos.x + size.cx / 2, pos.y - size.cy / 3, posX + size.cx / 2, posY + size.cy / 2);
    }
};


class House : IObject
{
public:
    House(Point position, SIZE s) : IObject(position, s) {}


    void Draw(HDC hdc) override {
        HBRUSH brush = CreateSolidBrush(RGB(133, 70, 4));
        SelectObject(hdc, brush);
        HPEN pen = CreatePen(BS_SOLID, 3, RGB(0, 0, 0));
        SelectObject(hdc, pen);


        Arc(hdc,
            pos.x + size.cx * 2, pos.y + size.cy * 12 / 17,
            pos.x, pos.y + size.cy * 7 / 17,
            pos.x + size.cx * 2, pos.y + size.cy * 9 / 17,
            pos.x, pos.y + size.cy * 9 / 17);


        Rectangle(hdc, pos.x + size.cx * 30 / 16, pos.y + size.cy / 2, pos.x + size.cx * 31 / 16, pos.y + size.cy);
        Rectangle(hdc, pos.x + size.cx / 16, pos.y + size.cy / 2, pos.x + size.cx * 2 / 16, pos.y + size.cy);
        Rectangle(hdc, pos.x, pos.y + size.cy * 15 / 16, pos.x + size.cx * 2, pos.y + size.cy);


        POINT p1[4] = { pos.x, pos.y + size.cy * 9 / 17,
                        pos.x + size.cx, pos.y,
                        pos.x + size.cx * 2,  pos.y + size.cy * 9 / 17,
                        pos.x + size.cx, pos.y + size.cy / 3, };
        Polygon(hdc, p1, 4);

        Arc(hdc,
            pos.x, pos.y + size.cy * 7 / 17,
            pos.x + size.cx * 2, pos.y + size.cy * 12 / 17,
            pos.x, pos.y + size.cy * 9 / 17,
            pos.x + size.cx * 2, pos.y + size.cy * 9 / 17);
    }
};


Button button_add(TEXT("Add"), { 30, 30 });
Button button_clear(TEXT("Clear"), { 30, 30 });
Point pClear = { 10, 10 };
Point pAdd = { 10, 50 };
bool isVisible = false;


template<typename T, typename... Args>
T CreateFactory(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

void Refresh(HWND hWnd) {
    DestroyButton(button_add);
    DestroyButton(button_clear);

   
    AddButton(hWnd, button_add, (HMENU)IDM_ADD, pAdd);
    AddButton(hWnd, button_clear, (HMENU)IDM_CLEAR, pClear);
   
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (message)
    {
    case WM_SIZE:
        Refresh(hWnd);
        break;
    case WM_CREATE:
    {
        Refresh(hWnd);
        break;
    }
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        switch (wmEvent)
        {
        case BN_CLICKED:
            switch (wmId)
            {
            case IDM_ADD:
                isVisible = true;
                InvalidateRect(hWnd, NULL, true);
                break;
            case IDM_CLEAR:
                isVisible = false;
                InvalidateRect(hWnd, NULL, true);
                break;
            default:
                break;
            }
            break;
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
    case WM_DRAWITEM:
    {
        DRAWITEMSTRUCT* draw = (DRAWITEMSTRUCT*)lParam;
        HBRUSH brush;
        HPEN pen;
        HDC hdc = draw->hDC;
        brush = CreateSolidBrush(RGB(255, 255, 255));

        switch (draw->CtlID)
        {
        case IDM_ADD:
        {

            pen = CreatePen(BS_SOLID, 2, RGB(0, 255, 0));

            SelectObject(hdc, brush);
            SelectObject(hdc, pen);

            Rectangle(hdc, 0, 0, button_add.GetSize().cx, button_add.GetSize().cy);

            auto x1 = button_add.GetSize().cx / 4;
            auto y1 = button_add.GetSize().cy / 2;
            auto y2 = button_add.GetSize().cy * 3 / 4;

            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, y1, y2);
            LineTo(hdc, y2, x1);
        }
        break;
        case IDM_CLEAR:
        {

            pen = CreatePen(BS_SOLID, 2, RGB(255, 0, 0));

            SelectObject(hdc, brush);
            SelectObject(hdc, pen);

            auto x1 = button_add.GetSize().cx / 3;
            auto y1 = button_add.GetSize().cy * 2 / 3;

            Rectangle(hdc, 0, 0, button_add.GetSize().cx, button_add.GetSize().cy);
            MoveToEx(hdc, x1, x1, NULL);
            LineTo(hdc, y1, y1);
            MoveToEx(hdc, y1, x1, NULL);
            LineTo(hdc, x1, y1);
        }
        break;
        default:
            break;
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (isVisible)
        {
            HBRUSH brush = CreateSolidBrush(RGB(12, 175, 7));
            SelectObject(hdc, brush);
            Rectangle(hdc, 0, 500, 2000, 1000); 


            CreateFactory<Sun>(Point{ 800, 10 }, SIZE{ 200, 200 }).Draw(hdc);
            CreateFactory<Cloud>(Point{ 20, 30 }, SIZE{ 100, 50 }).Draw(hdc);
            CreateFactory<Cloud>(Point{ 150, 100 }, SIZE{ 300, 150 }).Draw(hdc);
            CreateFactory<Cloud>(Point{ 500, 60 }, SIZE{ 200, 100 }).Draw(hdc);
            CreateFactory<Cloud>(Point{ 700, 70 }, SIZE{ 380, 190 }).Draw(hdc);
            CreateFactory<Cloud>(Point{ 1200, 120 }, SIZE{ 200, 200 }).Draw(hdc);
            CreateFactory<Tree>(Point{ 900, 300 }, SIZE{ 50, 100 }).Draw(hdc);
            CreateFactory<Tree>(Point{ 300, 250 }, SIZE{ 70, 140 }).Draw(hdc);
            CreateFactory<Tree>(Point{ 700, 350 }, SIZE{ 140, 280 }).Draw(hdc);
            CreateFactory<House>(Point{ 300, 400 }, SIZE{ 200, 200 }).Draw(hdc);
            CreateFactory<Tree>(Point{ 1200, 370 }, SIZE{ 150, 300 }).Draw(hdc);
            CreateFactory<Lake>(Point{ 0, 700 }, SIZE{ 300, 150 }).Draw(hdc);
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
