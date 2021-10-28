// lr2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ProjecTask.h"
#include <memory>
#include <mutex>
#include <semaphore>
#include <random>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

#define MAX_LOADSTRING 100
#define IDM_START 123

//#define MUTEX 
#define SEMAPHORE

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


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
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        width - (int)button.GetRetreat().x - button.GetSize().cx,
        height - (int)button.GetRetreat().y - button.GetSize().cy,
        button.GetSize().cx, button.GetSize().cy, hWnd, hMenu, NULL, NULL));
}

class IObject
{
public:

    void virtual Draw(HDC hdc, Point pos, SIZE size) = 0;
    virtual ~IObject() {

    }
};

class Circle : public IObject
{
public:
    Circle() {}

    void Draw(HDC hdc, Point pos, SIZE size) override {
        auto m = min(size.cx, size.cy);
        Ellipse(hdc, pos.x - m / 2, pos.y - m / 2, pos.x + m, pos.y + m);
    }
};

template<typename Base, typename T, typename... Args>
std::unique_ptr<Base> make_unique(Args&&... args)
{
    return std::unique_ptr<Base>(new T(std::forward<Args>(args)...));
}

#ifdef SEMAPHORE
std::binary_semaphore s1((ptrdiff_t)0), s2((ptrdiff_t)0);
#endif // SEMAPHORE


class ObjectController {
    std::unique_ptr<IObject> obj;
    Speed speed;
    Point pos;

#ifdef MUTEX
    std::mutex m;
#endif // MUTEX


public:
    ObjectController(std::unique_ptr<IObject> object, Point pos, Speed speed) {
        obj = move(object);
        this->pos = pos;
        this->speed = speed;
    }

    void MoveRight() {
        {
#ifdef MUTEX
            std::lock_guard<std::mutex> lk(m);
#endif // MUTEX
#ifdef SEMAPHORE
            s1.try_acquire_for(std::chrono::milliseconds(50));
#endif // SEMAPHORE
            pos.x += speed.xSpeed;
#ifdef SEMAPHORE
            s2.release();
#endif // SEMAPHORE
        }
    }

    void MoveLeft() {
        {
#ifdef MUTEX
            std::lock_guard<std::mutex> lk(m);
#endif // MUTEX
#ifdef SEMAPHORE
            s2.try_acquire_for(std::chrono::milliseconds(50));
#endif // SEMAPHORE
            pos.x -= speed.xSpeed;
#ifdef SEMAPHORE
            s1.release();
#endif // SEMAPHORE
        }
    }

    void MoveDown() {
        pos.y += speed.ySpeed;
    }

    void Draw(HDC hdc) {
        obj->Draw(hdc, pos, { 50, 50 });
    }

};


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
HWND hWnd;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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

ObjectController controller(make_unique<IObject, Circle>(), { 150, 150 }, { 10 ,1 });
Button button_start(TEXT("Start"), { 60, 20 });
Point pStart = { 10, 10 };
HANDLE hThread1, hThread2;
std::mt19937 engine;


DWORD WINAPI FirstThreadMove(LPVOID lpParam)
{
    while (true) {
        controller.MoveLeft();
        InvalidateRect(hWnd, NULL, FALSE);
        Sleep(engine() % 100);
    }
    return 1;
}

DWORD WINAPI SecondThreadMove(LPVOID lpParam)
{
    while (true) {
        controller.MoveRight();
        InvalidateRect(hWnd, NULL, FALSE);
        Sleep(engine() % 100);
    }
    return 1;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (message)
    {
    case WM_CREATE:
    {
        engine.seed(std::time(nullptr));
        AddButton(hWnd, button_start, (HMENU)IDM_START, pStart);

        hThread1 = CreateThread(NULL, 0u, (LPTHREAD_START_ROUTINE)FirstThreadMove, 0, 0, NULL);
        SuspendThread(hThread1);

        hThread2 = CreateThread(NULL, 0u, (LPTHREAD_START_ROUTINE)SecondThreadMove, 0, 0, NULL);
        SuspendThread(hThread2);


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
            case IDM_START:
                ResumeThread(hThread1);
                ResumeThread(hThread2);
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
    case WM_PAINT:
    {
        RECT Rect;
        GetClientRect(hWnd, &Rect);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &Rect, hBrush);
        
        controller.MoveDown();
        controller.Draw(hdc);
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
