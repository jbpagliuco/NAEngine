#pragma once

#include "Base/DataStructures/Singleton.h"
#include "Base/OS/OS.h"

#include "RenderDefs.h"
#include "StateManager.h"
#include "Renderer/NGA/NGARasterizer.h"
#include "Renderer/NGA/NGAResourceViews.h"
#include "Renderer/NGA/NGASwapChain.h"

#define NA_Renderer Renderer::Get()
#define NA_RStateManager Renderer::Get()->GetStateManager()

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

		void SetActiveCamera(Camera *camera);
		Camera* GetActiveCamera();

		const Window& GetWindow() { return mWindow; }

		StateManager* GetStateManager() { return &mStateManager; }

	protected:
		Window mWindow;
		NGASwapChain mSwapChain;

		NGARenderTargetView mRenderTargetView;
		NGADepthStencilView mDepthStencilView;

		NGARasterizerState mRasterizerState;

		StateManager mStateManager;

		Camera* mActiveCamera;
	};
}