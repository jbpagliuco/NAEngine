#pragma once

#include <vector>

#include "Renderer/Resources/ConstantBuffer.h"
#include "Material.h"

namespace na
{
	class Texture;

	class StaticMaterial : public Material, public AssetFactory<StaticMaterial>
	{
	public:
		bool Initialize(AssetID shaderID, void *parameterData, size_t parameterByteLength, const std::vector<AssetID> &textures);
		virtual void Shutdown() override;

		virtual void Bind() override;

		virtual int GetMaterialType()const override { return MATERIAL_TYPE_STATIC; }

	private:
		ConstantBuffer mConstantBuffer;

		std::vector<Texture*> mTextures;
	};
}