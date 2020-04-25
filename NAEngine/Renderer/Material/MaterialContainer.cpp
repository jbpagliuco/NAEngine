#include "MaterialContainer.h"

#include "Base/Debug/Assert.h"

#include "Renderer/Material/Material.h"
#include "Renderer/Material/DynamicMaterial.h"

namespace na
{
	bool MaterialContainer::Initialize(Material* material)
	{
		mMaterial = material;

		return true;
	}

	void MaterialContainer::Shutdown()
	{
		if (mDynMaterialInst) {
			mDynMaterialInst->Shutdown();
			DestroyDynamicMaterialInstance(mDynMaterialInst);
			mDynMaterialInst = nullptr;
		}

		mMaterial = nullptr;
	}

	void MaterialContainer::Bind()
	{
		mMaterial->Bind();

		if (mDynMaterialInst) {
			mDynMaterialInst->BindDynamicData();
		}
	}

	void MaterialContainer::CreateDynamicMaterialInstance()
	{
		NA_ASSERT_RETURN(mMaterial->GetMaterialType() == MATERIAL_TYPE_DYNAMIC);

		mDynMaterialInst = na::CreateDynamicMaterialInstance(static_cast<DynamicMaterial*>(mMaterial));
		NA_ASSERT(mDynMaterialInst != nullptr, "Failed to create dynamic material instance");
	}

	bool MaterialContainer::SetFloatParameter(const std::string& name, float value)
	{
		NA_ASSERT_RETURN_VALUE(mDynMaterialInst != nullptr, false);
		mDynMaterialInst->SetFloatParameter(name, value);

		return true;
	}

	bool MaterialContainer::SetVectorParameter(const std::string& name, const Vector4f& value)
	{
		NA_ASSERT_RETURN_VALUE(mDynMaterialInst != nullptr, false);
		mDynMaterialInst->SetVectorParameter(name, value);

		return true;
	}

	bool MaterialContainer::SetTextureParameter(const std::string& name, const Texture* texture)
	{
		NA_ASSERT_RETURN_VALUE(mDynMaterialInst != nullptr, false);
		mDynMaterialInst->SetTextureParameter(name, texture);

		return true;
	}

	Material* MaterialContainer::GetMaterial()const
	{
		return mMaterial;
	}
}