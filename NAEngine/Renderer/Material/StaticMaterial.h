#pragma once

#include "Material.h"

#include <vector>

namespace na
{
	class Texture;

	class StaticMaterial : public Material, public AssetFactory<StaticMaterial>
	{
	public:
		bool Initialize(AssetID shaderID, void *parameterData, size_t parameterByteLength, const std::vector<AssetID> &textures);
		
		virtual void Shutdown() override;

		virtual int GetMaterialType()const override { return MATERIAL_TYPE_STATIC; }
	};
}