#pragma once

#include "SceneRenderer.h"

#include "Renderer/Resources/ConstantBuffer.h"
#include "Renderer/Resources/RenderTarget.h"

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

	private:
		void BuildShadowMap(Scene &scene, const Camera &camera, const Matrix &lightVP);

	public:
		static RenderTarget mShadowMap;
		Shader* mBuildShadowMapShader;
		ConstantBuffer mShadowMapLightsBuffers[2];
	};
}