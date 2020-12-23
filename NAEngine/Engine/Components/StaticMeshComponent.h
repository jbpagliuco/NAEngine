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

		bool GetVisible()const;
		void SetVisible(bool visible);

	private:
		AssetID mMeshID;
		std::vector<MaterialAsset*> mMaterialAssets;

		MeshInstance mMeshInstance;
	};
}