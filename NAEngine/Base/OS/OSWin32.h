#pragma once

#if defined(_NA_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace na
{
	HWND CreateWindowWin32(int x, int y, int w, int h, const wchar_t *title);
}

#endif