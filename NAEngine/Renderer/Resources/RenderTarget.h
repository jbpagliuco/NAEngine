#pragma once

#include "Base/Streaming/AssetFactory.h"

#include "Renderer/Resources/Texture.h"

namespace na
{
	struct RenderTargetTextureDesc
	{
		NGATextureType mType = NGATextureType::TEXTURE2D;
		NGAFormat mFormat = NGAFormat::UNKNOWN;
		NGAUsage mUsage = NGAUsage::GPU_WRITE;

		bool mShaderResource = false;

		operator TextureDesc()const;
	};

	struct RenderTargetDesc
	{
		int mWidth;
		int mHeight;

		bool mUseColorMap;
		bool mUseDepthMap;

		RenderTargetTextureDesc mColorMapDesc;
		RenderTargetTextureDesc mDepthMapDesc;
	};

	class RenderTarget : public AssetFactory<RenderTarget>
	{
	public:
		bool Initialize(RenderTargetDesc desc);
		bool Initialize(const NGASwapChain &swapChain, const RenderTargetTextureDesc &depthMapDesc, int width, int height);
		void Shutdown();

		const Texture& GetColorMap()const;
		const Texture& GetDepthMap()const;

	private:
		Texture mColorMap;
		Texture mDepthMap;
	};
}