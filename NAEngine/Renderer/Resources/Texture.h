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
		bool Initialize(const NGATextureDesc &textureDesc, const NGASamplerStateDesc &samplerStateDesc);
		bool Initialize(const NGATextureDesc &textureDesc, const NGASamplerStateDesc &samplerStateDesc, const std::string &filename);
		bool Initialize(const NGATextureDesc &textureDesc, const NGASwapChain &swapChain);
		void Shutdown();

		const NGATexture& GetResource()const;
		const NGASamplerState& GetSamplerState()const;

		const NGARenderTargetView& GetRenderTargetView()const;
		const NGADepthStencilView& GetDepthStencilView()const;
		const NGAShaderResourceView& GetShaderResourceView()const;

	private:
		bool CreateViews(const NGATextureDesc &textureDesc);

	private:
		NGATexture mTexture;
		NGASamplerState mSampler;

		NGARenderTargetView mRenderTargetView;
		NGADepthStencilView mDepthStencilView;
		NGAShaderResourceView mShaderResourceView;
	};
}