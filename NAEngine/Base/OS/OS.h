#pragma once

#if defined(_NA_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace na
{
#if defined(_NA_WIN32)
	typedef HWND WindowHandle;
#endif

	struct Window
	{
		int x, y;
		int width, height;
		WindowHandle handle;

		bool operator==(const Window &rhs) { return handle == rhs.handle; }
		bool operator!=(const Window &rhs) { return handle != rhs.handle; }
	};
	extern Window INVALID_WINDOW;
	
	Window CreateAndShowWindow(int x, int y, int w, int h, const wchar_t *title);
}