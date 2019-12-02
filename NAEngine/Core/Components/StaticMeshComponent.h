#pragma once

#include "Renderer/Scene/Renderables/MeshInstance.h"

#include "GameComponent.h"

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
		MeshInstance mMeshInstance;
	};
}