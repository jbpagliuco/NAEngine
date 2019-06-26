#include "ForwardRenderer.h"

#include "Base/Util/Color.h"
#include "Renderables/RenderableInstance.h"
#include "Renderer/RendererD3D.h"
#include "Scene.h"

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

		const ColorF clearColor = COLOR_CORNFLOWERBLUE;
		mRT.Clear(clearColor.vArray);
	}

	void ForwardRenderer::EndRender()
	{

	}

	void ForwardRenderer::RenderScene(Scene *scene)
	{
		for (auto &r : scene->GetRenderables()) {
			NA_RStateData->SetObjectTransform(r->GetWorldTransform());
			r->Render();
		}
	}
}