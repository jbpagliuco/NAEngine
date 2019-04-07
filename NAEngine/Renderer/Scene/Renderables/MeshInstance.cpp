#include "MeshInstance.h"

#include <d3d11.h>

#include "Renderer/IndexBuffer.h"
#include "Renderer/Material.h"
#include "Renderer/RendererD3D.h"
#include "Renderer/VertexBuffer.h"

namespace na
{
	bool MeshInstance::Initialize(const char *meshFilename, Material *material)
	{
		AssetID id = StreamAsset(meshFilename);
		mMesh = Mesh::Create(id);

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