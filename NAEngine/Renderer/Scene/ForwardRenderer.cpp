#include "ForwardRenderer.h"

#include "Scene.h"
#include "Renderables/RenderableInstance.h"

namespace na
{
	bool ForwardRenderer::Initialize()
	{
		mRT.Initialize();

		return true;
	}

	void ForwardRenderer::Shutdown()
	{
		mRT.Shutdown();
	}

	void ForwardRenderer::BeginRender()
	{
		mRT.Bind();

		const float clearColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		mRT.Clear(clearColor);
	}

	void ForwardRenderer::EndRender()
	{

	}

	void ForwardRenderer::RenderScene(Scene *scene)
	{
		for (auto &r : scene->GetRenderables()) {
			r->Render();
		}
	}
}