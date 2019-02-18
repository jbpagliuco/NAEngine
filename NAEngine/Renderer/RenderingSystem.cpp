#include "RenderingSystem.h"

#include "Base/Debug/Assert.h"
#include "Base/OS/OS.h"

#include "RenderDefs.h"
#include "Scene/Scene.h"
#include "Scene/ForwardRenderer.h"

namespace na
{
	Scene MainScene;
	ForwardRenderer FRenderer;

	bool RenderingSystemInit()
	{
		int width = 800;
		int height = 600;

		RendererInitParams p;
		p.mWidth = width;
		p.mHeight = height;
		p.mWindowHandle = CreateAndShowWindow(-1, -1, 800, 600, L"NA Game LUL");

		NA_FATAL_ERROR(p.mWindowHandle != nullptr, "Failed to create main window.");
		
		bool success = NA_Renderer->Initialize(p);
		NA_FATAL_ERROR(success, "Failed to initialize renderer.");

		FRenderer.Initialize();

		return true;
	}

	void RenderingSystemShutdown()
	{
		FRenderer.Shutdown();

		NA_Renderer->Shutdown();
	}

	void RenderingSystemDoFrame()
	{
		NA_Renderer->BeginRender();

		FRenderer.BeginRender();
		FRenderer.RenderScene(&MainScene);
		FRenderer.EndRender();

		NA_Renderer->EndRender();
	}
}