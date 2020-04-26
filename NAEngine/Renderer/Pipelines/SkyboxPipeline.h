#pragma once

namespace na
{
	class Camera;
	class Skybox;

	bool SkyboxSystemInitialize();
	void SkyboxSystemShutdown();

	void RenderSkybox(const Skybox& skybox, const Camera& camera);
}