#include "MeshInstance.h"

#include <d3d11.h>

#include "Renderer/IndexBuffer.h"
#include "Renderer/Material.h"
#include "Renderer/RendererD3D.h"
#include "Renderer/VertexBuffer.h"

namespace na
{
	bool MeshInstance::Initialize(VertexBuffer *vertexBuffer, IndexBuffer *indexBuffer, Material *material)
	{
		mVertexBuffer = vertexBuffer;
		mIndexBuffer = indexBuffer;
		mMaterial = material;

		return true;
	}
	
	void MeshInstance::Shutdown()
	{
	}

	void MeshInstance::Render()
	{
		mVertexBuffer->Bind();
		mIndexBuffer->Bind();
		NA_RContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mMaterial->Bind();

		NA_RContext->DrawIndexed((UINT)mIndexBuffer->GetNumIndices(), 0, 0);
	}
}