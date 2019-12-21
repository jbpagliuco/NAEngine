#include "ForwardRenderer.h"

#include "Camera.h"
#include "Renderables/RenderableInstance.h"
#include "Renderer/Renderer.h"
#include "Scene.h"

namespace na
{
	bool ForwardRenderer::Initialize()
	{
		return true;
	}

	void ForwardRenderer::Shutdown()
	{
	}

	void ForwardRenderer::BeginRender()
	{
	}

	void ForwardRenderer::EndRender()
	{
	}

	void ForwardRenderer::RenderScene(Scene *scene)
	{
		auto &lights = scene->GetLights();

		LightsData lightsData;
		constexpr float ambient = 0.3f;
		lightsData.globalAmbient = Tuple4f(ambient, ambient, ambient, 1.0f);
		Vector3f cameraPos = NA_Renderer->GetActiveCamera()->mTransform.mPosition.AsVector3();
		lightsData.eyePosition = Tuple3f(cameraPos.x, cameraPos.y, cameraPos.z);
		lightsData.numLights = (int)lights.size();
		for (int i = 0; i < lightsData.numLights; ++i) {
			lightsData.lights[i] = *lights[i];
		}
		NA_RStateManager->SetLightsData(lightsData);

		for (auto &r : scene->GetRenderables()) {
			NA_RStateManager->SetObjectTransform(r->GetWorldTransform());
			r->Render();
		}
	}
}