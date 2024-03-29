#include "ForwardRenderer.h"

#include "Base/File/File.h"
#include "Base/Util/Color.h"

#include "Renderer/Pipelines/DebugTexture.h"
#include "Renderer/Pipelines/SkyboxPipeline.h"
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
		bool success = mShadowMapBuilder.Initialize(MAX_SHADOWMAPS);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize shadowmap builder.");

		success = mRenderPipelineState.Construct(NGAFixedFunctionStateDesc(), NGAGraphicsPipelineInputAssemblyDesc());
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to render pipeline state.");

		// Register engine assets
		RegisterEngineRenderTarget("shadowMap", &mShadowMapBuilder.GetRenderTarget());

		return true;
	}

	void ForwardRenderer::Shutdown()
	{
		mShadowMapBuilder.Shutdown();

		mRenderPipelineState.Destruct();
	}

	void ForwardRenderer::BeginRender()
	{
	}

	void ForwardRenderer::EndRender()
	{
	}

	void ForwardRenderer::RenderScene(Scene &scene, const Camera &camera)
	{
		CollectShadowCastingLights(scene);

		NGARect r;
		r.x = 0.0f;
		r.y = 0.0f;
		r.width = (float)NA_Renderer->GetWindow().width;
		r.height = (float)NA_Renderer->GetWindow().height;
		NA_RStateManager->SetViewport(r);

		NA_RStateManager->BindPipelineState(mRenderPipelineState);

		BuildShadowMaps(scene, camera);

		RenderSceneToBackBuffer(scene, camera);

		// Unbind shadow maps
		NA_RStateManager->BindShaderResource(NGAShaderResourceView::INVALID, NGA_SHADER_STAGE_PIXEL, (int)TextureRegisters::SHADOWMAP);
		NA_RStateManager->BindSamplerState(NGASamplerState::INVALID, NGA_SHADER_STAGE_PIXEL, (int)SamplerStateRegisters::SHADOWMAP);
	}


	void ForwardRenderer::CollectShadowCastingLights(Scene& scene)
	{
		// Clear out our shadow casting light list
		for (int i = 0; i < MAX_SHADOWMAPS; ++i) {
			mShadowCastingLights[i] = nullptr;
		}

		int shadowCasterIndex = 0;
		for (auto& light : scene.GetLights()) {
			// Invalidate shadow casting index
			light->mShadowCastingIndex = -1;

			// Add this light to the list of shadow casters, if possible.
			if (shadowCasterIndex < MAX_SHADOWMAPS) {
				// Is this a shadow casting light?
				if (light->mType == (int)LightType::DIRECTIONAL && light->mEnabled) {
					mShadowCastingLights[shadowCasterIndex] = light;
					mShadowCastingLights[shadowCasterIndex]->mShadowCastingIndex = shadowCasterIndex;

					++shadowCasterIndex;
				}
			}
		}

		mNumShadowCastingLights = shadowCasterIndex;
	}

	void ForwardRenderer::BuildShadowMaps(Scene &scene, const Camera &camera)
	{
		mShadowMapBuilder.BuildAll(scene, (const Light**)mShadowCastingLights, mNumShadowCastingLights);
	}
	
	void ForwardRenderer::RenderSceneToBackBuffer(Scene &scene, const Camera &camera)
	{
		NGARect r;
		r.x = 0.0f;
		r.y = 0.0f;
		r.width = (float)NA_Renderer->GetWindow().width;
		r.height = (float)NA_Renderer->GetWindow().height;
		NA_RStateManager->SetViewport(r);

		NA_RStateManager->BindPipelineState(mRenderPipelineState);

		// Set per frame data
		Matrix shadowCasterMatrices[MAX_SHADOWMAPS];
		for (int i = 0; i < MAX_SHADOWMAPS; ++i) {
			if (mShadowCastingLights[i] != nullptr) {
				shadowCasterMatrices[i] = mShadowCastingLights[i]->GetViewProj();
			}
		}

		NA_RStateManager->SetPerFrameData(camera.GetViewProj(), shadowCasterMatrices, mNumShadowCastingLights);

		// Bind render target
		RenderTarget* rt = (camera.mRenderTarget == nullptr) ? NA_RMainRenderTarget : camera.mRenderTarget;
		rt->Bind();
		rt->Clear(ColorF(COLOR_CORNFLOWERBLUE).vArray, true);

		// Bind shadow map textures
		const Texture &depthMap = mShadowMapBuilder.GetRenderTarget().GetDepthMap();
		NA_RStateManager->BindShaderResource(depthMap.GetShaderResourceView(), NGA_SHADER_STAGE_PIXEL, (int)TextureRegisters::SHADOWMAP);
		NA_RStateManager->BindSamplerState(depthMap.GetSamplerState(), NGA_SHADER_STAGE_PIXEL, (int)SamplerStateRegisters::SHADOWMAP);

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

		// Render all the renderables in the scene
		for (auto &r : scene.GetRenderables()) {
			NA_RStateManager->SetObjectTransform(r->GetWorldTransform());
			r->Render();
		}

		// Draw skybox last
		if (camera.mSkybox != nullptr) {
			RenderSkybox(*camera.mSkybox, camera);
		}

		// Draw debug texture, if set
		NA_RStateManager->BindPipelineState(mRenderPipelineState);
		RenderDebugTexture();
	}
}