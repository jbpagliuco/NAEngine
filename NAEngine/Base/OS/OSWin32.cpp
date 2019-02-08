#include "OSWin32.h"

namespace na
{
	LRESULT CALLBACK Win32WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}