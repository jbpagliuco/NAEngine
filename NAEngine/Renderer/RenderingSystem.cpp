#include "RenderingSystem.h"

#include "Base/Debug/Assert.h"
#include "Base/OS/OS.h"

#include "RenderDefs.h"

namespace na
{
	bool RenderingSystemInit()
	{
		int width = 800;
		int height = 600;

		RendererInitParams p;
		p.mWidth = width;
		p.mHeight = height;
		p.mWindowHandle = CreateAndShowWindow(-1, -1, 800, 600, L"NA Game LUL");;

		NA_FATAL_ERROR(p.mWindowHandle != nullptr, "Failed to create main window.");
		
		return NA_Renderer->Initialize(p);
	}

	void RenderingSystemShutdown()
	{
		NA_Renderer->Shutdown();
	}

	void RenderingSystemDoFrame()
	{

	}
}