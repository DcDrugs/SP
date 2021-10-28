// lr2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ProjecTask.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

#define MAX_LOADSTRING 100
#define IDM_CLEAR 123
#define IDM_DEL 124
#define IDM_ADD 125
#define IDM_RIGHT 126
#define IDM_LIST_LEFT 127
#define IDM_LIST_RIGHT 128
#define IDM_EDIT 129

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

void DestroyButton(Button& button)
{
    DestroyWindow(button.GetHwnd());
    button.SetHwnd(0);
}


class ListBox : public WindowsItem
{
public:
    ListBox(LPCTSTR name, SIZE size) : WindowsItem(name, size) {};
};

void AddList(HWND hWnd, ListBox& box, HMENU hMenu, Point reatreat = { 0, 0 })
{
    box.SetRetreat(reatreat);
    RECT rc;
    GetClientRect(hWnd, &rc);

    LONG width = rc.right - rc.left;
    LONG height = rc.bottom - rc.top;

    box.SetHwnd(CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), box.GetName(),
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL | LBS_MULTIPLESEL,
        (int)box.GetRetreat().x,
        (int)box.GetRetreat().y,
        box.GetSize().cx, box.GetSize().cy, hWnd, hMenu, hInst, NULL));
}

void DestroyBox(ListBox& box)
{
    DestroyWindow(box.GetHwnd());
    box.SetHwnd(0);
}

class EditBox : public WindowsItem
{
public:
    EditBox(LPCTSTR name, SIZE size) : WindowsItem(name, size) {};
};

void AddEdit(HWND hWnd, EditBox& box, HMENU hMenu, Point reatreat = { 0, 0 })
{
    box.SetRetreat(reatreat);
    RECT rc;
    GetClientRect(hWnd, &rc);

    LONG width = rc.right - rc.left;
    LONG height = rc.bottom - rc.top;

    box.SetHwnd(CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), box.GetName(),
        WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        (int)box.GetRetreat().x,
        (int)box.GetRetreat().y,
        box.GetSize().cx, box.GetSize().cy, hWnd, hMenu, hInst, NULL));
}

void DestroyEdit(EditBox& box)
{
    DestroyWindow(box.GetHwnd());
    box.SetHwnd(0);
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


bool Contains(HWND hListBox, TCHAR* s) {
    int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
    for (int i = 0; i < count; i++) {
        TCHAR buffer[256];
        SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)buffer);
        if (!_tcscmp(s, buffer))
            return true;
    }

    return false;
}

void AddStringInListBox(HWND hWnd, HWND hListBox, TCHAR* s) {
    if (!Contains(hListBox, s))
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)s);
    else
        MessageBox(hWnd, L"This string is already on the list", L"Error", MB_OK);
}

bool DeleteSelectedItem(HWND hListBox) {
    int count = SendMessage(hListBox, LB_GETSELCOUNT, 0, 0);
    int countBuffer[100];
    SendMessage(hListBox, LB_GETSELITEMS, 100, (LPARAM)countBuffer);

    for (int i = count - 1; i >= 0; --i) {
        int itemId = countBuffer[i];
        SendMessage(hListBox, LB_DELETESTRING, itemId, 0);
    }

    return true;
}

void ToRight(HWND hWnd, HWND hListBox1, HWND hListBox2) {
    int selCount = SendMessage(hListBox1, LB_GETSELCOUNT, 0, 0);
    int countBuffer[100];
    SendMessage(hListBox1, LB_GETSELITEMS, 100, (LPARAM)countBuffer);

    if (selCount == 0) {
        MessageBox(hWnd, L"Select item in first ListBox", L"Error", MB_OK);
    }
    else {
        for (int i = selCount - 1; i >= 0; --i) {
            int itemId = countBuffer[i];
            TCHAR buffer[256];
            SendMessage(hListBox1, LB_GETTEXT, itemId, (LPARAM)buffer);
            AddStringInListBox(hWnd, hListBox2, buffer);
        }

        DeleteSelectedItem(hListBox1);
    }
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

SIZE main_size = { 0,0 };
Point main_point = { 0,0 };

Button button_add(TEXT("Add"), { 60, 20 });
Button button_del(TEXT("Delete"), { 60, 20 });
Button button_clear(TEXT("Clear"), { 60, 20 });
Button button_right(TEXT("Right"), { 60, 20 });
Point pClear = { 10, 10 };
Point pAdd = { 10, 40 };
Point pDel = { 10, 70 };
Point pRight = { 10, 100 };

ListBox list_left(TEXT(""), { 200, 200 });
ListBox list_right(TEXT(""), { 200, 200 });
Point pListLeft = { 50, 20 };
Point pListRight = { 270, 20 };

EditBox edit(TEXT(""), { 200, 40 });
Point pEdit = { 50, 250 };

SIZE NewSize(HWND hWnd, SIZE size) {

    RECT rc;
    GetClientRect(hWnd, &rc);


    LONG width = rc.right - rc.left;
    LONG height = rc.bottom - rc.top;
    float w = float(width) / float(main_size.cx);
    float h = float(height) / float(main_size.cy);
    size.cx *= w;
    size.cy *= h;


    return size;
}

void Refresh(HWND hWnd) {
    DestroyButton(button_add);
    DestroyButton(button_right);
    DestroyButton(button_del);
    DestroyButton(button_clear);
    DestroyBox(list_left);
    DestroyBox(list_right);
    DestroyEdit(edit);

    button_add.SetSize(NewSize(hWnd, button_add.GetRealSize()));
    AddButton(hWnd, button_add, (HMENU)IDM_ADD, pAdd);
    button_clear.SetSize(NewSize(hWnd, button_clear.GetRealSize()));
    AddButton(hWnd, button_clear, (HMENU)IDM_CLEAR, pClear);
    button_del.SetSize(NewSize(hWnd, button_del.GetRealSize()));
    AddButton(hWnd, button_del, (HMENU)IDM_DEL, pDel);
    button_right.SetSize(NewSize(hWnd, button_right.GetRealSize()));
    AddButton(hWnd, button_right, (HMENU)IDM_RIGHT, pRight);
    list_left.SetSize(NewSize(hWnd, list_left.GetRealSize()));
    AddList(hWnd, list_left, (HMENU)IDM_LIST_LEFT, pListLeft);
    list_right.SetSize(NewSize(hWnd, list_right.GetRealSize()));
    AddList(hWnd, list_right, (HMENU)IDM_LIST_RIGHT, pListRight);
    edit.SetSize(NewSize(hWnd, edit.GetRealSize()));
    AddEdit(hWnd, edit, (HMENU)IDM_EDIT, pEdit);
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
        RECT rc;
        GetClientRect(hWnd, &rc);

        LONG width = rc.right - rc.left;
        LONG height = rc.bottom - rc.top;

        main_size.cx = width;
        main_size.cy = height;
        main_point.x = rc.left;
        main_point.y = rc.right;
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
                int len;
                len = SendMessage(edit.GetHwnd(), WM_GETTEXTLENGTH, 0, 0);
                if (len == 0)
                    MessageBox(hWnd, L"Fill the text field", L"Error", MB_OK);
                else {
                    TCHAR* buffer = new TCHAR[len];
                    SendMessage(edit.GetHwnd(), WM_GETTEXT, len + 1, (LPARAM)buffer);
                    AddStringInListBox(hWnd, list_left.GetHwnd(), buffer);
                    SetWindowText(GetDlgItem(hWnd, IDM_EDIT), L"");//очищаем edit
                }
                break;
            case IDM_CLEAR:
                SendMessage(list_left.GetHwnd(), LB_RESETCONTENT, 0, 0);
                SendMessage(list_right.GetHwnd(), LB_RESETCONTENT, 0, 0);
                break;
            case IDM_RIGHT:
                ToRight(hWnd, list_left.GetHwnd(), list_right.GetHwnd());
                break;
            case IDM_DEL:
                DeleteSelectedItem(list_left.GetHwnd());
                DeleteSelectedItem(list_right.GetHwnd());
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
