#include "ForwardRenderer.h"

#include "Base/Util/Color.h"

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

	void ForwardRenderer::RenderScene(Scene &scene, const Camera &camera)
	{
		// Bind render target
		RenderTarget* rt = (camera.mRenderTarget == nullptr) ? NA_RMainRenderTarget : camera.mRenderTarget;
		NA_RStateManager->BindRenderTarget(*rt);

		const ColorF clearColor = COLOR_CORNFLOWERBLUE;
		NA_RStateManager->ClearRenderTarget(*rt, clearColor.vArray, true);

		// Set up shader data
		auto &lights = scene.GetLights();

		constexpr float ambient = 0.3f;
		Vector3f cameraPos = camera.mTransform.mPosition.AsVector3();

		LightsData lightsData;
		lightsData.globalAmbient = Tuple4f(ambient, ambient, ambient, 1.0f);
		lightsData.eyePosition = Tuple3f(cameraPos.x, cameraPos.y, cameraPos.z);
		lightsData.numLights = (int)lights.size();
		for (int i = 0; i < lightsData.numLights; ++i) {
			lightsData.lights[i] = *lights[i];
		}
		NA_RStateManager->SetLightsData(lightsData);

		for (auto &r : scene.GetRenderables()) {
			NA_RStateManager->SetObjectTransform(r->GetWorldTransform());
			r->Render();
		}
	}
}