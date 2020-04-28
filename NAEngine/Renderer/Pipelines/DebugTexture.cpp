#include "DebugTexture.h"

#include "Renderer/Mesh.h"
#include "Renderer/RenderingSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/Resources/ConstantBuffer.h"
#include "Renderer/Resources/Texture.h"
#include "Renderer/Shader/Shader.h"

namespace na
{
	static ConstantBuffer DebugTextureCB;

	bool DebugTextureSystemInitialize()
	{
		bool success = DebugTextureCB.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(Matrix));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize debug texture constant buffer.");

		return true;
	}

	void DebugTextureSystemShutdown()
	{
		DebugTextureCB.Shutdown();
	}

	void RenderDebugTexture()
	{
		const Texture* debugTex = TryGetEngineTexture("debug");
		if (debugTex != nullptr) {
			// Bind shader
			NA_RStateManager->BindShader(GetEngineShader(EngineShader::DEBUG_TEXTURE)->GetVertexShader());
			NA_RStateManager->BindShader(GetEngineShader(EngineShader::DEBUG_TEXTURE)->GetPixelShader());

			// Scale and shift quad to lower-right corner.
			Matrix world = Matrix::Translation(Vector(0.5f, -0.5f, 0.0f, 1.0f)) * Matrix::Scaling(Vector(0.5f, 0.5f, 1.0f, 1.0f));
			DebugTextureCB.Map(&world);
			NA_RStateManager->BindConstantBuffer(DebugTextureCB.GetBuffer(), NGA_SHADER_STAGE_VERTEX, 0);

			// Bind the texture
			NA_RStateManager->BindShaderResource(debugTex->GetShaderResourceView(), NGA_SHADER_STAGE_PIXEL, 0);
			NA_RStateManager->BindSamplerState(debugTex->GetSamplerState(), NGA_SHADER_STAGE_PIXEL, 0);

			// Render the quad
			GetEngineMesh(EngineMesh::QUAD)->Render();
		}
	}
}