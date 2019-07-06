#include "StaticMaterial.h"

#include "Renderer/Texture.h"

namespace na
{
	NA_FACTORY_SETUP(StaticMaterial);

	bool StaticMaterial::Initialize(AssetID shaderID, void *parameterData, size_t parameterByteLength, const std::vector<AssetID> &textures)
	{
		if (!Material::Initialize(shaderID)) {
			return false;
		}

		if (!mConstantBuffer.Initialize(BufferUsage::IMMUTABLE, parameterData, parameterByteLength)) {
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
		PlatformConstantBuffer *cb = mConstantBuffer.GetBuffer();
		NA_RContext->PSSetConstantBuffers(NA_RStateData->GetUserPSConstantBufferIndex(), 1, &cb);

		// Bind textures
		for (int i = 0; i < mTextures.size(); ++i) {
			PlatformShaderResourceView *srv = mTextures[i]->GetShaderResourceView();
			NA_RContext->PSSetShaderResources(i, 1, &srv);

			PlatformSamplerState *sampler = mTextures[i]->GetSampler();
			NA_RContext->PSSetSamplers(i, 1, &sampler);
		}
	}
}