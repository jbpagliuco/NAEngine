#include "NGA/NGASamplerState.h"

#if defined(NGA_D3D11)

#include "NGACoreInternalDX11.h"

namespace na
{
	NGA_GPU_CLASS_IMPLEMENT(NGASamplerState);

	bool NGASamplerState::Construct(const NGASamplerStateDesc &desc)
	{
		D3D11_SAMPLER_DESC dxdesc;
		dxdesc.Filter = (D3D11_FILTER)desc.mFilter;
		dxdesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)desc.mAddressU;
		dxdesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)desc.mAddressV;
		dxdesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)desc.mAddressW;
		dxdesc.MipLODBias = 0.0f;
		dxdesc.MaxAnisotropy = 1;
		dxdesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		dxdesc.BorderColor[0] = desc.mBorderColor.x;
		dxdesc.BorderColor[1] = desc.mBorderColor.y;
		dxdesc.BorderColor[2] = desc.mBorderColor.z;
		dxdesc.BorderColor[3] = desc.mBorderColor.w;
		dxdesc.MinLOD = 0;
		dxdesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = NgaDx11State.mDevice->CreateSamplerState(&dxdesc, &mSamplerState);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "ID3D11Device::CreateSamplerState() failed with HRESULT %X", hr);

		return true;
	}

	void NGASamplerState::Destruct()
	{
		NGA_DX11_SAFE_RELEASE(mSamplerState);
	}

	bool NGASamplerState::IsConstructed()const
	{
		return mSamplerState != nullptr;
	}
}

#endif