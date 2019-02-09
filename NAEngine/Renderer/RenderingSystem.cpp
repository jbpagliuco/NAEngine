#include "RenderingSystem.h"

#include "Base/Debug/Assert.h"
#include "Base/OS/OS.h"

namespace na
{
	bool RenderingSystemInit()
	{
		WindowHandle hwnd = CreateAndShowWindow(-1, -1, 800, 600, L"NA Game LUL");
		NA_FATAL_ERROR(hwnd != nullptr, "Failed to create main window.");

		return true;
	}

	void RenderingSystemShutdown()
	{

	}

	void RenderingSystemDoFrame()
	{

	}
}