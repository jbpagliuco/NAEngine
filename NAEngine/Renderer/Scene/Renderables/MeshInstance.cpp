#include "MeshInstance.h"

#include "Renderer/Material/Material.h"
#include "Renderer/Material/DynamicMaterial.h"
#include "Renderer/Material/MaterialContainer.h"
#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/Resources/IndexBuffer.h"
#include "Renderer/Resources/VertexBuffer.h"

namespace na
{
	bool MeshInstance::Initialize(Mesh *mesh, std::vector<MaterialContainer*> materialContainers)
	{
		mMesh = mesh;
		mMaterialContainers = materialContainers;

		for (int i = 0; i < mMaterialContainers.size(); ++i) {
			Material* material = mMaterialContainers[i]->GetMaterial();

			const VertexShader& vs = material->GetShader()->GetVertexShader();

			// Build the input layout
			NGAVertexFormatDesc vDesc;
			for (auto& shaderInput : vs.GetVertexFormatDesc().mAttributes) {
				NGAVertexAttribute attr;

				// Find the matching input from the mesh.
				bool found = false;
				for (auto& meshAttr : mMesh->GetVertexFormatDesc().mAttributes) {
					if (shaderInput.mSemanticType == meshAttr.mSemanticType && shaderInput.mSemanticIndex == meshAttr.mSemanticIndex) {
						attr = shaderInput;
						attr.mOffset = meshAttr.mOffset;
						found = true;
						break;
					}
				}

				NA_ASSERT_RETURN_VALUE(found, false,
					"Shader '%s' requires %s%d, but mesh '%s' does not supply it.",
					GetAssetFilename(material->GetShader()->GetID()),
					GetSemanticName(shaderInput.mSemanticType),
					shaderInput.mSemanticIndex,
					GetAssetFilename(mMesh->GetID()));

				vDesc.mAttributes.push_back(attr);
			}

			mInputLayouts[i].Construct(vDesc, vs.GetShader());
		}

		return true;
	}
	
	void MeshInstance::Shutdown()
	{
		mMesh = nullptr;

		for (int i = 0; i < mMaterialContainers.size(); ++i) {
			mInputLayouts[i].Destruct();
		}
		mMaterialContainers.clear();
	}

	void MeshInstance::Render(bool bindMaterial)
	{
		NA_ASSERT_RETURN(mMaterialContainers.size() == mMesh->GetNumGroups());

		for (int group = 0; group < mMesh->GetNumGroups(); ++group) {
			if (bindMaterial) {
				mMaterialContainers[group]->Bind();
			}

			NA_RStateManager->BindInputLayout(mInputLayouts[group]);

			mMesh->Render(group);
		}
	}
}