#include "StateData.h"

#include <d3d11.h>

#include "Rect.h"
#include "RendererD3D.h"

namespace na
{
	StateData RendererStateData;

	void StateData::Shutdown()
	{
		NA_SAFE_RELEASE(mRasterizerState);
	}

	void StateData::SetViewport(const Rect &rect)
	{
		D3D11_VIEWPORT vp;
		vp.TopLeftX = rect.x;
		vp.TopLeftY = rect.y;
		vp.Width = rect.w;
		vp.Height = rect.h;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		NA_RContext->RSSetViewports(1, &vp);
	}

	void StateData::SetRasterizerState()
	{
		if (mRasterizerState == nullptr) {
			D3D11_RASTERIZER_DESC desc{};
			desc.AntialiasedLineEnable = false;
			desc.CullMode = D3D11_CULL_BACK;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0.0f;
			desc.DepthClipEnable = true;
			desc.FillMode = D3D11_FILL_SOLID;
			desc.FrontCounterClockwise = false;
			desc.MultisampleEnable = false;
			desc.ScissorEnable = false;
			desc.SlopeScaledDepthBias = 0.0f;

			HRESULT hr = NA_RDevice->CreateRasterizerState(&desc, &mRasterizerState);
			NA_ASSERT(SUCCEEDED(hr), "Failed to create rasterizer state.");
		}

		NA_RContext->RSSetState(mRasterizerState);
	}
}