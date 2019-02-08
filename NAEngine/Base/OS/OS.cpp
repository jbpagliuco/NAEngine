#include "OS.h"

#if defined(_NA_WIN32)
#include "OSWin32.h"
#endif

#include "Debug/Assert.h"

namespace na
{
#if defined(_NA_WIN32)
	HWND MainWindowHandle = NULL;

	static HWND CreateWindowWin32(int x, int y, int w, int h, const wchar_t *title)
	{
		HINSTANCE appInst = GetModuleHandle(NULL);

		// 1. Register our window class
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpfnWndProc = Win32WndProc;
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

		// Set the window to as the main window, if necessary
		if (MainWindowHandle == NULL) {
			MainWindowHandle = hwnd;
		}

		// Success!
		return hwnd;
	}
#endif

	WindowHandle CreateAndShowWindow(int x, int y, int w, int h, const wchar_t *title)
	{
#if defined(_NA_WIN32)
		return CreateWindowWin32(x, y, w, h, title);
#endif
	}
}