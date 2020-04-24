#pragma once

#include <map>

#include "Base/Streaming/Stream.h"

#include "Renderer/Material/MaterialContainer.h"

namespace na
{
	class Material;
	class RenderTarget;
	class Texture;

	// Wrapper over a material container to manage asset requests.
	class MaterialManager
	{
	public:
		void Initialize(const std::string &materialFilename);
		void Shutdown();

		MaterialContainer* GetMaterialContainer();

		bool SetFloatParameter(const std::string &name, float value);
		bool SetVectorParameter(const std::string& name, const Vector4f& value);

		bool SetTextureParameter(const std::string& name, const std::string &filename);
		bool SetTextureParameter(const std::string &name, Texture *pTexture);

		bool SetRenderTargetParameter(const std::string& name, const std::string &filename, bool useColorMap);
		bool SetRenderTargetParameter(const std::string& name, RenderTarget *renderTarget, bool useColorMap);

	private:
		void Release(const std::string& name);

	private:
		Material* mMaterial;
		MaterialContainer mMaterialContainer;

		std::map<std::string, AssetID> mAssets;
	};
}