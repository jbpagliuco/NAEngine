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
		bool Initialize(Shader *shader, void *parameterData, size_t parameterByteLength, const std::vector<const Texture*> &textures);
		virtual void Shutdown() override;

		virtual void Bind() override;

		virtual AssetID GetID()const override { return AssetFactory<StaticMaterial>::GetID(); }
		virtual int GetMaterialType()const override { return MATERIAL_TYPE_STATIC; }

		const std::vector<const Texture*>& GetTextures()const { return mTextures; }

	private:
		ConstantBuffer mConstantBuffer;

		std::vector<const Texture*> mTextures;
	};
}