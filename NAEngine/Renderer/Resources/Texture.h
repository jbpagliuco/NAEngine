#pragma once

#include <string>

#include "Base/Streaming/AssetFactory.h"

#include "Renderer/NGA/NGAResources.h"
#include "Renderer/NGA/NGAResourceViews.h"
#include "Renderer/NGA/NGASamplerState.h"

namespace na
{
	struct TextureDesc
	{
		NGATextureDesc mTextureDesc;
		NGASamplerStateDesc mSamplerStateDesc;
	};

	class Texture : public AssetFactory<Texture>
	{
	public:
		bool Initialize(const TextureDesc& textureDesc, bool releaseTextureReference);
		bool Initialize(const TextureDesc& textureDesc, const std::string& filename, bool releaseTextureReference);
		bool Initialize(const NGASwapChain& swapChain);
		void Shutdown();

		const NGATexture& GetResource()const;
		const NGASamplerState& GetSamplerState()const;

		const NGARenderTargetView& GetRenderTargetView()const;
		const NGADepthStencilView& GetDepthStencilView()const;
		const NGAShaderResourceView& GetShaderResourceView()const;

	private:
		bool CreateViews(const TextureDesc& textureDesc);

	private:
		NGATexture mTexture;
		NGASamplerState mSampler;

		NGAShaderResourceView mShaderResourceView;

		// A texture can either have a render target view or a depth stencil view, but never both.
		NGARenderTargetView mRenderTargetView;
		NGADepthStencilView mDepthStencilView;
	};
}