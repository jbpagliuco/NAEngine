#include "Texture.h"

#include "Base/Util/String.h"
#include "Renderer/Renderer.h"
#include "Vendor/DirectXTK/Include/DDSTextureLoader.h"

namespace na
{
	NA_FACTORY_SETUP(Texture);

	bool Texture::Initialize(const std::string &filename, const NGASamplerStateDesc &samplerStateDesc)
	{
		bool success = mSampler.Construct(samplerStateDesc);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to construct sampler state for texture %s", filename.c_str());

		NGATextureDesc desc;
		desc.mType = NGATextureType::TEXTURE2D;
		desc.mUsage = NGAUsage::IMMUTABLE;
		desc.mIsShaderResource = true;
		success = mTexture.ConstructFromFile(desc, filename.c_str());
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to construct texture %s", filename.c_str());

		success = mSRV.Construct(mTexture);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to construct shader resource view for texture %s", filename.c_str());
		
		return true;
	}

	void Texture::Shutdown()
	{
		mSRV.Destruct();
		mTexture.Destruct();
		mSampler.Destruct();
	}

	const NGATexture& Texture::GetResource()const
	{
		return mTexture;
	}

	const NGAShaderResourceView& Texture::GetShaderResourceView()const
	{
		return mSRV;
	}

	const NGASamplerState& Texture::GetSamplerState()const
	{
		return mSampler;
	}
}