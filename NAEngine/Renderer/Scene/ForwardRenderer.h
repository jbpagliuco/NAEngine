#pragma once

#include "SceneRenderer.h"

#include "Renderer/Scene/ShadowMap.h"

namespace na
{
	class Matrix;
	class Shader;

	constexpr int MAX_SHADOWMAPS = 1;

	class ForwardRenderer : public SceneRenderer
	{
	public:
		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void BeginRender() override;
		virtual void EndRender() override;

		virtual void RenderScene(Scene &scene, const Camera &camera) override;

	public:
		ShadowMap mShadowMaps[MAX_SHADOWMAPS];
	};
}