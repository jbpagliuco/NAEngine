#include "ShadowMap.h"

#include "Base/Math/Matrix.h"
#include "Base/Util/Color.h"

#include "Renderer/NGA/NGAShapes.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderingSystem.h"
#include "Renderer/Resources/ConstantBuffer.h"
#include "Renderer/Scene/Scene.h"
#include "Renderer/Scene/Renderables/RenderableInstance.h"
#include "Renderer/Shader/EngineShaders/shader_constants.h"
#include "Renderer/Shader/Shader.h"

namespace na
{
	static NGARect ShadowMapViewport;

	static ConstantBuffer ShadowMapPerFrameBuffer;
	static ConstantBuffer ShadowMapPerObjectBuffer;


	bool ShadowMap::Initialize()
	{
		RenderTargetDesc desc;
		desc.mWidth = SHADOWMAP_SIZE;
		desc.mHeight = SHADOWMAP_SIZE;

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
		desc.mDepthMapDesc.mSamplerStateDesc.mAddressU = NGATextureAddressMode::CLAMP;
		desc.mDepthMapDesc.mSamplerStateDesc.mAddressV = NGATextureAddressMode::CLAMP;
		desc.mDepthMapDesc.mSamplerStateDesc.mAddressW = NGATextureAddressMode::CLAMP;
		desc.mDepthMapDesc.mSamplerStateDesc.mFilter = NGATextureFilter::MIN_MAG_MIP_POINT;

		bool success = mRenderTarget.Initialize(desc);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize shadow map render target.");

		return true;
	}
	
	void ShadowMap::Shutdown()
	{
		mRenderTarget.Shutdown();
	}

	void ShadowMap::Build(Scene& scene, Light& light)
	{
		// Set viewport
		NA_RStateManager->SetViewport(ShadowMapViewport);

		// Setup render target
		NA_RStateManager->BindRenderTarget(mRenderTarget);
		NA_RStateManager->ClearRenderTarget(mRenderTarget, ColorF(COLOR_WHITE).vArray, true);

		// Set shaders
		NA_RStateManager->BindShader(GetEngineShader(EngineShader::SHADOWMAP)->GetVertexShader());
		NA_RStateManager->BindShader(GetEngineShader(EngineShader::SHADOWMAP)->GetPixelShader());

		// Set per frame data
		Matrix lightVP = light.GetViewProj();
		NA_RStateManager->MapBufferData(ShadowMapPerFrameBuffer.GetBuffer(), &lightVP);
		NA_RStateManager->BindConstantBuffer(ShadowMapPerFrameBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, 0);

		// Render objects from the view point of the light
		for (auto &r : scene.GetRenderables()) {
			Matrix worldTransform = r->GetWorldTransform();
			NA_RStateManager->MapBufferData(ShadowMapPerObjectBuffer.GetBuffer(), &worldTransform);
			NA_RStateManager->BindConstantBuffer(ShadowMapPerObjectBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, 1);

			// Render without binding material
			r->Render(false);
		}
	}

	RenderTarget& ShadowMap::GetRenderTarget()
	{
		return mRenderTarget;
	}



	bool ShadowMapSystemInitialize()
	{
		ShadowMapViewport.x = 0.0f;
		ShadowMapViewport.y = 0.0f;
		ShadowMapViewport.width = (float)SHADOWMAP_SIZE;
		ShadowMapViewport.height = (float)SHADOWMAP_SIZE;

		bool success = ShadowMapPerFrameBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(Matrix));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create shadow map per frame buffer.");

		success = ShadowMapPerObjectBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(Matrix));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create shadow map per object buffer.");

		return true;
	}
	
	void ShadowMapSystemShutdown()
	{
		ShadowMapPerObjectBuffer.Shutdown();
		ShadowMapPerFrameBuffer.Shutdown();
	}
}