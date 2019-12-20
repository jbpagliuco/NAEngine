#include "MeshInstance.h"

#include <d3d11.h>

#include "Renderer/Material/Material.h"
#include "Renderer/Material/DynamicMaterial.h"
#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/Resources/IndexBuffer.h"
#include "Renderer/Resources/VertexBuffer.h"

namespace na
{
	bool MeshInstance::Initialize(AssetID meshID, AssetID matID)
	{
		mMesh = Mesh::Get(meshID);
		mMaterial = GetMaterialByID(matID);

		const VertexShader &vs = mMaterial->GetShader()->GetVertexShader();

		// Build the input layout
		NGAVertexFormatDesc vDesc;
		for (auto &shaderInput : vs.GetVertexFormatDesc().mAttributes) {
			NGAVertexAttribute attr;
			
			// Find the matching input from the mesh.
			bool found = false;
			for (auto &meshAttr : mMesh->GetVertexFormatDesc().mAttributes) {
				if (shaderInput.mSemanticType == meshAttr.mSemanticType && shaderInput.mSemanticIndex == meshAttr.mSemanticIndex) {
					attr = shaderInput;
					attr.mOffset = meshAttr.mOffset;
					found = true;
					break;
				}
			}

			NA_ASSERT_RETURN_VALUE(found, false,
				"Shader '%s' requires %s%d, but mesh '%s' does not supply it.",
				GetAssetFilename(matID),
				GetSemanticName(shaderInput.mSemanticType),
				shaderInput.mSemanticIndex,
				GetAssetFilename(meshID));

			vDesc.mAttributes.push_back(attr);
		}

		mInputLayout.Construct(vDesc, vs.GetShader());

		return true;
	}
	
	void MeshInstance::Shutdown()
	{
		NA_SAFE_RELEASE_ASSET_OBJECT(mMesh);

		if (mDynMaterialInst) {
			mDynMaterialInst->Shutdown();
			DestroyDynamicMaterialInstance(mDynMaterialInst);
			mDynMaterialInst = nullptr;
		}

		ReleaseMaterial(mMaterial);
		mMaterial = nullptr;

		mInputLayout.Destruct();
	}

	DynamicMaterialInstance* MeshInstance::CreateDynamicMaterialInstance()
	{
		NA_ASSERT_RETURN_VALUE(mMaterial->GetMaterialType() == MATERIAL_TYPE_DYNAMIC, nullptr);

		mDynMaterialInst = na::CreateDynamicMaterialInstance(static_cast<DynamicMaterial*>(mMaterial));
		NA_ASSERT(mDynMaterialInst != nullptr, "Failed to create dynamic material instance");

		return mDynMaterialInst;
	}

	DynamicMaterialInstance* MeshInstance::GetDynamicMaterialInstance()
	{
		return mDynMaterialInst;
	}

	void MeshInstance::Render()
	{
		mMaterial->Bind();

		if (mDynMaterialInst) {
			mDynMaterialInst->BindData();
		}

		NA_RStateData->BindInputLayout(mInputLayout);
		mMesh->Render();
	}
}