#include "Window.h"
#include "Resource.h"

#define WS_MYSTYLE (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)

// 정적 멤버 변수 정의
Window::WindowClass Window::WindowClass::wndClass;

LPCWSTR Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

Window::WindowClass::WindowClass() noexcept
    : hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEXW wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetInstance();
    wcex.hIcon = LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_REDBLACKTREE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_REDBLACKTREE);
    wcex.lpszClassName = GetName();
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassEx(&wcex);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

Window::Window(const WCHAR* app_title, UINT width, UINT height)
{
    RECT wr;

    wr.left = 100;
    wr.top = 100;
    
    wr.right = width + wr.left;
    wr.bottom = height + wr.top;

    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZE | WS_SYSMENU, FALSE );

    hWnd = CreateWindowW( WindowClass::GetName(), app_title, WS_MYSTYLE,
        CW_USEDEFAULT, 0, width, height, nullptr, nullptr, WindowClass::GetInstance(), nullptr);

    if (!hWnd)
    {
        DWORD error = GetLastError();
        LPVOID errorMsg;

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM ,
            NULL,
            error,
            0, // Default language
            (LPWSTR)&errorMsg,
            0,
            NULL
        );

        MessageBox(NULL, (LPWSTR)errorMsg, L"Error", MB_ICONERROR);
        LocalFree(errorMsg);
        return;
    }

    gfx.Init(hWnd);

    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

std::optional<int> Window::ProcessMessages()
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return (int)msg.wParam;
        }
                
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

void Window::DoFrame()
{
    gfx.draw();
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(WindowClass::GetInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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

INT_PTR CALLBACK Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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


