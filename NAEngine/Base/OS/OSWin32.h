#pragma once

#if defined(_NA_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace na
{
	LRESULT CALLBACK Win32WndProc(HWND, UINT, WPARAM, LPARAM);
}

#endif