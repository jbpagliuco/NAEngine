#include "TextureAsset.h"

#include "Base/Streaming/Stream.h"
#include "Base/Util/Serialize.h"
#include "Renderer/Resources/RenderTarget.h"
#include "Renderer/Resources/Texture.h"

namespace na
{
	NA_FACTORY_SETUP(TextureAsset);


	static bool OnTextureTexxLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header);
	static bool OnTextureDDSLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header);
	static void OnTextureUnload(const AssetID &id);

	static bool OnRenderTargetLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header);
	static void OnRenderTargetUnload(const AssetID &id);




	bool TextureAsset::Initialize(const std::string& filename)
	{
		TextureDesc desc;
		desc.mTextureDesc.mBindFlags = NGA_TEXTURE_BIND_SHADER_RESOURCE;
		desc.mTextureDesc.mUsage = NGAUsage::IMMUTABLE;
		desc.mSamplerStateDesc = NGASamplerStateDesc();

		bool success = mTexture.Initialize(desc, filename, true);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create texture %s", filename.c_str());

		return true;
	}

	void TextureAsset::Shutdown()
	{
		mTexture.Shutdown();
	}

	const Texture& TextureAsset::GetTexture()const
	{
		return mTexture;
	}





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

		AssetType rtxType;
		rtxType.mExt = "rtx";
		rtxType.mOnLoad = OnRenderTargetLoad;
		rtxType.mOnUnload = OnRenderTargetUnload;
		RegisterAssetType(rtxType);

		return true;
	}

	void TextureAssetSystemShutdown()
	{
		NA_ASSERT(TextureAsset::ReportEmpty(), "There were still textures allocated during shutdown!");
		TextureAsset::ReleaseAll();
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
		TextureAsset *pTex = TextureAsset::Create(id);
		NA_ASSERT_RETURN_VALUE(pTex != nullptr, false, "Failed to allocate texture");

		// Create the texture right from a DDS file, with the normal sampler state parameters
		return pTex->Initialize(filename);
	}

	static void OnTextureUnload(const AssetID &id)
	{
		TextureAsset::Release(id);
	}




	static bool OnRenderTargetLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header)
	{
		RenderTarget *pRenderTarget = RenderTarget::Create(id);
		NA_ASSERT_RETURN_VALUE(pRenderTarget != nullptr, false, "Failed to allocate render target");

		DeserializationParameterMap params = ParseFile(filename);

		RenderTargetDesc desc;
		desc.mWidth = params["width"].AsInt();
		desc.mHeight = params["height"].AsInt();

		// Color map parameters
		if (params.HasChild("colorMap")) {
			DeserializationParameterMap colorMapParams = params["colorMap"];
			desc.mUseColorMap = true;
			desc.mColorMapDesc.mFormat = NGAFormat::R32G32B32A32_FLOAT;
			desc.mColorMapDesc.mType = NGATextureType::TEXTURE2D;
			desc.mColorMapDesc.mShaderResource = colorMapParams["shaderResource"].AsBool();
		}

		// Depth map parameters
		if (params.HasChild("depthMap")) {
			DeserializationParameterMap depthMapParams = params["depthMap"];
			desc.mUseDepthMap = true;
			desc.mDepthMapDesc.mType = NGATextureType::TEXTURE2D;
			desc.mDepthMapDesc.mShaderResource = depthMapParams["shaderResource"].AsBool();

			// Deduce format
			int depth = depthMapParams["depth"].AsInt(-1);
			bool stencil = depthMapParams["stencil"].AsBool();

			if (depth == 16 && !stencil) {
				desc.mDepthMapDesc.mFormat = NGAFormat::D16_UNORM;
			}
			else if (depth == 24 && stencil) {
				desc.mDepthMapDesc.mFormat = NGAFormat::D24_UNORM_S8_UINT;
			}
			else if (depth == 32 && !stencil) {
				desc.mDepthMapDesc.mFormat = NGAFormat::D32_FLOAT;
			}
			else {
				NA_ASSERT(false, "Unrecognized depth buffer format.");
			}

			if (desc.mDepthMapDesc.mShaderResource) {
				NA_ASSERT(desc.mDepthMapDesc.mFormat == NGAFormat::D32_FLOAT);
				desc.mDepthMapDesc.mFormat = NGAFormat::R32_TYPELESS;
			}
		}

		// Create the render target
		return pRenderTarget->Initialize(desc);
	}

	static void OnRenderTargetUnload(const AssetID &id)
	{
		RenderTarget::Release(id);
	}
}