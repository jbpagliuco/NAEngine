#include "BillboardInstance.h"

#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderingSystem.h"
#include "Renderer/Resources/Texture.h"
#include "Renderer/Shader/Shader.h"

namespace na
{
	bool BillboardInstance::Initialize(const Texture* texture)
	{
		NA_ASSERT_RETURN_VALUE(texture->IsShaderResource(), false);

		mTexture = texture;

		return true;
	}

	void BillboardInstance::Shutdown()
	{
		mTexture = nullptr;
	}

	void BillboardInstance::Render(bool bindMaterial)
	{
		// Bind input layout
		NA_RStateManager->BindInputLayout(*GetEngineInputLayout(EngineInputLayout::BILLBOARD));

		if (bindMaterial) {
			// Bind billboard shaders
			GetEngineShader(EngineShader::BILLBOARD)->Bind();

			// Bind billboard 
			NA_RStateManager->BindUserShaderResource(*mTexture, NGAShaderStage::NGA_SHADER_STAGE_PIXEL, 0);
			NA_RStateManager->BindUserSamplerState(mTexture->GetSamplerState(), NGAShaderStage::NGA_SHADER_STAGE_PIXEL, 0);
		}

		// Draw point
		GetEngineMesh(EngineMesh::POINT)->Render();
	}
}