#pragma once

#include <string>

namespace na
{
	class Mesh;
	class NGAInputLayout;
	class RenderTarget;
	class Shader;
	class Texture;


	enum class EngineMesh
	{
		POINT,
		QUAD
	};

	enum class EngineShader
	{
		DEBUG_TEXTURE,
		SHADOWMAP,
		SKYBOX,
		BILLBOARD
	};

	enum class EngineInputLayout
	{
		BILLBOARD
	};


	bool RenderingSystemInit();
	bool RenderingSystemInitLate();
	void RenderingSystemShutdown();

	void RenderingSystemBeginFrame();
	void RenderingSystemDoFrame();
	void RenderingSystemEndFrame();


	void RegisterEngineTexture(const std::string& name, const Texture* texture);
	const Texture* GetEngineTexture(const std::string& name);
	const Texture* TryGetEngineTexture(const std::string& name);

	void RegisterEngineRenderTarget(const std::string& name, RenderTarget* renderTarget);
	RenderTarget* GetEngineRenderTarget(const std::string& name);

	Mesh* GetEngineMesh(EngineMesh mesh);
	Shader* GetEngineShader(EngineShader shader);
	NGAInputLayout* GetEngineInputLayout(EngineInputLayout inputLayout);
}