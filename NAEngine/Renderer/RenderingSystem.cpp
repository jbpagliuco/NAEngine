#include "RenderingSystem.h"

#include <algorithm>

#include "Base/Debug/Assert.h"
#include "Base/OS/OS.h"

#include "NGA/NGACore.h"

#include "ImguiRenderer.h"
#include "Renderer.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include "Scene/ForwardRenderer.h"

namespace na
{
	Scene MainScene;
	ForwardRenderer FRenderer;

	bool RenderingSystemInit()
	{
		constexpr int width = 1600;
		constexpr int height = 900;

		RendererInitParams p;
		p.mWidth = width;
		p.mHeight = height;
		p.mWindow = CreateAndShowWindow(-1, -1, width, height, L"NA Game LUL");

		NA_FATAL_ERROR(p.mWindow != INVALID_WINDOW, "Failed to create main window.");

		NGAInitParams ngaParams;
		NGAInitialize(ngaParams);
		
		bool success = NA_Renderer->Initialize(p);
		NA_FATAL_ERROR(success, "Failed to initialize renderer.");

		if (!ImguiRendererSystemInit()) {
			// Not really fatal
			NA_ASSERT(false, "Failed to initialize ImGui renderer.");
		}

		return true;
	}

	bool RenderingSystemInitLate()
	{
		FRenderer.Initialize();

		return true;
	}

	void RenderingSystemShutdown()
	{
		FRenderer.Shutdown();

		ImguiRendererSystemShutdown();

		NA_Renderer->Shutdown();

		NGAShutdown();
	}

	void RenderingSystemBeginFrame()
	{
		NA_Renderer->BeginRender();
		ImguiRendererBeginFrame();
	}

	void RenderingSystemDoFrame()
	{
		Scene *mainScene = Scene::Get();

		const auto &cameras = mainScene->GetCameras();

		// Make sure there's only a single camera rendering to the back buffer.
		size_t numMainCameras = std::count_if(cameras.begin(), cameras.end(), [](const Camera* a) { return a->mEnabled && a->mRenderTarget == nullptr; });
		NA_ASSERT(numMainCameras == 1, "Only one camera can render to the back buffer.");

		for (const auto& camera : cameras) {
			if (!camera->mEnabled) {
				continue;
			}

			FRenderer.BeginRender();
			FRenderer.RenderScene(*mainScene, *camera);
			FRenderer.EndRender();
		}
	}

	void RenderingSystemEndFrame()
	{
		ImguiRendererEndFrame();
		NA_Renderer->EndRender();
	}

}