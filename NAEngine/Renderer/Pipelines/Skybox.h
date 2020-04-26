#pragma once

#include <string>

#include "Renderer/NGA/NGAPipelineState.h"

#include "Renderer/Resources/Texture.h"

namespace na
{
	class Camera;

	class Skybox
	{
	public:
		bool Initialize(const std::string& skyboxTexture);
		void Shutdown();

		void Render(const Camera &camera);

	private:
		Texture mSkyboxTexture;

		NGAPipelineState mPipelineState;
	};

	bool SkyboxSystemInitialize();
	void SkyboxSystemShutdown();
}