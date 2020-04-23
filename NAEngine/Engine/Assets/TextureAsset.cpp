#include "TextureAsset.h"

#include "Base/Streaming/Stream.h"
#include "Base/Util/Serialize.h"
#include "Renderer/Resources/Texture.h"

namespace na
{
	static bool OnTextureTexxLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header);
	static bool OnTextureDDSLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header);
	static void OnTextureUnload(const AssetID &id);

	bool TextureAssetSystemInit()
	{
		AssetType texType;
		texType.mExt = "texx";
		texType.mOnLoad = OnTextureTexxLoad;
		texType.mOnUnload = OnTextureUnload;
		RegisterAssetType(texType);

		AssetType ddsType;
		ddsType.mExt = "dds";
		ddsType.mOnLoad = OnTextureDDSLoad;
		ddsType.mOnUnload = OnTextureUnload;
		RegisterAssetType(ddsType);

		return true;
	}

	void TextureAssetSystemShutdown()
	{
		NA_ASSERT(Texture::NumInstances() == 0, "There were still textures allocated during shutdown!");
		Texture::ReleaseAll();
	}


	static bool OnTextureTexxLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header)
	{
		/*Texture *pTex = Texture::Create(id);
		NA_ASSERT_RETURN_VALUE(pTex != nullptr, false, "Failed to allocate texture");

		DeserializationParameterMap params = ParseFile(filename);

		const std::string textureFile = params["textureFile"].AsFilepath();

		static std::map<std::string, TextureFilter> _filterConv = {
			{ "point", TextureFilter::POINT },
			{ "point_min_mag", TextureFilter::POINT_MIN_MAG },
			{ "point_min_mip", TextureFilter::POINT_MIN_MIP },
			{ "point_mag_mip", TextureFilter::POINT_MAG_MIP },
			{ "linear", TextureFilter::LINEAR },
			{ "linear_min_mag", TextureFilter::LINEAR_MIN_MAG },
			{ "linear_min_mip", TextureFilter::LINEAR_MIN_MIP },
			{ "linear_mag_mip", TextureFilter::LINEAR_MAG_MIP },
			{ "anisotropic", TextureFilter::ANISOTROPIC },
		};

		static std::map<std::string, TextureAddressMode> _addressModeConv = {
			{ "wrap", TextureAddressMode::WRAP },
			{ "mirror", TextureAddressMode::MIRROR },
			{ "clamp", TextureAddressMode::CLAMP },
			{ "border", TextureAddressMode::BORDER },
			{ "mirror_once", TextureAddressMode::MIRROR_ONCE },
		};

		DeserializationParameterMap samplerStateParamMap = params["sampler"];

		SamplerStateParameters samplerStateParams;
		samplerStateParams.mFilter = _filterConv[samplerStateParamMap["filter"].AsString("point")];
		samplerStateParams.mAddressU = _addressModeConv[samplerStateParamMap["addressU"].AsString("wrap")];
		samplerStateParams.mAddressV = _addressModeConv[samplerStateParamMap["addressV"].AsString("wrap")];
		samplerStateParams.mAddressW = _addressModeConv[samplerStateParamMap["addressW"].AsString("wrap")];
		samplerStateParams.mBorderColor = samplerStateParamMap.AsColor();
		
		return pTex->Initialize(textureFile, samplerStateParams);*/

		return false;
	}

	static bool OnTextureDDSLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header)
	{
		Texture *pTex = Texture::Create(id);
		NA_ASSERT_RETURN_VALUE(pTex != nullptr, false, "Failed to allocate texture");

		// Create the texture right from a DDS file, with the normal sampler state parameters
		return pTex->Initialize(filename, NGASamplerStateDesc());
	}

	static void OnTextureUnload(const AssetID &id)
	{
		Texture::Release(id);
	}
}