#include "NGA/NGARasterizer.h"

#if defined(NGA_D3D11)

#include "NGACoreInternalDX11.h"

namespace na
{
	NGA_GPU_CLASS_IMPLEMENT(NGARasterizerState);

	bool NGARasterizerState::Construct(const NGARasterizerStateDesc &desc)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false);
		
		D3D11_RASTERIZER_DESC rs{};
		rs.AntialiasedLineEnable = desc.mAntialias;
		rs.CullMode = (D3D11_CULL_MODE)desc.mCullMode;
		rs.DepthBias = 0;
		rs.DepthBiasClamp = 0.0f;
		rs.DepthClipEnable = true;
		rs.FillMode = (D3D11_FILL_MODE)desc.mFillMode;
		rs.FrontCounterClockwise = desc.mFrontCounterClockwise;
		rs.MultisampleEnable = false;
		rs.ScissorEnable = false;
		rs.SlopeScaledDepthBias = 0.0f;

		HRESULT hr = NgaDx11State.mDevice->CreateRasterizerState(&rs, &mRasterizerState);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "ID3D11Device::CreateRasterizerState() failed with HRESULT %X", hr);

		return true;
	}

	void NGARasterizerState::Destruct()
	{
		NGA_DX11_SAFE_RELEASE(mRasterizerState);
	}

	bool NGARasterizerState::IsConstructed()const
	{
		return mRasterizerState != nullptr;
	}
}

#endif