#include "Skybox.h"

#include "Base/File/File.h"
#include "Base/Math/Matrix.h"

#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderingSystem.h"
#include "Renderer/Scene/Camera.h"
#include "Renderer/Shader/Shader.h"

namespace na
{
	static ConstantBuffer SkyboxPerFrameBuffer;
	static Mesh* SkyboxMesh;


	bool Skybox::Initialize(const std::string& skyboxTexture)
	{
		TextureDesc desc;
		desc.mTextureDesc.mBindFlags = NGA_TEXTURE_BIND_SHADER_RESOURCE;
		desc.mTextureDesc.mType = NGATextureType::TEXTURECUBE;

		std::string fullPath;
		na::GetFullFilepath(fullPath, skyboxTexture);
		bool success = mSkyboxTexture.Initialize(desc, fullPath, true);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create skybox texture.");

		NGAFixedFunctionStateDesc fixedDesc;
		fixedDesc.mRasterizerStateDesc.mCullMode = NGACullMode::CULL_FRONT;
		fixedDesc.mDepthStencilStateDesc.mDepthFunc = NGADepthFunc::LESS_EQUAL;

		success = mPipelineState.Construct(fixedDesc, NGAGraphicsPipelineInputAssemblyDesc());
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create skybox pipeline.");

		return true;
	}

	void Skybox::Shutdown()
	{
		mPipelineState.Destruct();
		mSkyboxTexture.Shutdown();
	}

	void Skybox::Render(const Camera &camera)
	{
		NA_RStateManager->BindPipelineState(mPipelineState);

		NA_RStateManager->BindShader(GetEngineShader(EngineShader::SKYBOX)->GetVertexShader());
		NA_RStateManager->BindShader(GetEngineShader(EngineShader::SKYBOX)->GetPixelShader());

		Matrix vp = camera.GetViewProj() * Matrix::Translation(camera.mTransform.mPosition);
		SkyboxPerFrameBuffer.Map(&vp);
		NA_RStateManager->BindConstantBuffer(SkyboxPerFrameBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, 0);

		NA_RStateManager->BindShaderResource(mSkyboxTexture.GetShaderResourceView(), NGA_SHADER_STAGE_PIXEL, 0);
		NA_RStateManager->BindSamplerState(mSkyboxTexture.GetSamplerState(), NGA_SHADER_STAGE_PIXEL, 0);

		SkyboxMesh->Render();
	}


	bool SkyboxSystemInitialize()
	{
		SkyboxPerFrameBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(Matrix));

		// TODO: Don't request an asset provided by the game here
		AssetID meshID = RequestAsset("cube.meshx");
		SkyboxMesh = Mesh::Get(meshID);

		return true;
	}

	void SkyboxSystemShutdown()
	{
		ReleaseAsset(SkyboxMesh->GetID());
		SkyboxPerFrameBuffer.Shutdown();
	}
}