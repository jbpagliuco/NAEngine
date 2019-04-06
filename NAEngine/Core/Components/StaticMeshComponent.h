#pragma once

#include "Renderer/Scene/Renderables/MeshInstance.h"
#include "Renderer/Material.h"

#include "GameComponent.h"

namespace na
{
	class StaticMeshComponent : public GameComponent
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;

		virtual void Activate() override;
		virtual void Deactivate() override;

	private:
		MeshInstance *mMeshInstance;
		Material mat;
	};
}