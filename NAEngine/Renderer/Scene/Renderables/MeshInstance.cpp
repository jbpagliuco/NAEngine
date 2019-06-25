#include "MeshInstance.h"

#include <d3d11.h>

#include "Renderer/IndexBuffer.h"
#include "Renderer/Material.h"
#include "Renderer/RendererD3D.h"
#include "Renderer/VertexBuffer.h"

namespace na
{
	bool MeshInstance::Initialize(AssetID meshID, AssetID matID)
	{
		mMesh = Mesh::Get(meshID);
		mMaterial = Material::Get(matID);

		return true;
	}
	
	void MeshInstance::Shutdown()
	{
		NA_SAFE_RELEASE_ASSET_OBJECT(mMesh);
		NA_SAFE_RELEASE_ASSET_OBJECT(mMaterial);
	}

	void MeshInstance::Render()
	{
		mMaterial->Bind();
		mMesh->Render();
	}
}