#pragma once

#include "Material.h"

#include "Base/Util/Color.h"
#include "Base/Util/Serialize.h"
#include "Renderer/Resources/ConstantBuffer.h"

namespace na
{
	class DynamicMaterialInstance;
	class RenderTarget;
	class Texture;

	struct DynamicMaterialParameterInfo
	{
		size_t mOffset;
		size_t mLength;
	};

	class DynamicMaterial : public Material, public AssetFactory<DynamicMaterial>
	{
	public:
		virtual bool Initialize(Shader *shader, size_t parameterByteLength, const DeserializationParameterMap &map, void *defaultParameterData, const std::vector<const Texture*> &defaultTextures);
		virtual void Shutdown() override;

		virtual void Bind() override;

		virtual AssetID GetID()const override { return AssetFactory<DynamicMaterial>::GetID(); }
		virtual int GetMaterialType()const override { return MATERIAL_TYPE_DYNAMIC; }

		const std::vector<const Texture*>& GetTextures()const { return mDefaultTextures; }

	protected:
		bool GetParameterInfo(DynamicMaterialParameterInfo &info, const std::string &name);
		int GetTextureParameterIndex(const std::string &name);

		void* GetDefaultParameterData()const;
		std::vector<const Texture*> GetDefaultTextures()const;

		ConstantBuffer mConstantBuffer;

	private:
		std::map<std::string, DynamicMaterialParameterInfo> mParameterMap;
		std::map<std::string, int> mTextureParameterMap;

		void *mDefaultParameterData;
		std::vector<const Texture*> mDefaultTextures;

		friend class DynamicMaterialInstance;
	};



	class DynamicMaterialInstance
	{
	public:
		DynamicMaterialInstance(DynamicMaterial *pMaterial);

		void Shutdown();

		void BindDynamicData();

		void SetFloatParameter(const std::string &name, float value);
		void SetVectorParameter(const std::string& name, const Vector4f& value);
		void SetTextureParameter(const std::string &name, const Texture *pTexture);
		
	private:
		DynamicMaterial *mParent;

		void *mParameterData;
		std::vector<const Texture*> mTextures;
	};

	DynamicMaterialInstance* CreateDynamicMaterialInstance(DynamicMaterial *pParent);
	void DestroyDynamicMaterialInstance(DynamicMaterialInstance *pInst);
}