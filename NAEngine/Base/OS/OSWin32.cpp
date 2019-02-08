#include "OSWin32.h"

#include "Debug/Assert.h"

namespace na
{
	static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	HWND CreateWindowWin32(int x, int y, int w, int h, const wchar_t *title)
	{
		HINSTANCE appInst = GetModuleHandle(NULL);

		// 1. Register our window class
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpfnWndProc = MainWndProc;
		wc.hInstance = appInst;
		wc.hIconSm = wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = title;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		NA_ASSERT_RETURN_VALUE(RegisterClassEx(&wc), NULL, "Failed to register window class '%ls'. Error Code: %d", title, GetLastError());

		// 2. Create the window
		HWND hwnd = CreateWindowEx(
			NULL,
			title,
			title,
			WS_OVERLAPPEDWINDOW,

			x, y,
			w, h,

			NULL,
			NULL,
			appInst,
			NULL
		);

		NA_ASSERT_RETURN_VALUE(hwnd != NULL, NULL, "Failed to create window '%ls'. Error Code: %d", title, GetLastError());

		// 3. Show the window
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		// Success!
		return hwnd;
	}
}