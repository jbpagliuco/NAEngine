#include "ForwardRenderer.h"

#include "Base/Util/Color.h"

#include "Camera.h"
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
		auto &lights = scene->GetLights();

		LightsData lightsData;
		constexpr float ambient = 0.3f;
		lightsData.globalAmbient = DirectX::XMFLOAT4(ambient, ambient, ambient, 1.0f);
		Vector3f cameraPos = NA_Renderer->GetActiveCamera()->mTransform.mPosition.AsVector3();
		lightsData.eyePosition = DirectX::XMFLOAT3(cameraPos.x, cameraPos.y, cameraPos.z);
		lightsData.numLights = (int)lights.size();
		for (int i = 0; i < lightsData.numLights; ++i) {
			lightsData.lights[i] = *lights[i];
		}
		NA_RStateData->SetLightsData(lightsData);

		for (auto &r : scene->GetRenderables()) {
			NA_RStateData->SetObjectTransform(r->GetWorldTransform());
			r->Render();
		}
	}
}