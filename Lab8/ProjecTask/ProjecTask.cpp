// lr2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ProjecTask.h"
#include <list>
#include <TlHelp32.h>
#include <windowsx.h>

#define MAX_LOADSTRING 100

#define IDM_LISTBOX1 127
#define IDM_LISTBOX2 128
#define IDM_IDLE 129
#define IDM_NORMAL 130
#define IDM_HIGH 131
#define IDM_REAL_TIME 132



// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна


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

class ListBox : public WindowsItem
{
public:
    ListBox(LPCTSTR name, SIZE size) : WindowsItem(name, size) {};
};

void AddListBox(HWND hWnd, ListBox& listBox, HMENU hMenu, Point reatreat = { 0, 0 })
{
    listBox.SetRetreat(reatreat);
    listBox.SetHwnd(CreateWindow(TEXT("LISTBOX"), listBox.GetName(),
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
        (int)listBox.GetRetreat().x,
        (int)listBox.GetRetreat().y,
        listBox.GetSize().cx, listBox.GetSize().cy, hWnd, hMenu, NULL, NULL));
}

std::list<PROCESSENTRY32> procs;
ListBox listBox1(TEXT("Proccesses"), { 300, 400 });
ListBox listBox2(TEXT("Modules"), { 300, 400 });
Point pListBox1{ 10, 10 };
Point pListBox2{ 350, 10 };


void ClearListBox(ListBox& listBox) {
    while (SendMessage(listBox.GetHwnd(), LB_GETCOUNT, 0, NULL))
        SendMessage(listBox.GetHwnd(), LB_DELETESTRING, 0, NULL);
}


void ShowListBox(ListBox& listBox,const std::list<PROCESSENTRY32>& items) {
    for (auto i : items)
        SendMessage(listBox.GetHwnd(), LB_ADDSTRING, 0, (LPARAM)i.szExeFile);
}

void ShowListBox(ListBox& listBox, const std::list<MODULEENTRY32>& items) {
    for (auto i : items)
        SendMessage(listBox.GetHwnd(), LB_ADDSTRING, 0, (LPARAM)i.szModule);
}

std::list<PROCESSENTRY32> GetProcs()
{
    std::list<PROCESSENTRY32> results = std::list<PROCESSENTRY32>();
    PROCESSENTRY32 s;
    results.push_back(s);
    results.back().dwSize = sizeof(PROCESSENTRY32);

    HANDLE toolHelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bool result = Process32First(toolHelp, &results.back());
    while (result)
    {
        results.push_back(s);
        results.back().dwSize = sizeof(PROCESSENTRY32);
        result = Process32Next(toolHelp, &results.back());
    }
    CloseHandle(toolHelp);

    return results;
}

std::list<MODULEENTRY32> GetModules(PROCESSENTRY32 proc)
{
    std::list<MODULEENTRY32> results = std::list<MODULEENTRY32>();
    MODULEENTRY32 s;
    results.push_back(s);
    results.back().dwSize = sizeof(MODULEENTRY32);

    HANDLE toolHelp = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, proc.th32ProcessID);
    bool result = Module32First(toolHelp, &results.back());
    while (result)
    {
        results.push_back(s);
        results.back().dwSize = sizeof(MODULEENTRY32);
        result = Module32Next(toolHelp, &results.back());
    }
    CloseHandle(toolHelp);

    return results;
}

auto find(int index) {
    auto it = procs.begin();
    std::advance(it, index);
    return it;
}


void SetPriority(DWORD priorityClass) {
    int itemId = SendMessage(listBox1.GetHwnd(), LB_GETCURSEL, 0, NULL);
    if (itemId != -1) {
        auto it = find(itemId);
        if (it != procs.end())
        {
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, it->th32ProcessID);
            SetPriorityClass(hProcess, priorityClass);
            CloseHandle(hProcess);
        }
    }
}


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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (message)
    {
    case WM_CREATE:
    {
        AddListBox(hWnd, listBox1, (HMENU)IDM_LISTBOX1, pListBox1);
        AddListBox(hWnd, listBox2, (HMENU)IDM_LISTBOX2, pListBox2);

        procs = GetProcs();
        ShowListBox(listBox1, procs);

        break;
    }
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        switch (wmId)
        {
        case IDM_LISTBOX1:
            if (wmEvent == LBN_SELCHANGE)
            {
                int itemId = SendMessage(listBox1.GetHwnd(), LB_GETCURSEL, 0, NULL);
                if (itemId != -1) {
                    auto it = find(itemId);
                    if (it != procs.end())
                    {
                        auto modules = GetModules(*it);
                        ClearListBox(listBox2);
                        ShowListBox(listBox2, modules);
                    }
                }
            }
            break;
        case IDM_IDLE:
            SetPriority(IDLE_PRIORITY_CLASS);
            break;
        case IDM_NORMAL:
            SetPriority(NORMAL_PRIORITY_CLASS);
            break;
        case IDM_HIGH:
            SetPriority(HIGH_PRIORITY_CLASS);
            break;
        case IDM_REAL_TIME:
            SetPriority(REALTIME_PRIORITY_CLASS);
            break;
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
    case WM_CONTEXTMENU:
        if ((HWND)wParam == listBox1.GetHwnd()) {
            int itemId = SendMessage(listBox1.GetHwnd(), LB_GETCURSEL, 0, NULL);
            if (itemId != -1) {
                auto it = find(itemId);
                if (it != procs.end()) {
                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, it->th32ProcessID);
                    int pC = GetPriorityClass(hProcess);
                    CloseHandle(hProcess);


                    HMENU hPopupMenu = CreatePopupMenu();

                    AppendMenu(hPopupMenu, pC == REALTIME_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, IDM_REAL_TIME, L"Real time");
                    AppendMenu(hPopupMenu, pC == HIGH_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, IDM_HIGH, L"High");
                    AppendMenu(hPopupMenu, pC == NORMAL_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, IDM_NORMAL, L"Normal");
                    AppendMenu(hPopupMenu, pC == IDLE_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, IDM_IDLE, L"Idle");
                    TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hWnd, NULL);
                }
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
