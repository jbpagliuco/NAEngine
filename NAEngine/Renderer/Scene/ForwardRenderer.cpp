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
	constexpr int SHADOW_MAP_SIZE = 1024;
	constexpr const char* BUILD_SHADOW_MAP_SHADERX = "build_shadow_map.shaderx";

	RenderTarget ForwardRenderer::mShadowMap;

	bool ForwardRenderer::Initialize()
	{
		RenderTargetDesc desc;
		desc.mWidth = SHADOW_MAP_SIZE;
		desc.mHeight = SHADOW_MAP_SIZE;
		desc.mUseColorMap = true;
		desc.mColorMapDesc.mFormat = NGAFormat::R32G32B32A32_FLOAT;
		desc.mColorMapDesc.mShaderResource = false;
		desc.mColorMapDesc.mType = NGATextureType::TEXTURE2D;
		desc.mColorMapDesc.mUsage = NGAUsage::GPU_WRITE;
		desc.mUseDepthMap = true;
		desc.mDepthMapDesc.mFormat = NGAFormat::R32_TYPELESS;
		desc.mDepthMapDesc.mShaderResource = true;
		desc.mDepthMapDesc.mType = NGATextureType::TEXTURE2D;
		desc.mDepthMapDesc.mUsage = NGAUsage::GPU_WRITE;

		bool success = mShadowMap.Initialize(desc);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create shadow map.");

		std::string shadowMapFilePath;
		success = GetFullFilepath(shadowMapFilePath, BUILD_SHADOW_MAP_SHADERX);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to find %s", BUILD_SHADOW_MAP_SHADERX);

		AssetID shaderID = RequestAsset(shadowMapFilePath);
		mBuildShadowMapShader = Shader::Get(shaderID);

		success = mShadowMapLightsBuffers[0].Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(Matrix));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create shadow map light WVP buffer.");

		success = mShadowMapLightsBuffers[1].Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(Matrix));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create shadow map object buffer.");

		RegisterEngineRenderTarget("shadowMap", &mShadowMap);

		return true;
	}

	void ForwardRenderer::Shutdown()
	{
		ReleaseAsset(mBuildShadowMapShader->GetID());
	}

	void ForwardRenderer::BeginRender()
	{
	}

	void ForwardRenderer::EndRender()
	{
	}

	void ForwardRenderer::RenderScene(Scene &scene, const Camera &camera)
	{
		//////////////////////////////////////////////////////////////
		// Build our shadow map
		BuildShadowMap(scene, camera);

		//////////////////////////////////////////////////////////////
		// Render the real scene

		NA_RStateManager->SetViewProjMatrices(
			camera.mTransform.GetMatrix().Inverted(),
			Matrix::PerspectiveFOVLH(camera.mFOV, NA_Renderer->GetWindow().GetAspectRatio(), camera.mNear, camera.mFar)
		);

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

	void ForwardRenderer::BuildShadowMap(Scene& scene, const Camera& camera)
	{
		Light *directionalLight = nullptr;
		for (auto& light : scene.GetLights()) {
			if (light->mType == (int)LightType::DIRECTIONAL && light->mEnabled) {
				directionalLight = light;
			}
		}

		NA_ASSERT_RETURN(directionalLight != nullptr);

		// Set shader
		NA_RStateManager->BindShader(mBuildShadowMapShader->GetVertexShader());
		NA_RStateManager->BindShader(mBuildShadowMapShader->GetPixelShader());

		// Setup render target
		NA_RStateManager->BindRenderTarget(mShadowMap);
		NA_RStateManager->ClearRenderTarget(mShadowMap, ColorF(COLOR_WHITE).vArray, true);

		// Set light WVP constant buffer
		Matrix lightView, lightProj;
		Vector lightDir(directionalLight->mDirection.x, directionalLight->mDirection.y, directionalLight->mDirection.z, 1.0f);
		Vector lightPos(directionalLight->mPosition.x, directionalLight->mPosition.y, directionalLight->mPosition.z, 1.0f);
		lightView = Matrix::LookAtLH(lightPos, lightPos + lightDir.V3Normalize(), Vector(0.0f, 1.0f, 0.0f, 0.0f));
		lightProj = Matrix::OrthoLH(100.0f, 100.0f, 0.1f, 100.0f);
		Matrix lightWVP = lightProj * lightView;
		NA_RStateManager->MapBufferData(mShadowMapLightsBuffers[0].GetBuffer(), &lightWVP);
		NA_RStateManager->BindConstantBufferRealSlot(mShadowMapLightsBuffers[0].GetBuffer(), NGA_SHADER_STAGE_VERTEX, 0);

		for (auto &r : scene.GetRenderables()) {
			Matrix worldTransform = r->GetWorldTransform();
			NA_RStateManager->MapBufferData(mShadowMapLightsBuffers[1].GetBuffer(), &worldTransform);
			NA_RStateManager->BindConstantBufferRealSlot(mShadowMapLightsBuffers[1].GetBuffer(), NGA_SHADER_STAGE_VERTEX, 1);

			r->Render(false);
		}
	}
}