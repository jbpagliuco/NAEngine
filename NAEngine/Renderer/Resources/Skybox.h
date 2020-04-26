#pragma once

#include "Base/Streaming/AssetFactory.h"

#include "Renderer/Resources/Texture.h"

namespace na
{
	class Skybox : public AssetFactory<Skybox>
	{
	public:
		bool Initialize(const std::string& filename);
		void Shutdown();

		const Texture& GetTexture()const;

	private:
		Texture mSkyboxTexture;
	};
}