#include "Mesh.h"

#include "RendererD3D.h"

namespace na
{
	NA_FACTORY_BUILD(Mesh);

	bool Mesh::Initialize(const MeshData &meshData)
	{
		if (!mVertexBuffer.Initialize(meshData.vertices, meshData.numVertices, meshData.vertexStride, BufferUsage::DEFAULT)) {
			return false;
		}

		if (!mIndexBuffer.Initialize(meshData.indices, meshData.numIndices, BufferUsage::DEFAULT)) {
			mVertexBuffer.Shutdown();
			return false;
		}

		return true;
	}

	void Mesh::Shutdown()
	{
		mVertexBuffer.Shutdown();
		mIndexBuffer.Shutdown();
	}

	void Mesh::Render()
	{
		mVertexBuffer.Bind();
		mIndexBuffer.Bind();

		NA_RContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		NA_RContext->DrawIndexed((UINT)mIndexBuffer.GetNumIndices(), 0, 0);
	}
}