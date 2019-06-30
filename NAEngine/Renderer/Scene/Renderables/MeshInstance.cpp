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

		return true;
	}
	
	void MeshInstance::Shutdown()
	{
		NA_SAFE_RELEASE_ASSET_OBJECT(mMesh);
		
		ReleaseMaterial(mMaterial);
		mMaterial = nullptr;
	}

	void MeshInstance::Render()
	{
		mMaterial->Bind();
		mMesh->Render();
	}
}