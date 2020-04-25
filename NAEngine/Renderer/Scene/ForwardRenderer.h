#pragma once

#include "SceneRenderer.h"

#include "Renderer/Scene/ShadowMap.h"

#define HLSL_CPP_INCLUDE
#include "Renderer/Shader/EngineShaders/shader_common.hlsli"

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
		ShadowMap mShadowMaps[MAX_SHADOWMAPS];
	};
}