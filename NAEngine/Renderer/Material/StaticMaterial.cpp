#include "StaticMaterial.h"

#include "Renderer/Renderer.h"
#include "Renderer/Resources/Texture.h"

namespace na
{
	NA_FACTORY_SETUP(StaticMaterial);

	bool StaticMaterial::Initialize(Shader *shader, void *parameterData, size_t parameterByteLength, const std::vector<const Texture*> &textures)
	{
		if (!Material::Initialize(shader)) {
			return false;
		}

		if (parameterByteLength > 0 && !mConstantBuffer.Initialize(ConstantBufferUsage::IMMUTABLE, parameterData, parameterByteLength)) {
			return false;
		}

		mTextures = textures;

		return true;
	}
	
	void StaticMaterial::Shutdown()
	{
		Material::Shutdown();

		mConstantBuffer.Shutdown();
	}

	void StaticMaterial::Bind()
	{
		Material::Bind();

		// Bind constant data
		const NGABuffer& constantBuffer = mConstantBuffer.GetBuffer();
		if (constantBuffer.IsConstructed()) {
			NA_RStateManager->BindConstantBuffer(constantBuffer, NGA_SHADER_STAGE_PIXEL, 0);
		}

		// Bind textures
		for (int i = 0; i < mTextures.size(); ++i) {
			NA_RStateManager->BindShaderResource(*mTextures[i], NGA_SHADER_STAGE_PIXEL, i);
			NA_RStateManager->BindSamplerState(mTextures[i]->GetSamplerState(), NGA_SHADER_STAGE_PIXEL, i);
		}
	}
}