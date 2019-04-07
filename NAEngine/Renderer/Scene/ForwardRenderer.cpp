#include "ForwardRenderer.h"

#include "Scene.h"
#include "Renderables/RenderableInstance.h"
#include "Renderer/RendererD3D.h"

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
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(2.0f, 2.0f, -2.0f, 1.0f), DirectX::XMVectorZero(), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, NA_Renderer->GetWindow().GetAspectRatio(), 0.1f, 100.0f);

		NA_RStateData->SetViewProjMatrices(view, proj);
		for (auto &r : scene->GetRenderables()) {
			NA_RStateData->SetObjectTransform(r->GetWorldTransform());
			r->Render();
		}
	}
}