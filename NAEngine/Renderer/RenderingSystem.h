#pragma once

#include <string>

namespace na
{
	class Texture;
	class RenderTarget;


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
}