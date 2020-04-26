#pragma once

#include <string>

namespace na
{
	class RenderTarget;
	class Shader;
	class Texture;


	enum class EngineShader
	{
		SHADOWMAP,
		SKYBOX
	};


	bool RenderingSystemInit();
	bool RenderingSystemInitLate();
	void RenderingSystemShutdown();

	void RenderingSystemBeginFrame();
	void RenderingSystemDoFrame();
	void RenderingSystemEndFrame();


	void RegisterEngineTexture(const std::string& name, Texture* texture);
	Texture* GetEngineTexture(const std::string& name);

	void RegisterEngineRenderTarget(const std::string& name, RenderTarget* renderTarget);
	RenderTarget* GetEngineRenderTarget(const std::string& name);

	Shader* GetEngineShader(EngineShader shader);
}