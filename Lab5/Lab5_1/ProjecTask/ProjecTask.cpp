// lr2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ProjecTask.h"
#include <bitset>

#define MAX_LOADSTRING 100


#define IDM_COLOR 123
#define IDM_RED 124
#define IDM_GREEN 125
#define IDM_BLUE 126

#define IDM_FIGURE 127
#define IDM_CIRCLE 128
#define IDM_RECTANGLE 129
#define IDM_STAR 130
#define IDM_RHOMBUS 131

#define IDM_DRAW 132

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна


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
    SIZE size;
public:

    WindowsItem(LPCTSTR name, SIZE size) : name(name)
    {
        this->size = size;
        retreat = { 0, 0 };
        hWnd = NULL;
    }

    void SetSize(SIZE size)
    {
        this->size = size;
    }

    SIZE GetSize()
    {
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
};

class Button : public WindowsItem
{
public:
    Button(LPCTSTR name, SIZE size) : WindowsItem(name, size) {};
};

class Grid : public WindowsItem
{
public:
    Grid(LPCTSTR name, SIZE size) : WindowsItem(name, size) {};
};

class AutoCheckBox : public WindowsItem
{
public:
    AutoCheckBox(LPCTSTR name, SIZE size) : WindowsItem(name, size) {};
};


void AddAutoCheckBox(HWND hWnd, AutoCheckBox& box, HMENU hMenu, Point reatreat = { 0, 0 })
{
    box.SetRetreat(reatreat);

    box.SetHwnd(CreateWindow(TEXT("BUTTON"), box.GetName(),
        WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        (int)box.GetRetreat().x,
        (int)box.GetRetreat().y,
        box.GetSize().cx, box.GetSize().cy, hWnd, hMenu, NULL, NULL));

}

void AddButton(HWND hWnd, Button& button, HMENU hMenu, Point reatreat = { 0, 0 })
{
    button.SetRetreat(reatreat);

    button.SetHwnd(CreateWindow(TEXT("BUTTON"), button.GetName(),
        WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
        (int)button.GetRetreat().x,
        (int)button.GetRetreat().y,
        button.GetSize().cx, button.GetSize().cy, hWnd, hMenu, NULL, NULL));

}

void AddGrid(HWND hWnd, Grid& grid, HMENU hMenu, Point reatreat = { 0, 0 })
{
    grid.SetRetreat(reatreat);

    grid.SetHwnd(CreateWindow(TEXT("BUTTON"), grid.GetName(),
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
        (int)grid.GetRetreat().x,
        (int)grid.GetRetreat().y,
        grid.GetSize().cx, grid.GetSize().cy, hWnd, hMenu, NULL, NULL));

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

LPCWSTR project = TEXT("Lab5_2");

Grid color(TEXT("Color"), { 90, 20 });
Button button_red(TEXT("Red"), { 90, 20 });
Button button_green(TEXT("Green"), { 90, 20 });
Button button_blue(TEXT("Blue"), { 90, 20 });
Point pColor = { 10, 10 };
Point pRed = { 10, 30 };
Point pGreen = { 10, 50 };
Point pBlue = { 10, 70 };

Grid figure(TEXT("figure"), { 90, 20 });
Button button_circle(TEXT("Circle"), { 90, 20 });
Button button_rectangle(TEXT("Rectangle"), { 90, 20 });
Button button_star(TEXT("Star"), { 90, 20 });
Button button_rhombus(TEXT("Rhombus"), { 90, 20 });
Point pFigure = { 10, 100 };
Point pCircle = { 10, 120 };
Point pRectangle = { 10, 140 };
Point pStar = { 10, 160 };
Point pRhombus = { 10, 180 };


AutoCheckBox draw(TEXT("Draw"), { 90, 20 });
Point pDraw = { 10, 220 };

union Data
{
    struct {
        long long canDraw : 2;
        long long color : 3;
        long long shape : 3;
    };
} result;

void SendItem(HWND hWnd, Data result) {
    std::string binary = std::bitset<2>(result.canDraw).to_string()
        + std::bitset<3>(result.color).to_string() + std::bitset<3>(result.shape).to_string();
    COPYDATASTRUCT cd;
    cd.lpData = binary.data();
    cd.cbData = binary.size() + 1;
    HWND app2 = FindWindowW(NULL, project);
    if (app2 == NULL) MessageBoxA(hWnd, "App not find!", "CheckBox", MB_OK);
    else SendMessageW(app2, WM_COPYDATA, 0, (LPARAM)&cd);
}


bool CanDraw(HWND hWnd) {
    HWND hwndCheck = GetDlgItem(hWnd, IDM_DRAW);
    LRESULT res = SendMessage(hwndCheck, BM_GETCHECK, 0, 0);
    return res == BST_CHECKED;
    
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (message)
    {
    case WM_SIZE:
    case WM_CREATE:
    {
        AddGrid(hWnd, color, (HMENU)IDM_COLOR, pColor);
        AddButton(hWnd, button_red, (HMENU)IDM_RED, pRed);
        AddButton(hWnd, button_green, (HMENU)IDM_GREEN, pGreen);
        AddButton(hWnd, button_blue, (HMENU)IDM_BLUE, pBlue);


        AddGrid(hWnd, figure, (HMENU)IDM_FIGURE, pFigure);
        AddButton(hWnd, button_circle, (HMENU)IDM_CIRCLE, pCircle);
        AddButton(hWnd, button_rectangle, (HMENU)IDM_RECTANGLE, pRectangle);
        AddButton(hWnd, button_star, (HMENU)IDM_STAR, pStar);
        AddButton(hWnd, button_rhombus, (HMENU)IDM_RHOMBUS, pRhombus);

        AddAutoCheckBox(hWnd, draw, (HMENU)IDM_DRAW, pDraw);
        break;
    }
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        if (IDM_DRAW <= LOWORD(wParam)) {
            result.canDraw = CanDraw(hWnd);
        }
        if (IDM_RED <= LOWORD(wParam) && LOWORD(wParam) <= IDM_BLUE)
        {
            result.color = LOWORD(wParam) - IDM_RED;
            CheckRadioButton(hWnd, IDM_RED, IDM_BLUE, LOWORD(wParam));
        }
        if (IDM_CIRCLE <= LOWORD(wParam) && LOWORD(wParam) <= IDM_RHOMBUS)
        {
            result.shape = LOWORD(wParam) - IDM_CIRCLE;
            CheckRadioButton(hWnd, IDM_CIRCLE, IDM_RHOMBUS, LOWORD(wParam));
        }
        SendItem(hWnd, result);

        switch (wmEvent)
        {
        case BN_CLICKED:
            switch (wmId)
            {
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
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
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
