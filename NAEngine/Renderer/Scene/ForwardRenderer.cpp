#include "ForwardRenderer.h"

#include "Base/File/File.h"
#include "Base/Util/Color.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderingSystem.h"
#include "Renderer/Scene/Camera.h"
#include "Renderer/Scene/Renderables/RenderableInstance.h"
#include "Renderer/Scene/Scene.h"
#include "Renderer/Shader/Shader.h"

namespace na
{
	bool ForwardRenderer::Initialize()
	{
		for (int i = 0; i < MAX_SHADOWMAPS; ++i) {
			mShadowMaps[i].Initialize();
			RegisterEngineRenderTarget("shadowMap" + std::to_string(i), &mShadowMaps[i].GetRenderTarget());
		}

		return true;
	}

	void ForwardRenderer::Shutdown()
	{
		for (int i = 0; i < MAX_SHADOWMAPS; ++i) {
			mShadowMaps[i].Shutdown();
		}
	}

	void ForwardRenderer::BeginRender()
	{
	}

	void ForwardRenderer::EndRender()
	{
	}

	void ForwardRenderer::RenderScene(Scene &scene, const Camera &camera)
	{
		// TEMP: Grab the first directional light for shadows
		Light *directionalLight = nullptr;
		for (auto& light : scene.GetLights()) {
			if (light->mType == (int)LightType::DIRECTIONAL && light->mEnabled) {
				directionalLight = light;
			}
		}

		//////////////////////////////////////////////////////////////
		// Build our shadow maps
		for (int i = 0; i < MAX_SHADOWMAPS; ++i) {
			mShadowMaps[i].Build(scene, *directionalLight);
		}

		//////////////////////////////////////////////////////////////
		// Render the real scene
		NGARect r;
		r.x = 0.0f;
		r.y = 0.0f;
		r.width = (float)NA_Renderer->GetWindow().width;
		r.height = (float)NA_Renderer->GetWindow().height;
		NA_RStateManager->SetViewport(r);

		Matrix cameraView = camera.mTransform.GetMatrix().Inverted();
		Matrix cameraProj = Matrix::PerspectiveFOVLH(camera.mFOV, NA_Renderer->GetWindow().GetAspectRatio(), camera.mNear, camera.mFar);
		NA_RStateManager->SetPerFrameData(cameraProj * cameraView, &directionalLight->GetViewProj());

		// Bind render target
		RenderTarget* rt = (camera.mRenderTarget == nullptr) ? NA_RMainRenderTarget : camera.mRenderTarget;
		NA_RStateManager->BindRenderTarget(*rt);

		const ColorF clearColor = COLOR_CORNFLOWERBLUE;
		NA_RStateManager->ClearRenderTarget(*rt, clearColor.vArray, true);

		// Bind shadow map textures
		for (int i = 0; i < MAX_SHADOWMAPS; ++i) {
			const Texture& depthMap = mShadowMaps[i].GetRenderTarget().GetDepthMap();
			NA_RStateManager->BindShaderResource(depthMap.GetShaderResourceView(), NGA_SHADER_STAGE_PIXEL, (int)TextureRegisters::SHADOWMAP0 + i);
			NA_RStateManager->BindSamplerState(depthMap.GetSamplerState(), NGA_SHADER_STAGE_PIXEL, (int)TextureRegisters::SHADOWMAP0 + i);
		}

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

		// Unbind shadow maps
		for (int i = 0; i < MAX_SHADOWMAPS; ++i) {
			NA_RStateManager->BindShaderResource(NGAShaderResourceView::INVALID, NGA_SHADER_STAGE_PIXEL, (int)TextureRegisters::SHADOWMAP0 + i);
			NA_RStateManager->BindSamplerState(NGASamplerState::INVALID, NGA_SHADER_STAGE_PIXEL, (int)TextureRegisters::SHADOWMAP0 + i);
		}
	}
}