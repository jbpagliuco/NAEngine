#pragma once

#include <string>

#include "Base/Streaming/AssetFactory.h"
#include "Renderer/Resources/Texture.h"

namespace na
{
	class TextureAsset : public AssetFactory<TextureAsset>
	{
	public:
		bool Initialize(const std::string& filename);
		void Shutdown();

		const Texture& GetTexture()const;

	private:
		Texture mTexture;
	};

	bool TextureAssetSystemInit();
	void TextureAssetSystemShutdown();
}