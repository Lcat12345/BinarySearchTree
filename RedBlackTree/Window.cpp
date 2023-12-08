#include "Window.h"
#include "imgui.h"
#include "imgui_impl_win32.h"

#include "Resource.h"
#include <tchar.h>

#define WS_MYSTYLE (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)

// ���� ��� ���� ����
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
    : width(width), height(height)
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

    // ImGui �ʱ�ȭ
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hWnd);

    gfx.Init(hWnd);

    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    // ImGui ����
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    

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
    // ImGui ������Ʈ �� ������
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, ImGui!");
    ImGui::Text("This is a simple ImGui example.");
    ImGui::End();
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(WindowClass::GetInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            KillTimer(hWnd, 1);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);  // 1�ʸ��� Ÿ�̸� �߻�
        break;
    case WM_TIMER:
        if (wParam == 1) {
            // Ÿ�̸Ӱ� �߻��� ������ ȭ���� ����
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // �ð��� ���ͼ� �׸���
        SYSTEMTIME st;
        GetLocalTime(&st);
        TCHAR timeStr[9]; // "HH:MM:SS" ������ ���ڿ�
        _stprintf_s(timeStr, _T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);

        // ��Ʈ ����
        HFONT hFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("D2Coding"));
        HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

        TextOut(hdc, 120, 70, timeStr, _tcslen(timeStr));

        SelectObject(hdc, oldFont);
        DeleteObject(oldFont);

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

void Window::clear()
{
    HDC hdc = GetDC(hWnd);

    HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Rectangle(hdc, -1, -1, width, height);

    DeleteObject(hBrush);
}


