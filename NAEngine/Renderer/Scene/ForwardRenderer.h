#pragma once

#include "SceneRenderer.h"

#include "Renderer/NGA/NGAPipelineState.h"
#include "Renderer/Pipelines/ShadowMap.h"
#include "Renderer/Pipelines/SSAO.h"
#include "Renderer/Shader/EngineShaders/shader_constants.h"

namespace na
{
	struct Light;
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

	private:
		void CollectShadowCastingLights(Scene &scene);
		void BuildShadowMaps(Scene &scene, const Camera &camera);
		void BuildSSAOMap(Scene &scene, const Camera &camera);
		void RenderSceneToBackBuffer(Scene &scene, const Camera &camera);

	private:
		NGAPipelineState mRenderPipelineState;

		Light* mShadowCastingLights[MAX_SHADOWMAPS];
		int mNumShadowCastingLights;

		ShadowMapBuilder mShadowMapBuilder;
		SSAOBuilder mSSAOBuilder;
	};
}