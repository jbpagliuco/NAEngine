#include "TextureAsset.h"

#include "Base/Streaming/Stream.h"
#include "Base/Util/Serialize.h"
#include "Renderer/Texture.h"

namespace na
{
	static bool OnTextureLoad(const AssetID &id, const std::string &filename);
	static void OnTextureUnload(const AssetID &id);

	bool TextureAssetSystemInit()
	{
		AssetType texType;
		texType.mExt = "texx";
		texType.mOnLoad = OnTextureLoad;
		texType.mOnUnload = OnTextureUnload;
		RegisterAssetType(texType);

		return true;
	}

	void TextureAssetSystemShutdown()
	{
		NA_ASSERT(Texture::NumInstances() == 0, "There were still textures allocated during shutdown!");
		Texture::ReleaseAll();
	}


	static bool OnTextureLoad(const AssetID &id, const std::string &filename)
	{
		Texture *pTex = Texture::Create(id);
		NA_ASSERT_RETURN_VALUE(pTex != nullptr, false, "Failed to allocate texture");

		DeserializationParameterMap params = ParseFile(filename);
		
		return pTex->Initialize(params["textureFile"].AsFilepath());
	}

	static void OnTextureUnload(const AssetID &id)
	{
		Texture::Release(id);
	}
}