#pragma once

#include "Base/DataStructures/Singleton.h"
#include "Base/OS/OS.h"

#include "RenderDefs.h"
#include "StateData.h"
#include "Renderer/NGA/NGARasterizer.h"
#include "Renderer/NGA/NGAResourceViews.h"
#include "Renderer/NGA/NGASwapChain.h"

#define NA_RStateData Renderer::Get()->GetStateData()
#if defined(NA_D3D11)
#include "NGA/DX11/NGACoreInternalDX11.h"
#define NA_Renderer Renderer::Get()
#define NA_RSwapChain Renderer::Get()->GetStateData()
#define NA_RDevice na::NgaDx11State.mDevice
#define NA_RContext na::NgaDx11State.mContext
#endif

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

		StateData* GetStateData() { return &mStateData; }

	protected:
		Window mWindow;
		NGASwapChain mSwapChain;

		NGARenderTargetView mRenderTargetView;
		NGADepthStencilView mDepthStencilView;

		NGARasterizerState mRasterizerState;

		StateData mStateData;

		Camera* mActiveCamera;
	};
}