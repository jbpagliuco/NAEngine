#include "MaterialManager.h"

#include "Base/Streaming/Stream.h"

#include "Renderer/Material/Material.h"
#include "Renderer/Resources/RenderTarget.h"
#include "Renderer/Resources/Texture.h"

namespace na
{
	void MaterialManager::Initialize(const std::string& materialFilename)
	{
		AssetID matID = RequestAsset(materialFilename);
		mMaterial = GetMaterialByID(matID);

		mMaterialContainer.Initialize(mMaterial);
	}

	void MaterialManager::Shutdown()
	{
		for (auto it = mAssets.begin(); it != mAssets.end(); ++it) {
			ReleaseAsset(it->second);
		}

		mMaterialContainer.Shutdown();
		ReleaseAsset(mMaterial->GetID());
	}

	MaterialContainer* MaterialManager::GetMaterialContainer()
	{
		return &mMaterialContainer;
	}

	bool MaterialManager::SetFloatParameter(const std::string& name, float value)
	{
		return mMaterialContainer.SetFloatParameter(name, value);
	}

	bool MaterialManager::SetVectorParameter(const std::string& name, const Vector4f& value)
	{
		return mMaterialContainer.SetVectorParameter(name, value);
	}

	bool MaterialManager::SetTextureParameter(const std::string& name, const std::string &filename)
	{
		AssetID texID = RequestAsset(filename);
		Texture* texture = Texture::Get(texID);

		Release(name);
		mAssets[name] = texID;

		return mMaterialContainer.SetTextureParameter(name, texture);
	}

	bool MaterialManager::SetTextureParameter(const std::string& name, Texture* texture)
	{
		AddAssetRef(texture->GetID());

		Release(name);
		mAssets[name] = texture->GetID();

		return mMaterialContainer.SetTextureParameter(name, texture);
	}

	bool MaterialManager::SetRenderTargetParameter(const std::string& name, const std::string& filename, bool useColorMap)
	{
		AssetID rtID = RequestAsset(filename);
		RenderTarget* renderTarget = RenderTarget::Get(rtID);

		Release(name);
		mAssets[name] = rtID;

		if (useColorMap) {
			return mMaterialContainer.SetTextureParameter(name, &renderTarget->GetColorMap());
		}
		else {
			return mMaterialContainer.SetTextureParameter(name, &renderTarget->GetDepthMap());
		}
	}

	bool MaterialManager::SetRenderTargetParameter(const std::string& name, RenderTarget* renderTarget, bool useColorMap)
	{
		AddAssetRef(renderTarget->GetID());

		Release(name);
		mAssets[name] = renderTarget->GetID();

		if (useColorMap) {
			return mMaterialContainer.SetTextureParameter(name, &renderTarget->GetColorMap());
		}
		else {
			return mMaterialContainer.SetTextureParameter(name, &renderTarget->GetDepthMap());
		}
	}

	void MaterialManager::Release(const std::string& name)
	{
		if (mAssets.find(name) != mAssets.end()) {
			ReleaseAsset(mAssets[name]);
			mAssets.erase(name);
		}
	}
}