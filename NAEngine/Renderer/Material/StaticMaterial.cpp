#include "StaticMaterial.h"

#include "Renderer/Renderer.h"
#include "Renderer/Resources/Texture.h"

namespace na
{
	NA_FACTORY_SETUP(StaticMaterial);

	bool StaticMaterial::Initialize(AssetID shaderID, void *parameterData, size_t parameterByteLength, const std::vector<AssetID> &textures)
	{
		if (!Material::Initialize(shaderID)) {
			return false;
		}

		if (!mConstantBuffer.Initialize(ConstantBufferUsage::IMMUTABLE, parameterData, parameterByteLength)) {
			return false;
		}

		for (auto &texID : textures) {
			mTextures.push_back(Texture::Get(texID));
		}

		return true;
	}
	
	void StaticMaterial::Shutdown()
	{
		Material::Shutdown();

		mConstantBuffer.Shutdown();

		for (auto &texture : mTextures) {
			ReleaseAsset(texture->GetID());
		}
	}

	void StaticMaterial::Bind()
	{
		Material::Bind();

		// Bind constant data
		NA_RStateManager->BindConstantBuffer(mConstantBuffer.GetBuffer(), NGA_SHADER_STAGE_PIXEL, 0);

		// Bind textures
		for (int i = 0; i < mTextures.size(); ++i) {
			NA_RStateManager->BindShaderResource(mTextures[i]->GetShaderResourceView(), NGA_SHADER_STAGE_PIXEL, i);
			NA_RStateManager->BindSamplerState(mTextures[i]->GetSamplerState(), NGA_SHADER_STAGE_PIXEL, i);
		}
	}
}