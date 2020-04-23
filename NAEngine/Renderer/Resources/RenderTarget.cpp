#include "RenderTarget.h"

#include "Base/Util/String.h"
#include "Renderer/Renderer.h"
#include "Vendor/DirectXTK/Include/DDSTextureLoader.h"

namespace na
{
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

		success = mRTV.Construct(mTexture);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct render target view.");

		success = mSRV.Construct(mTexture);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct shader resource view.");

		if (desc.mDepthBufferFormat != NGADepthBufferFormat::NONE) {
			NGADepthStencilViewDesc dsvDesc;
			dsvDesc.mFormat = desc.mDepthBufferFormat;
			dsvDesc.mWidth = desc.mWidth;
			dsvDesc.mHeight = desc.mHeight;
			
			success = mDSV.Construct(dsvDesc);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct depth stencil view.");
		}

		return true;
	}

	void RenderTarget::Shutdown()
	{
		mDSV.Destruct();
		mSRV.Destruct();
		mRTV.Destruct();
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
		return mRTV;
	}

	const NGAShaderResourceView& RenderTarget::GetShaderResourceView()const
	{
		return mSRV;
	}
}