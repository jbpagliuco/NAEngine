#pragma once

#include <string>

#include "Base/Streaming/AssetFactory.h"

#include "Renderer/NGA/NGAResources.h"
#include "Renderer/NGA/NGAResourceViews.h"
#include "Renderer/NGA/NGASamplerState.h"

namespace na
{
	class Texture : public AssetFactory<Texture>
	{
	public:
		bool Initialize(const std::string &filename, const NGASamplerStateDesc &samplerStateDesc);
		void Shutdown();

		const NGATexture& GetResource()const;
		const NGAShaderResourceView& GetShaderResourceView()const;
		const NGASamplerState& GetSamplerState()const;

	private:
		NGATexture mTexture;
		NGAShaderResourceView mSRV;
		NGASamplerState mSampler;
	};
}