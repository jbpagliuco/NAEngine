#pragma once

#include "Base/Streaming/Stream.h"

#include "GameComponent.h"

namespace na
{
	class Skybox;

	class SkyboxComponent : public GameComponentBase<GameComponentType::SKYBOX>
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;

		virtual void Activate() override;
		virtual void Deactivate() override;

		Skybox* GetSkybox();

	private:
		AssetID mSkyboxID;
	};
}