#include "MeshInstance.h"

#include <d3d11.h>

#include "Renderer/IndexBuffer.h"
#include "Renderer/Material/Material.h"
#include "Renderer/RendererD3D.h"
#include "Renderer/VertexBuffer.h"

namespace na
{
	bool MeshInstance::Initialize(AssetID meshID, AssetID matID)
	{
		mMesh = Mesh::Get(meshID);
		mMaterial = GetMaterialByID(matID);

		const VertexShader &vs = mMaterial->GetShader()->GetVertexShader();

		// Build the input layout
		VertexFormatDesc vDesc;
		for (auto &shaderInput : vs.GetVertexFormatDesc().mAttributes) {
			VertexAttribute attr;
			
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

		mInputLayout.Initialize(vDesc, vs);

		return true;
	}
	
	void MeshInstance::Shutdown()
	{
		NA_SAFE_RELEASE_ASSET_OBJECT(mMesh);
		
		ReleaseMaterial(mMaterial);
		mMaterial = nullptr;

		mInputLayout.Shutdown();
	}

	void MeshInstance::Render()
	{
		mMaterial->Bind();
		mInputLayout.Bind();
		mMesh->Render();
	}
}