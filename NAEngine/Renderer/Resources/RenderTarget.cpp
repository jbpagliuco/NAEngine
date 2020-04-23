#include "RenderTarget.h"

#include "Base/Util/String.h"
#include "Renderer/Renderer.h"
#include "Vendor/DirectXTK/Include/DDSTextureLoader.h"

namespace na
{
	bool RenderTarget::Initialize(const RenderTargetDesc& desc)
	{
		bool success = mRenderTargetView.Construct(*NA_RSwapChain);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target view from back buffer.");

		if (desc.mDepthBufferFormat != NGADepthBufferFormat::NONE) {
			NGADepthStencilViewDesc dsvDesc;
			dsvDesc.mFormat = desc.mDepthBufferFormat;
			dsvDesc.mWidth = desc.mWidth;
			dsvDesc.mHeight = desc.mHeight;
			success = mDepthStencilView.Construct(dsvDesc);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create depth stencil view.");
		}

		return true;
	}

	bool RenderTarget::Initialize(const RenderTargetDesc &desc, const NGASamplerStateDesc &samplerStateDesc)
	{
		bool success = mSampler.Construct(samplerStateDesc);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct sampler state for render target.");

		NGATextureDesc texDesc;
		texDesc.mType = NGATextureType::TEXTURE2D;
		texDesc.mFormat = NGAFormat::R32G32B32A32_FLOAT;
		texDesc.mUsage = NGAUsage::GPU_WRITE;
		texDesc.mWidth = desc.mWidth;
		texDesc.mHeight = desc.mHeight;
		texDesc.mIsRenderTarget = true;
		texDesc.mIsShaderResource = true;

		success = mTexture.Construct(texDesc, nullptr);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct render target texture.");

		success = mRenderTargetView.Construct(mTexture);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct render target view.");

		if (desc.mDepthBufferFormat != NGADepthBufferFormat::NONE) {
			NGADepthStencilViewDesc dsvDesc;
			dsvDesc.mFormat = desc.mDepthBufferFormat;
			dsvDesc.mWidth = desc.mWidth;
			dsvDesc.mHeight = desc.mHeight;

			success = mDepthStencilView.Construct(dsvDesc);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct depth stencil view.");
		}

		success = mShaderResourceView.Construct(mTexture);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct shader resource view.");


		return true;
	}

	void RenderTarget::Shutdown()
	{
		mShaderResourceView.Destruct();
		mDepthStencilView.Destruct();
		mRenderTargetView.Destruct();
		mTexture.Destruct();
		mSampler.Destruct();
	}

	const NGATexture& RenderTarget::GetResource()const
	{
		return mTexture;
	}

	const NGASamplerState& RenderTarget::GetSamplerState()const
	{
		return mSampler;
	}

	const NGARenderTargetView& RenderTarget::GetRenderTargetView()const
	{
		return mRenderTargetView;
	}

	const NGADepthStencilView& RenderTarget::GetDepthStencilView()const
	{
		return mDepthStencilView;
	}

	const NGAShaderResourceView& RenderTarget::GetShaderResourceView()const
	{
		return mShaderResourceView;
	}
}