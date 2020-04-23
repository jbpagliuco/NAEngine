#include "Renderer.h"

#include "Base/Util/Color.h"

#include "Rect.h"
#include "Scene/Camera.h"

namespace na
{
	NA_CREATE_SINGLETON_INSTANCE(Renderer);

	bool Renderer::Initialize(const RendererInitParams &params)
	{
		mWindow = params.mWindow;
		
		NGASwapChainDesc swapChainDesc;
		swapChainDesc.mBufferCount = 2;
		swapChainDesc.mWindow = params.mWindow;
		bool success = mSwapChain.Construct(swapChainDesc);
		NA_FATAL_ERROR(success, "Failed to construct swap chain.");

		RenderTargetDesc rtDesc;
		rtDesc.mDepthBufferFormat = NGADepthBufferFormat::DEPTH24_STENCIL;
		rtDesc.mWidth = params.mWidth;
		rtDesc.mHeight = params.mHeight;
		success = mMainRenderTarget.Initialize(rtDesc);
		NA_FATAL_ERROR(success, "Failed to create main render target view.");

		if (!mStateManager.Initialize()) {
			return false;
		}

		return true;
	}

	void Renderer::Shutdown()
	{
		mStateManager.Shutdown();
		mMainRenderTarget.Shutdown();
		mSwapChain.Destruct();
	}

	void Renderer::BeginRender()
	{
		NGARect r;
		r.x = 0.0f;
		r.y = 0.0f;
		r.width = (float)mWindow.width;
		r.height = (float)mWindow.height;
		mStateManager.SetViewport(r);
	}

	void Renderer::EndRender()
	{
		mSwapChain.Present();
	}
}