#pragma once

#include "GameComponent.h"

namespace na
{
	struct Light;

	class LightComponent : public GameComponent
	{
	public:
		virtual void Deactivate() override;

		virtual void UpdateLate(float deltaTime) override;

	protected:
		void SetLight(Light *pLight);
		
		Light *mLight;
	};

	class DirectionalLightComponent : public LightComponent
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;
	};

	class PointLightComponent : public LightComponent
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;
	};

	class SpotLightComponent : public LightComponent
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;
	};
}