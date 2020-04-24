#include "DynamicMaterial.h"

#include "Renderer/Renderer.h"
#include "Renderer/Resources/RenderTarget.h"
#include "Renderer/Resources/Texture.h"

namespace na
{
	bool DynamicMaterial::Initialize(Shader *shader, size_t parameterByteLength, const DeserializationParameterMap &map, void *defaultParameterData, const std::vector<const Texture*> &defaultTextures)
	{
		if (!Material::Initialize(shader)) {
			return false;
		}

		if (!mConstantBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, parameterByteLength)) {
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
				info.mLength = GetFormatByteSize(GetFormatFromString(type.c_str()));
				info.mOffset = curParameterByteOffset;

				mParameterMap[name] = info;

				curParameterByteOffset += info.mLength;
			}
		}

		// Set default data
		mDefaultParameterData = NA_ALLOC(parameterByteLength);
		memcpy(mDefaultParameterData, defaultParameterData, parameterByteLength);

		mDefaultTextures = defaultTextures;

		// Make sure all of these textures are shader resources
		for (auto& texture : mDefaultTextures) {
			NA_RENDER_ASSERT_RETURN_VALUE(texture->IsShaderResource(), false, "Static material was given a texture that is not a shader resource.");
		}

		return true;
	}

	void DynamicMaterial::Shutdown()
	{
		Material::Shutdown();

		mConstantBuffer.Shutdown();

		NA_FREE(mDefaultParameterData);
	}

	void DynamicMaterial::Bind() 
	{
		Material::Bind();

		// Bind default data. This is not great.
		mConstantBuffer.Map(mDefaultParameterData);

		// Bind constant data
		NA_RStateManager->BindConstantBuffer(mConstantBuffer.GetBuffer(), NGA_SHADER_STAGE_PIXEL, 0);

		// Bind textures
		for (int i = 0; i < mDefaultTextures.size(); ++i) {
			NA_RStateManager->BindShaderResource(*mDefaultTextures[i], NGA_SHADER_STAGE_PIXEL, i);
			NA_RStateManager->BindSamplerState(mDefaultTextures[i]->GetSamplerState(), NGA_SHADER_STAGE_PIXEL, i);
		}
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

	std::vector<const Texture*> DynamicMaterial::GetDefaultTextures()const
	{
		return mDefaultTextures;
	}





	DynamicMaterialInstance::DynamicMaterialInstance(DynamicMaterial *pMaterial)
	{
		mParent = pMaterial;

		mParameterData = NA_ALLOC(mParent->mConstantBuffer.GetSize());
		memcpy(mParameterData, mParent->GetDefaultParameterData(), mParent->mConstantBuffer.GetSize());
		
		for (auto &tex : mParent->GetDefaultTextures()) {
			mTextures.push_back(tex);
		}
	}

	void DynamicMaterialInstance::Shutdown()
	{
		NA_FREE(mParameterData);

		mTextures.erase(mTextures.begin(), mTextures.end());
	}

	void DynamicMaterialInstance::BindDynamicData()
	{
		// Map our data to the material's constant buffer.
		mParent->mConstantBuffer.Map(mParameterData);

		// Bind textures
		for (int i = 0; i < mTextures.size(); ++i) {
			if (mTextures[i] != nullptr) {
				NA_RStateManager->BindShaderResource(*mTextures[i], NGA_SHADER_STAGE_PIXEL, i);
				NA_RStateManager->BindSamplerState(mTextures[i]->GetSamplerState(), NGA_SHADER_STAGE_PIXEL, i);
			}
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

	void DynamicMaterialInstance::SetVectorParameter(const std::string &name, const Vector4f &value)
	{
		DynamicMaterialParameterInfo paramInfo;
		if (mParent->GetParameterInfo(paramInfo, name)) {
			NA_ASSERT(paramInfo.mLength == sizeof(Vector4f), "Trying to set vector data for a parameter of length %zu", paramInfo.mLength);
			memcpy(mParameterData, &value, paramInfo.mLength);
		}
	}

	void DynamicMaterialInstance::SetTextureParameter(const std::string &name, const Texture *pTexture)
	{
		NA_ASSERT_RETURN(pTexture->IsShaderResource(), "Texture is not a shader resource.");

		int index = mParent->GetTextureParameterIndex(name);
		NA_ASSERT_RETURN(index >= 0 && index < mTextures.size(), "Failed to find valid index for a texture parameter with name '%s'", name);

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