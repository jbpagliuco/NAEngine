#pragma once

#include "SceneRenderer.h"

#include "Renderer/Scene/ShadowMap.h"
#include "Renderer/Shader/EngineShaders/shader_constants.h"

namespace na
{
	class Matrix;
	class Shader;

	class ForwardRenderer : public SceneRenderer
	{
	public:
		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void BeginRender() override;
		virtual void EndRender() override;

		virtual void RenderScene(Scene &scene, const Camera &camera) override;

	public:
		ShadowMapBuilder mShadowMapBuilder;
	};
}