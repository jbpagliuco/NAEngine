#pragma once

#include "SceneRenderer.h"

#include "Renderer/Pipelines/ShadowMap.h"
#include "Renderer/Pipelines/Skybox.h"
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
		NGAPipelineState mRenderPipelineState;

		ShadowMapBuilder mShadowMapBuilder;
		Skybox mSkybox;
	};
}