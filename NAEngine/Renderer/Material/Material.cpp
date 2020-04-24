#include "Material.h"

#include "StaticMaterial.h"
#include "DynamicMaterial.h"

namespace na
{
	bool Material::Initialize(Shader *shader)
	{
		mShader = shader;

		return true;
	}
	
	void Material::Shutdown()
	{
		mShader = nullptr;
	}

	void Material::Bind()
	{
		mShader->Bind();
	}


	Material* GetMaterialByID(AssetID matID)
	{
		if (StaticMaterial::Exists(matID)) {
			return StaticMaterial::Get(matID);
		} else if (DynamicMaterial::Exists(matID)) {
			return DynamicMaterial::Get(matID);
		} else {
			NA_ASSERT(false, "Unknown material type for material %s", GetAssetFilename(matID));
		}

		return nullptr;
	}

	void ReleaseMaterial(Material *pMaterial)
	{
		const int matType = pMaterial->GetMaterialType();
		if (matType == MATERIAL_TYPE_STATIC) {
			ReleaseAsset(static_cast<StaticMaterial*>(pMaterial)->GetID());
		} else if (matType == MATERIAL_TYPE_DYNAMIC) {
			ReleaseAsset(static_cast<DynamicMaterial*>(pMaterial)->GetID());
		} else {
			NA_ASSERT(false, "Unknown material type %d", matType);
		}
	}
}