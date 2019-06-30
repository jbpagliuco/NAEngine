#include "Texture.h"

#include "Base/Util/String.h"
#include "Renderer/RendererD3D.h"
#include "Vendor/DirectXTK/Include/DDSTextureLoader.h"

namespace na
{
	NA_FACTORY_SETUP(Texture);

	bool Texture::Initialize(const std::string &filename)
	{
		const HRESULT hr = DirectX::CreateDDSTextureFromFile(NA_RDevice, ToWideString(filename).c_str(), &mRes, &mSRV);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to load texture from file %s", filename.c_str());

		D3D11_SAMPLER_DESC desc;
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		NA_RDevice->CreateSamplerState(&desc, &mSampler);

		return true;
	}

	void Texture::Shutdown()
	{
		NA_SAFE_RELEASE(mRes);
		NA_SAFE_RELEASE(mSRV);
		NA_SAFE_RELEASE(mSampler);
	}

	PlatformShaderResourceView* Texture::GetShaderResourceView()const
	{
		return mSRV;
	}

	PlatformSamplerState* Texture::GetSampler()const
	{
		return mSampler;
	}
}