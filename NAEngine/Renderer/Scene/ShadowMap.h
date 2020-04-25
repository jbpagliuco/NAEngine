#pragma once

#include "Renderer/Resources/RenderTarget.h"

namespace na
{
	struct Light;
	class Scene;

	class ShadowMap
	{
	public:
		bool Initialize();
		void Shutdown();

		void Build(Scene& scene, Light& light);

		RenderTarget& GetRenderTarget();

	private:
		RenderTarget mRenderTarget;
	};


	bool ShadowMapSystemInitialize();
	void ShadowMapSystemShutdown();
}