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
		mMesh->AddRef();

		mMaterial = Material::Get(matID);
		mMaterial->AddRef();

		return true;
	}
	
	void MeshInstance::Shutdown()
	{
		Mesh::Destroy(mMesh);
		Material::Destroy(mMaterial);
	}

	void MeshInstance::Render()
	{
		mMaterial->Bind();
		mMesh->Render();
	}
}