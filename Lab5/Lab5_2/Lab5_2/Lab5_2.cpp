// Lab5_2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Lab5_2.h"
#include <bitset>
#include <cassert>
#include <map>
#include <memory>
#include <algorithm> 

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

struct Point
{
	float x;
	float y;
} target;


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
    LoadStringW(hInstance, IDC_LAB52, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB52));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB52));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB52);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   RegisterWindowMessageW(TEXT("ProjectTask"));

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


union Data
{
    struct {
        long long canDraw : 2;
        long long color : 3;
        long long shape : 3;
    };
} result;


Data Cast(char* number_u)
{
	Data casted_number;

    casted_number.canDraw = std::bitset<2>(number_u).to_ulong();
    casted_number.color = std::bitset<3>(number_u + 2).to_ulong();
    casted_number.shape = std::bitset<3>(number_u + 5).to_ulong();

	return casted_number;
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


    void Draw(HDC hdc, Point pos, SIZE size) override {
		auto m = min(size.cx, size.cy);
		Ellipse(hdc, pos.x - m / 2, pos.y - m / 2, pos.x + m, pos.y + m);
    }
};

class RectangleObj : public IObject
{
public:


	void Draw(HDC hdc, Point pos, SIZE size) override {
		Rectangle(hdc, pos.x - size.cx / 2, pos.y - size.cy / 2, pos.x + size.cx, pos.y + size.cy);
	}
};

class Star : public IObject
{
public:


	void Draw(HDC hdc, Point pos, SIZE size) override {
		POINT poly[10];
		poly[0].x = pos.x - size.cx;
		poly[0].y = pos.y - 68;
		poly[1].x = pos.x - 30;
		poly[1].y = pos.y - 10;
		poly[2].x = pos.x - 80;
		poly[2].y = pos.y + 26;
		poly[3].x = pos.x - 18;
		poly[3].y = pos.y + 26;
		poly[4].x = pos.x;
		poly[4].y = pos.y + 80;
		poly[5].x = pos.x + 18;
		poly[5].y = pos.y + 26;
		poly[6].x = pos.x + 80;
		poly[6].y = pos.y + 26;
		poly[7].x = pos.x + 30;
		poly[7].y = pos.y - 10;
		poly[8].x = pos.x + size.cx;
		poly[8].y = pos.y - 68;
		poly[9].x = pos.x;
		poly[9].y = pos.y - 32;
		Polygon(hdc, poly, 10);
	}
};

class Rhombus : public IObject
{
public:


	void Draw(HDC hdc, Point pos, SIZE size) override {
		POINT poly[4];
		poly[0].x = pos.x;
		poly[0].y = pos.y - size.cy;
		poly[1].x = pos.x + size.cx;
		poly[1].y = pos.y;
		poly[2].x = pos.x;
		poly[2].y = pos.y + size.cy;
		poly[3].x = pos.x - size.cx;
		poly[3].y = pos.y;
		Polygon(hdc, poly, 4);
	}
};

std::map<short, COLORREF> colors = {
	{0, RGB(255, 0, 0)},
	{1, RGB(0, 255, 0)},
	{2, RGB(0, 0, 255)}
};

template<typename Base, typename T, typename... Args>
std::shared_ptr<Base> make_shared(Args&&... args)
{
	return std::shared_ptr<Base>(new T(std::forward<Args>(args)...));
}

std::map<short, std::shared_ptr<IObject>> shape = {
	{0, make_shared<IObject, Circle>()},
	{1, make_shared<IObject, RectangleObj>()},
	{2, make_shared<IObject, Star>()},
	{3, make_shared<IObject, Rhombus>()}
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COPYDATA:
    {
        Data check = Cast((char*)((PCOPYDATASTRUCT)lParam)->lpData);
        result = check;
        break;
    }
	case WM_LBUTTONDOWN:
		target = { (float)LOWORD(lParam), (float)HIWORD(lParam) };
		InvalidateRect(hWnd, NULL, FALSE);
		break;
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
		HBRUSH hBrush= CreateSolidBrush(RGB(255, 255, 255));
		PAINTSTRUCT ps;
		RECT Rect;
		HDC hdc = BeginPaint(hWnd, &ps);

		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		if (result.canDraw == 0)
		{
			break;
		}
		else
		{
			GetClientRect(hWnd, &Rect);
			hBrush = CreateSolidBrush(colors.at(result.color));
			SelectObject(hdc, hBrush);
			shape.at(result.shape)->Draw(hdc, target, {50, 50});
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
