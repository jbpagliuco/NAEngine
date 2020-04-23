#include "Texture.h"

#include "Base/Util/String.h"
#include "Renderer/Renderer.h"
#include "Vendor/DirectXTK/Include/DDSTextureLoader.h"

namespace na
{
	NA_FACTORY_SETUP(Texture);

	bool Texture::Initialize(const NGATextureDesc& textureDesc, const NGASamplerStateDesc& samplerStateDesc)
	{
		bool success = mSampler.Construct(samplerStateDesc);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct sampler state for texture.");

		success = mTexture.Construct(textureDesc, nullptr);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct texture.");

		success = CreateViews(textureDesc);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create texure views.");

		return true;
	}

	bool Texture::Initialize(const NGATextureDesc &textureDesc, const NGASamplerStateDesc &samplerStateDesc, const std::string &filename)
	{
		bool success = mSampler.Construct(samplerStateDesc);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct sampler state for texture %s", filename.c_str());

		success = mTexture.ConstructFromFile(textureDesc, filename.c_str());
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct texture %s", filename.c_str());

		success = CreateViews(textureDesc);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create texure views for texture %s.", filename.c_str());
		
		return true;
	}

	bool Texture::Initialize(const NGATextureDesc &textureDesc, const NGASwapChain& swapChain)
	{
		bool success = mRenderTargetView.Construct(swapChain);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target view from swap chain.");

		NGADepthStencilViewDesc dsvDesc;
		dsvDesc.mFormat = textureDesc.mDepthBufferFormat;
		dsvDesc.mWidth = textureDesc.mWidth;
		dsvDesc.mHeight = textureDesc.mHeight;
		success = mDepthStencilView.Construct(dsvDesc);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create depth stencil view from swap chain.");

		return true;
	}

	void Texture::Shutdown()
	{
		mShaderResourceView.Destruct();
		mDepthStencilView.Destruct();
		mRenderTargetView.Destruct();

		mTexture.Destruct();
		mSampler.Destruct();
	}


	const NGATexture& Texture::GetResource()const
	{
		return mTexture;
	}

	const NGASamplerState& Texture::GetSamplerState()const
	{
		return mSampler;
	}

	const NGARenderTargetView& Texture::GetRenderTargetView()const
	{
		return mRenderTargetView;
	}

	const NGADepthStencilView& Texture::GetDepthStencilView()const
	{
		return mDepthStencilView;
	}

	const NGAShaderResourceView& Texture::GetShaderResourceView()const
	{
		return mShaderResourceView;
	}


	bool Texture::CreateViews(const NGATextureDesc& textureDesc)
	{
		const bool isRenderTarget = textureDesc.mBindFlags & NGA_TEXTURE_BIND_RENDER_TARGET;
		const bool isDepthStencil = textureDesc.mBindFlags & NGA_TEXTURE_BIND_DEPTH_STENCIL;
		NA_RENDER_ASSERT_RETURN_VALUE(!(isRenderTarget && isDepthStencil), "Texture cannot be bound as both render target and depth stencil view.");

		if (textureDesc.mBindFlags & NGA_TEXTURE_BIND_SHADER_RESOURCE) {
			bool success = mShaderResourceView.Construct(mTexture);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create shader resource view.");
		}

		if (textureDesc.mBindFlags & NGA_TEXTURE_BIND_RENDER_TARGET) {
			bool success = mRenderTargetView.Construct(mTexture);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target view.");
		}

		if (textureDesc.mDepthBufferFormat != NGADepthBufferFormat::NONE) {
			NGADepthStencilViewDesc desc;
			desc.mFormat = textureDesc.mDepthBufferFormat;
			desc.mWidth = textureDesc.mWidth;
			desc.mHeight = textureDesc.mHeight;

			bool success = mDepthStencilView.Construct(desc);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create depth stencil view.");
		}

		return true;
	}
}