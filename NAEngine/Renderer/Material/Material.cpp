#include "Material.h"

#include "StaticMaterial.h"

namespace na
{
	bool Material::Initialize(AssetID shaderID)
	{
		mShader = Shader::Get(shaderID);

		return true;
	}
	
	void Material::Shutdown()
	{
		NA_SAFE_RELEASE_ASSET_OBJECT(mShader);
	}

	void Material::Bind()
	{
		mShader->Bind();
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