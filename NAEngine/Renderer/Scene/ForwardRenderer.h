#pragma once

#include "SceneRenderer.h"
#include "Renderer/RenderTarget.h"

namespace na
{
	class ForwardRenderer : public SceneRenderer
	{
	public:
		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void BeginRender() override;
		virtual void EndRender() override;

		virtual void RenderScene(Scene *scene) override;

	private:
		RenderTarget mRT;
	};
}