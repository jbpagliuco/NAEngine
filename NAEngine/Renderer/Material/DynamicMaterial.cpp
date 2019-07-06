#include "DynamicMaterial.h"

#include "Renderer/Texture.h"

namespace na
{
	NA_FACTORY_SETUP(DynamicMaterial);

	bool DynamicMaterial::Initialize(AssetID shaderID, size_t parameterByteLength, const DeserializationParameterMap &map, void *defaultParameterData, const std::vector<AssetID> &defaultTextures)
	{
		if (!Material::Initialize(shaderID)) {
			return false;
		}

		if (!mConstantBuffer.Initialize(BufferUsage::DYNAMIC, nullptr, parameterByteLength)) {
			return false;
		}

		// Build indexing maps
		size_t curParameterByteOffset = 0;
		int curTextureIndex = 0;
		for (auto it : map.childrenArray) {
			const std::string name = it.meta["name"];
			const std::string type = it.meta["type"];

			if (type == "texture") {
				mTextureParameterMap[name] = curTextureIndex;
				++curTextureIndex;
			} else {
				DynamicMaterialParameterInfo info;
				info.mLength = GetFormatByteSize(GetFormatFromString(type));
				info.mOffset = curParameterByteOffset;

				mParameterMap[name] = info;

				curParameterByteOffset += info.mLength;
			}
		}

		// Set default data
		mDefaultParameterData = NA_ALLOC(parameterByteLength);
		memcpy(mDefaultParameterData, defaultParameterData, parameterByteLength);

		for (auto &texID : defaultTextures) {
			mDefaultTextures.push_back(Texture::Get(texID));
		}

		return true;
	}

	void DynamicMaterial::Shutdown()
	{
		Material::Shutdown();

		mConstantBuffer.Shutdown();

		NA_FREE(mDefaultParameterData);
		for (auto &texture : mDefaultTextures) {
			ReleaseAsset(texture->GetID());
		}
	}

	void DynamicMaterial::Bind() 
	{
		Material::Bind();

		// Bind default data. This is not great.
		mConstantBuffer.Map(mDefaultParameterData);

		// Bind textures
		for (int i = 0; i < mDefaultTextures.size(); ++i) {
			PlatformShaderResourceView *srv = mDefaultTextures[i]->GetShaderResourceView();
			NA_RContext->PSSetShaderResources(i, 1, &srv);

			PlatformSamplerState *sampler = mDefaultTextures[i]->GetSampler();
			NA_RContext->PSSetSamplers(i, 1, &sampler);
		}

		// Bind constant data
		PlatformConstantBuffer *cb = mConstantBuffer.GetBuffer();
		NA_RContext->PSSetConstantBuffers(NA_RStateData->GetUserPSConstantBufferIndex(), 1, &cb);
	}

	bool DynamicMaterial::GetParameterInfo(DynamicMaterialParameterInfo &info, const std::string &name)
	{
		NA_ASSERT_RETURN_VALUE(mParameterMap.find(name) != mParameterMap.end(), false, "Failed to find parameter info for parameter '%s'", name);

		info = mParameterMap[name];
		return true;
	}

	int DynamicMaterial::GetTextureParameterIndex(const std::string &name)
	{
		NA_ASSERT_RETURN_VALUE(mTextureParameterMap.find(name) != mTextureParameterMap.end(), -1, "Failed to find texture index for parameter '%s'", name);

		return mTextureParameterMap[name];
	}

	void* DynamicMaterial::GetDefaultParameterData()const
	{
		return mDefaultParameterData;
	}

	std::vector<AssetID> DynamicMaterial::GetDefaultTextures()const
	{
		std::vector<AssetID> textures;
		for (auto &it : mDefaultTextures) {
			textures.push_back(it->GetID());
		}

		return textures;
	}





	DynamicMaterialInstance::DynamicMaterialInstance(DynamicMaterial *pMaterial)
	{
		mParent = pMaterial;

		mParameterData = NA_ALLOC(mParent->mConstantBuffer.GetSize());
		memcpy(mParameterData, mParent->GetDefaultParameterData(), mParent->mConstantBuffer.GetSize());
		
		for (auto &assetId : mParent->GetDefaultTextures()) {
			AddAssetRef(assetId);
			mTextures.push_back(Texture::Get(assetId));
		}
	}

	void DynamicMaterialInstance::Shutdown()
	{
		NA_FREE(mParameterData);

		for (auto &pTex : mTextures) {
			ReleaseAsset(pTex->GetID());
		}
	}

	void DynamicMaterialInstance::BindData()
	{
		// Map our data to the material's constant buffer.
		mParent->mConstantBuffer.Map(mParameterData);

		// Bind textures
		for (int i = 0; i < mTextures.size(); ++i) {
			PlatformShaderResourceView *srv = mTextures[i]->GetShaderResourceView();
			NA_RContext->PSSetShaderResources(i, 1, &srv);

			PlatformSamplerState *sampler = mTextures[i]->GetSampler();
			NA_RContext->PSSetSamplers(i, 1, &sampler);
		}
	}


	void DynamicMaterialInstance::SetFloatParameter(const std::string &name, float value)
	{
		DynamicMaterialParameterInfo paramInfo;
		if (mParent->GetParameterInfo(paramInfo, name)) {
			NA_ASSERT(paramInfo.mLength == sizeof(float), "Trying to set float data for a parameter of length %zu", paramInfo.mLength);
			memcpy(mParameterData, &value, paramInfo.mLength);
		}
	}

	void DynamicMaterialInstance::SetVectorParameter(const std::string &name, const DirectX::XMFLOAT4 &value)
	{
		DynamicMaterialParameterInfo paramInfo;
		if (mParent->GetParameterInfo(paramInfo, name)) {
			NA_ASSERT(paramInfo.mLength == sizeof(DirectX::XMFLOAT4), "Trying to set vector data for a parameter of length %zu", paramInfo.mLength);
			memcpy(mParameterData, &value, paramInfo.mLength);
		}
	}

	void DynamicMaterialInstance::SetTextureParameter(const std::string &name, const std::string &filename)
	{
		AssetID textureID = RequestAsset(filename);
		SetTextureParameter(name, Texture::Get(textureID));
	}

	void DynamicMaterialInstance::SetTextureParameter(const std::string &name, Texture *pTexture)
	{
		int index = mParent->GetTextureParameterIndex(name);
		NA_ASSERT_RETURN(index >= 0 && index < mTextures.size(), "Failed to find valid index for a texture parameter with name '%s'", name);

		ReleaseAsset(mTextures[index]->GetID());

		mTextures[index] = pTexture;
	}



	DynamicMaterialInstance* CreateDynamicMaterialInstance(DynamicMaterial *pParent)
	{
		void *pMem = NA_ALLOC(sizeof(DynamicMaterialInstance));
		DynamicMaterialInstance *pInst = new (pMem) DynamicMaterialInstance(pParent);
		NA_ASSERT(pInst != nullptr, "Failed to allocate dynamic material instance.");

		return pInst;
	}

	void DestroyDynamicMaterialInstance(DynamicMaterialInstance *pInst)
	{
		NA_FREE(pInst);
	}
}