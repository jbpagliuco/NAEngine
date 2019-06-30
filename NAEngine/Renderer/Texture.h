#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#include <DirectXMath.h>
#endif

#include <string>

#include "Base/Streaming/AssetFactory.h"

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11Resource PlatformResource;
	typedef ID3D11ShaderResourceView PlatformShaderResourceView;
	typedef ID3D11SamplerState PlatformSamplerState;
#endif

	enum class TextureFilter {
#if defined(NA_D3D11)
		POINT = D3D11_FILTER_MIN_MAG_MIP_POINT,
		POINT_MIN_MAG = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
		POINT_MIN_MIP = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
		POINT_MAG_MIP = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,

		LINEAR = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		LINEAR_MIN_MAG = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
		LINEAR_MIN_MIP = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		LINEAR_MAG_MIP = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,

		ANISOTROPIC = D3D11_FILTER_ANISOTROPIC,
#endif
	};

	enum class TextureAddressMode {
#if defined(NA_D3D11)
		WRAP = D3D11_TEXTURE_ADDRESS_WRAP,
		MIRROR = D3D11_TEXTURE_ADDRESS_MIRROR,
		CLAMP = D3D11_TEXTURE_ADDRESS_CLAMP,
		BORDER = D3D11_TEXTURE_ADDRESS_BORDER,
		MIRROR_ONCE = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE,
#endif
	};

	struct SamplerStateParameters
	{
		TextureFilter mFilter = TextureFilter::LINEAR;

		TextureAddressMode mAddressU = TextureAddressMode::WRAP;
		TextureAddressMode mAddressV = TextureAddressMode::WRAP;
		TextureAddressMode mAddressW = TextureAddressMode::WRAP;

		DirectX::XMFLOAT4 mBorderColor = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	};

	class Texture : public AssetFactory<Texture>
	{
	public:
		bool Initialize(const std::string &filename, SamplerStateParameters samplerStateParams);
		void Shutdown();

		PlatformShaderResourceView* GetShaderResourceView()const;
		PlatformSamplerState* GetSampler()const;

	private:
		PlatformResource *mRes;
		PlatformShaderResourceView *mSRV;
		PlatformSamplerState *mSampler;
	};
}