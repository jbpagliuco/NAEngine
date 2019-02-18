#include "RenderTarget.h"

#if defined(NA_D3D)

#include <d3d11.h>

#include "Base/Debug/Assert.h"

namespace na
{
	bool RenderTarget::Initialize()
	{
		HRESULT hr;

		ID3D11Texture2D *backBuffer;
		hr = NA_RSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to get back buffer.");

		hr = NA_RDevice->CreateRenderTargetView(backBuffer, nullptr, &mRTView);

		backBuffer->Release();

		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create render target view.");

		return true;
	}

	void RenderTarget::Shutdown()
	{
		NA_SAFE_RELEASE(mRTView);
	}

	void RenderTarget::Clear(const float *clearColor)
	{
		NA_RContext->ClearRenderTargetView(mRTView, clearColor);
	}

	void RenderTarget::Bind()
	{
		NA_RContext->OMSetRenderTargets(1, &mRTView, nullptr);
	}
}

#endif