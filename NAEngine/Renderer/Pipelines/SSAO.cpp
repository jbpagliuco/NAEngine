#include "SSAO.h"

#include "Base/Math/Matrix.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderingSystem.h"
#include "Renderer/Scene/Camera.h"
#include "Renderer/Scene/Renderables/RenderableInstance.h"
#include "Renderer/Scene/Scene.h"
#include "Renderer/Shader/Shader.h"

namespace na
{
	struct SSAOPerFrame
	{
		Matrix view;
		Matrix viewProj;
	};

	struct SSAOPerObject
	{
		Matrix world;
		Matrix worldInverseTranspose;
	};


	bool SSAOBuilder::Initialize()
	{
		bool success = mPipelineState.Construct(NGAFixedFunctionStateDesc(), NGAGraphicsPipelineInputAssemblyDesc());
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize SSAO pipeline state.");

		RenderTargetDesc rtDesc;
		rtDesc.mWidth = NA_Renderer->GetWindow().width;
		rtDesc.mHeight = NA_Renderer->GetWindow().height;
		rtDesc.mUseColorMap = true;
		rtDesc.mColorMapDesc.mFormat = NGAFormat::R32G32B32A32_FLOAT;
		rtDesc.mColorMapDesc.mShaderResource = true;
		rtDesc.mColorMapDesc.mType = NGATextureType::TEXTURE2D;
		rtDesc.mColorMapDesc.mUsage = NGAUsage::GPU_WRITE;
		rtDesc.mUseDepthMap = true;
		rtDesc.mDepthMapDesc.mFormat = NGAFormat::D16_UNORM;
		rtDesc.mDepthMapDesc.mType = NGATextureType::TEXTURE2D;
		rtDesc.mDepthMapDesc.mUsage = NGAUsage::GPU_WRITE;

		success = mNormalDepthRenderTarget.Initialize(rtDesc);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize SSAO render target.");

		mPerFrameBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(SSAOPerFrame));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize SSAO per frame buffer.");

		mPerObjectBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(SSAOPerObject));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize SSAO per object buffer.");

		return true;
	}

	void SSAOBuilder::Shutdown()
	{
		mPerObjectBuffer.Shutdown();
		mPerFrameBuffer.Shutdown();
		mNormalDepthRenderTarget.Shutdown();
		mPipelineState.Destruct();
	}

	void SSAOBuilder::Build(const Scene &scene, const Camera& camera)
	{
		NA_RStateManager->BindPipelineState(mPipelineState);

		mNormalDepthRenderTarget.Bind();

		// Clear view space normal to (0,0,-1) and clear depth to be very far away.  
		const float clearColor[] = {0.0f, 0.0f, -1.0f, 1e5f};
		mNormalDepthRenderTarget.Clear(clearColor, true);

		// Bind per frame data
		SSAOPerFrame perFrameData;
		perFrameData.view = camera.GetView();
		perFrameData.viewProj = camera.GetProj() * perFrameData.view;
		mPerFrameBuffer.Map(&perFrameData);
		NA_RStateManager->BindConstantBuffer(mPerFrameBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, 0);

		// Bind shaders
		NA_RStateManager->BindShader(GetEngineShader(EngineShader::BUILD_SSAO_NORMAL_DEPTH)->GetVertexShader());
		NA_RStateManager->BindShader(GetEngineShader(EngineShader::BUILD_SSAO_NORMAL_DEPTH)->GetPixelShader());

		// Render each object
		for (auto& renderable : scene.GetRenderables()) {
			SSAOPerObject perObjectData;
			perObjectData.world = renderable->GetWorldTransform();
			perObjectData.worldInverseTranspose = perObjectData.world.Inverted().Transposed();
			mPerObjectBuffer.Map(&perObjectData);
			NA_RStateManager->BindConstantBuffer(mPerObjectBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, 1);

			renderable->Render(false);
		}

		RegisterEngineTexture("debug", &mNormalDepthRenderTarget.GetColorMap());
	}
}