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
	
	WindowHandle CreateAndShowWindow(int x, int y, int w, int h, const wchar_t *title);
}