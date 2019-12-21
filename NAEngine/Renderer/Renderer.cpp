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

		success = mRenderTargetView.Construct(mSwapChain);
		NA_FATAL_ERROR(success, "Failed to create main render target view.");

		success = mDepthStencilView.Construct(params.mWidth, params.mHeight);
		NA_FATAL_ERROR(success, "Failed to create main depth stencil view.");

		NGARasterizerStateDesc rasterizerDesc;
		rasterizerDesc.mAntialias = true;
		rasterizerDesc.mCullMode = NGACullMode::CULL_BACK;
		rasterizerDesc.mFillMode = NGAFillMode::SOLID;
		rasterizerDesc.mFrontCounterClockwise = false;
		success = mRasterizerState.Construct(rasterizerDesc);
		NA_FATAL_ERROR(success, "Failed to create main rasterizer state.");

		if (!mStateData.Initialize()) {
			return false;
		}

		return true;
	}

	void Renderer::Shutdown()
	{
		mStateData.Shutdown();
		mDepthStencilView.Destruct();
		mRenderTargetView.Destruct();
		mSwapChain.Destruct();
	}

	void Renderer::BeginRender()
	{
		mStateData.BindRenderTarget(mRenderTargetView, mDepthStencilView);

		const ColorF clearColor = COLOR_CORNFLOWERBLUE;
		mStateData.ClearRenderTarget(mRenderTargetView, clearColor.vArray);
		mStateData.ClearDepthStencilView(mDepthStencilView);

		Rect r;
		r.x = 0.0f;
		r.y = 0.0f;
		r.w = (float)mWindow.width;
		r.h = (float)mWindow.height;

		mStateData.SetViewport(r);

		mStateData.SetRasterizerState(mRasterizerState);

		if (mActiveCamera != nullptr) {
			mStateData.SetViewProjMatrices(
				mActiveCamera->mTransform.GetMatrix().Inverted(),
				Matrix::PerspectiveFOVLH(mActiveCamera->mFOV, mWindow.GetAspectRatio(), mActiveCamera->mNear, mActiveCamera->mFar)
			);
		}
	}

	void Renderer::EndRender()
	{
		mSwapChain.Present();
	}

	void Renderer::SetActiveCamera(Camera *camera)
	{
		mActiveCamera = camera;
	}

	Camera* Renderer::GetActiveCamera()
	{
		return mActiveCamera;
	}
}