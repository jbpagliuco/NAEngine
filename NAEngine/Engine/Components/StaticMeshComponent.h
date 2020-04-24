#pragma once

#include "Renderer/Scene/Renderables/MeshInstance.h"

#include "GameComponent.h"
#include "Engine/Material/MaterialManager.h"

namespace na
{
	class StaticMeshComponent : public GameComponent
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;

		virtual void Activate() override;
		virtual void Deactivate() override;

		virtual void UpdateLate(float deltaTime) override;

	private:
		AssetID mMeshID;

		MeshInstance mMeshInstance;
		MaterialManager mMaterialManager;
	};
}