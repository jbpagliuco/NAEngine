#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
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

	class Texture : public AssetFactory<Texture>
	{
	public:
		bool Initialize(const std::string &filename);
		void Shutdown();

		PlatformShaderResourceView* GetShaderResourceView()const;
		PlatformSamplerState* GetSampler()const;

	private:
		PlatformResource *mRes;
		PlatformShaderResourceView *mSRV;
		PlatformSamplerState *mSampler;
	};
}