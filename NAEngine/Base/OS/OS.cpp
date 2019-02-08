#include "OS.h"

#if defined(_NA_WIN32)
#include "OSWin32.h"
#endif

#include "Debug/Assert.h"

namespace na
{
	WindowHandle CreateAndShowWindow(int x, int y, int w, int h, const wchar_t *title)
	{
#if defined(_NA_WIN32)
		return CreateWindowWin32(x, y, w, h, title);
#endif
	}
}