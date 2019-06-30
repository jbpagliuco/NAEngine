#pragma once

#include "Material.h"

#include "Renderer/ConstantBuffer.h"

namespace na
{
	class StaticMaterial : public Material, public AssetFactory<StaticMaterial>
	{
	public:
		bool Initialize(AssetID shaderID, void *parameterData, size_t parameterByteLength);
		
		virtual void Shutdown() override;

		virtual void Bind() override;

		virtual int GetMaterialType()const override { return MATERIAL_TYPE_STATIC; }

	private:
		ConstantBuffer mConstantBuffer;
	};
}