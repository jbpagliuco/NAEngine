#pragma once

#if defined(_NA_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "Base/Util/Util.h"

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

		bool IsActiveWindow()const;

		Point GetPosition()const;
		Size GetSize()const;
		Size GetViewportSize()const;

		float GetAspectRatio()const;
	};
	extern Window INVALID_WINDOW;
	
	Window CreateAndShowWindow(int x, int y, int w, int h, const wchar_t *title);
}