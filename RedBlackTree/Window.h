#pragma once
#include <Windows.h>
#include <optional>

class Window
{
private:
	class WindowClass
	{
	public:
		static LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		// singleton
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr LPCWSTR wndClassName = L"자료구조 프로그램";

	private:
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(const WCHAR* app_title, UINT width, UINT height);
	~Window();
	Window(const Window&) = delete;
	Window& operator= (const Window&) = delete;
	static std::optional<int> ProcessMessages();


private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

public:

private:
	HWND	hWnd;
};

