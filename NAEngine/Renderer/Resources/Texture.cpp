#include "Texture.h"

#include "Base/Util/String.h"
#include "Renderer/Renderer.h"
#include "Vendor/DirectXTK/Include/DDSTextureLoader.h"

namespace na
{
	NA_FACTORY_SETUP(Texture);

	bool Texture::Initialize(const TextureDesc& textureDesc, bool releaseTextureReference)
	{
		if (textureDesc.mTextureDesc.mBindFlags & NGA_TEXTURE_BIND_SHADER_RESOURCE) {
			bool success = mSampler.Construct(textureDesc.mSamplerStateDesc);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct sampler state for texture.");
		}

		bool success = mTexture.Construct(textureDesc.mTextureDesc, nullptr);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct texture.");

		success = CreateViews(textureDesc);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create texure views.");

		// We might not need to keep a reference to our texture.
		if (releaseTextureReference) {
			mTexture.Destruct();
		}

		return true;
	}

	bool Texture::Initialize(const TextureDesc &textureDesc, const std::string &filename, bool releaseTextureReference)
	{
		if (textureDesc.mTextureDesc.mBindFlags & NGA_TEXTURE_BIND_SHADER_RESOURCE) {
			bool success = mSampler.Construct(textureDesc.mSamplerStateDesc);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct sampler state for texture %s", filename.c_str());
		}

		bool success = mTexture.ConstructFromFile(textureDesc.mTextureDesc, filename.c_str());
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to construct texture %s", filename.c_str());

		success = CreateViews(textureDesc);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create texure views for texture %s.", filename.c_str());

		// We might not need to keep a reference to our texture.
		if (releaseTextureReference) {
			mTexture.Destruct();
		}

		return true;
	}

	bool Texture::Initialize(const NGASwapChain& swapChain)
	{
		bool success = mRenderTargetView.Construct(swapChain);
		NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target view from swap chain.");

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


	bool Texture::CreateViews(const TextureDesc& textureDesc)
	{
		const bool isRenderTarget = textureDesc.mTextureDesc.mBindFlags & NGA_TEXTURE_BIND_RENDER_TARGET;
		const bool isDepthStencil = textureDesc.mTextureDesc.mBindFlags & NGA_TEXTURE_BIND_DEPTH_STENCIL;
		NA_RENDER_ASSERT_RETURN_VALUE(!(isRenderTarget && isDepthStencil), "Texture cannot be bound as both render target and depth stencil view.");

		if (textureDesc.mTextureDesc.mBindFlags & NGA_TEXTURE_BIND_SHADER_RESOURCE) {
			bool success = mShaderResourceView.Construct(mTexture);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create shader resource view.");
		}

		if (isRenderTarget) {
			bool success = mRenderTargetView.Construct(mTexture);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create render target view.");
		}

		if (isDepthStencil) {
			bool success = mDepthStencilView.Construct(mTexture);
			NA_RENDER_ASSERT_RETURN_VALUE(success, false, "Failed to create depth stencil view.");
		}

		return true;
	}
}