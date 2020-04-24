#pragma once

#include "Base/DataStructures/Singleton.h"
#include "Base/OS/OS.h"

#include "RenderDefs.h"
#include "StateManager.h"
#include "Renderer/NGA/NGAResourceViews.h"
#include "Renderer/NGA/NGASwapChain.h"
#include "Renderer/NGA/NGAPipelineState.h"
#include "Renderer/Resources/RenderTarget.h"
#include "Renderer/Resources/Texture.h"

#define NA_Renderer Renderer::Get()
#define NA_RStateManager Renderer::Get()->GetStateManager()
#define NA_RSwapChain Renderer::Get()->GetSwapChain()
#define NA_RMainRenderTarget Renderer::Get()->GetRenderTarget()

namespace na
{
	class Camera;

	struct RendererInitParams
	{
		Window mWindow;
		int mWidth;
		int mHeight;
	};

	class Renderer : public Singleton<Renderer>
	{
	public:
		virtual bool Initialize(const RendererInitParams &params);
		virtual void Shutdown();

		virtual void BeginRender();
		virtual void EndRender();

		const Window& GetWindow() { return mWindow; }

		StateManager* GetStateManager() { return &mStateManager; }
		NGASwapChain* GetSwapChain() { return &mSwapChain; }
		RenderTarget* GetRenderTarget() { return &mMainRenderTarget; }
		
	private:
		Window mWindow;
		NGASwapChain mSwapChain;

		RenderTarget mMainRenderTarget;
		
		StateManager mStateManager;
	};
}