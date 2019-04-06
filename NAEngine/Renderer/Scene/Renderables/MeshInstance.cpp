#include "MeshInstance.h"

#include <d3d11.h>

#include "Renderer/IndexBuffer.h"
#include "Renderer/Material.h"
#include "Renderer/RendererD3D.h"
#include "Renderer/VertexBuffer.h"

namespace na
{
	NA_FACTORY_BUILD(MeshInstance);

	bool MeshInstance::Initialize(const MeshData &meshData, Material *material)
	{
		// TODO: Reuse meshes
		mMesh = Mesh::Create();
		mMesh->Initialize(meshData);

		mMaterial = material;

		return true;
	}
	
	void MeshInstance::Shutdown()
	{
		Mesh::Destroy(mMesh);
		mMaterial->Shutdown();
	}

	void MeshInstance::Render()
	{
		mMaterial->Bind();
		mMesh->Render();
	}
}