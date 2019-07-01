#include "Material.h"

#include "StaticMaterial.h"

#include "Renderer/Texture.h"

namespace na
{
	bool Material::Initialize(AssetID shaderID, const std::vector<AssetID> &textures)
	{
		mShader = Shader::Get(shaderID);

		for (auto &texID : textures) {
			mTextures.push_back(Texture::Get(texID));
		}

		return true;
	}
	
	void Material::Shutdown()
	{
		NA_SAFE_RELEASE_ASSET_OBJECT(mShader);

		mConstantBuffer.Shutdown();

		for (auto &texture : mTextures) {
			ReleaseAsset(texture->GetID());
		}
	}

	void Material::Bind()
	{
		mShader->Bind();

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


	Material* GetMaterialByID(AssetID matID)
	{
		if (StaticMaterial::Exists(matID)) {
			return StaticMaterial::Get(matID);
		}

		return nullptr;
	}

	void ReleaseMaterial(Material *pMaterial)
	{
		const int matType = pMaterial->GetMaterialType();
		if (matType == MATERIAL_TYPE_STATIC) {
			ReleaseAsset(static_cast<StaticMaterial*>(pMaterial)->GetID());
		} else {
			NA_ASSERT(false, "Unknown material type %d", matType);
		}
	}
}