#pragma once

#include "Material.h"

#include "Base/Util/Color.h"
#include "Base/Util/Serialize.h"
#include "Renderer/ConstantBuffer.h"

namespace na
{
	class DynamicMaterialInstance;
	class Texture;

	struct DynamicMaterialParameterInfo
	{
		size_t mOffset;
		size_t mLength;
	};

	class DynamicMaterial : public Material, public AssetFactory<DynamicMaterial>
	{
	public:
		virtual bool Initialize(AssetID shaderID, size_t parameterByteLength, const DeserializationParameterMap &map, void *defaultParameterData, const std::vector<AssetID> &defaultTextures);
		virtual void Shutdown() override;

		virtual void Bind() override;

		virtual int GetMaterialType()const override { return MATERIAL_TYPE_DYNAMIC; }

	protected:
		bool GetParameterInfo(DynamicMaterialParameterInfo &info, const std::string &name);
		int GetTextureParameterIndex(const std::string &name);

		void* GetDefaultParameterData()const;
		std::vector<AssetID> GetDefaultTextures()const;

		ConstantBuffer mConstantBuffer;

	private:
		std::map<std::string, DynamicMaterialParameterInfo> mParameterMap;
		std::map<std::string, int> mTextureParameterMap;

		void *mDefaultParameterData;
		std::vector<Texture*> mDefaultTextures;

		friend class DynamicMaterialInstance;
	};

	class DynamicMaterialInstance
	{
	public:
		DynamicMaterialInstance(DynamicMaterial *pMaterial);

		void Shutdown();

		void BindData();

		void SetFloatParameter(const std::string &name, float value);
		void SetVectorParameter(const std::string &name, const DirectX::XMFLOAT4 &value);

		void SetTextureParameter(const std::string &name, const std::string &filename);
		void SetTextureParameter(const std::string &name, Texture *pTexture);

	private:
		DynamicMaterial *mParent;

		void *mParameterData;
		std::vector<Texture*> mTextures;
	};

	DynamicMaterialInstance* CreateDynamicMaterialInstance(DynamicMaterial *pParent);
	void DestroyDynamicMaterialInstance(DynamicMaterialInstance *pInst);
}