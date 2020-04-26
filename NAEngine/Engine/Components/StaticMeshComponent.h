#pragma once

#include "Renderer/Scene/Renderables/MeshInstance.h"

#include "GameComponent.h"

namespace na
{
	class MaterialAsset;

	class StaticMeshComponent : public GameComponentBase<GameComponentType::STATIC_MESH>
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;

		virtual void Activate() override;
		virtual void Deactivate() override;

		virtual void UpdateLate(float deltaTime) override;

	private:
		AssetID mMeshID;
		MaterialAsset *mMaterialAsset;

		MeshInstance mMeshInstance;
	};
}