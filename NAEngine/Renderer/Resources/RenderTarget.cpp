#include "RenderTarget.h"

#include "Renderer/RenderDefs.h"
#include "Renderer/NGA/NGASwapChain.h"

namespace na
{
	NA_FACTORY_SETUP(RenderTarget);

	RenderTargetTextureDesc::operator TextureDesc()const
	{
		TextureDesc desc;
		desc.mTextureDesc.mFormat = mFormat;
		desc.mTextureDesc.mType = mType;
		desc.mTextureDesc.mUsage = mUsage;
		desc.mTextureDesc.mBindFlags = (mShaderResource) ? NGA_TEXTURE_BIND_SHADER_RESOURCE : NGA_TEXTURE_BIND_NONE;

		return desc;
	}

	bool RenderTarget::Initialize(RenderTargetDesc desc)
	{
		if (desc.mUseColorMap) {
			TextureDesc colorMapDesc;
			colorMapDesc = desc.mColorMapDesc;
			colorMapDesc.mTextureDesc.mWidth = desc.mWidth;
			colorMapDesc.mTextureDesc.mHeight = desc.mHeight;
			colorMapDesc.mTextureDesc.mBindFlags |= NGA_TEXTURE_BIND_RENDER_TARGET;

			bool success = mColorMap.Initialize(colorMapDesc, true);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target color map.");
		}

		if (desc.mUseDepthMap) {
			TextureDesc depthMapDesc;
			depthMapDesc = desc.mDepthMapDesc;
			depthMapDesc.mTextureDesc.mWidth = desc.mWidth;
			depthMapDesc.mTextureDesc.mHeight = desc.mHeight;
			depthMapDesc.mTextureDesc.mBindFlags |= NGA_TEXTURE_BIND_DEPTH_STENCIL;

			bool success = mDepthMap.Initialize(depthMapDesc, true);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target depth map.");
		}

		return true;
	}

	bool RenderTarget::Initialize(const NGASwapChain& swapChain, const RenderTargetTextureDesc& depthMapDesc, int width, int height)
	{
		bool success = mColorMap.Initialize(swapChain);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target color map.");

		TextureDesc desc = depthMapDesc;
		desc.mTextureDesc.mBindFlags |= NGA_TEXTURE_BIND_DEPTH_STENCIL;
		desc.mTextureDesc.mWidth = width;
		desc.mTextureDesc.mHeight = height;

		success = mDepthMap.Initialize(desc, true);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target depth map.");

		return true;
	}

	void RenderTarget::Shutdown()
	{
		mColorMap.Shutdown();
		mDepthMap.Shutdown();
	}

	const Texture& RenderTarget::GetColorMap()const
	{
		return mColorMap;
	}

	const Texture& RenderTarget::GetDepthMap()const
	{
		return mDepthMap;
	}
}