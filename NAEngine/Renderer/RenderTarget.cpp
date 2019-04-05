#include "RenderTarget.h"

#include "Base/Debug/Assert.h"

#include "RendererD3D.h"

namespace na
{
	bool RenderTarget::Initialize()
	{
#if defined(NA_D3D11)
		HRESULT hr;

		// TODO: Probably replace this.
		ID3D11Texture2D *backBuffer;
		hr = NA_RSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to get back buffer.");

		hr = NA_RDevice->CreateRenderTargetView(backBuffer, nullptr, &mRTView);

		backBuffer->Release();

		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create render target view.");

		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = NA_Renderer->GetWindow().width;
		texDesc.Height = NA_Renderer->GetWindow().height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		hr = NA_RDevice->CreateTexture2D(&texDesc, nullptr, &mDSBuffer);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create depth stencil buffer");

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = NA_RDevice->CreateDepthStencilView(mDSBuffer, &depthStencilViewDesc, &mDSView);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create a depth stencil view");
#endif

		return true;
	}

	void RenderTarget::Shutdown()
	{
		NA_SAFE_RELEASE(mDSView);
		NA_SAFE_RELEASE(mDSBuffer);
		NA_SAFE_RELEASE(mRTView);
	}

	void RenderTarget::Clear(const float *clearColor)
	{
#if defined(NA_D3D11)
		NA_RContext->ClearRenderTargetView(mRTView, clearColor);
		NA_RContext->ClearDepthStencilView(mDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
#endif
	}

	void RenderTarget::Bind()
	{
#if defined(NA_D3D11)
		NA_RContext->OMSetRenderTargets(1, &mRTView, mDSView);
#endif
	}
}