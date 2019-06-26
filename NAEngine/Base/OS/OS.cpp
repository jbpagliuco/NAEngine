#include "OS.h"

#if defined(_NA_WIN32)
#include "OSWin32.h"
#endif

#include "Debug/Assert.h"

namespace na
{
	Window INVALID_WINDOW = { 0, 0, 0, 0, NULL };

	float Window::GetAspectRatio()const
	{
		return (float)width / (float)height;
	}

	Window CreateAndShowWindow(int x, int y, int w, int h, const wchar_t *title)
	{
#if defined(_NA_WIN32)
		return CreateWindowWin32(x, y, w, h, title);
#endif
	}
}