#include "Mesh.h"

#include "Renderer.h"

namespace na
{
	NA_FACTORY_SETUP(Mesh);

	bool Mesh::Initialize(const MeshData &meshData)
	{
		if (!mVertexBuffer.Initialize(meshData.vertices, meshData.numVertices, meshData.vertexStride)) {
			return false;
		}

		if (!mIndexBuffer.Initialize(meshData.indices, meshData.numIndices)) {
			mVertexBuffer.Shutdown();
			return false;
		}

		mVertexFormat = meshData.mVertexFormat;

		return true;
	}

	void Mesh::Shutdown()
	{
		mVertexBuffer.Shutdown();
		mIndexBuffer.Shutdown();
	}

	void Mesh::Render()
	{
		NA_RStateData->BindIndexBuffer(mIndexBuffer);
		NA_RStateData->BindVertexBuffer(mVertexBuffer);

		NA_RContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		NA_RStateData->DrawIndexed(mIndexBuffer);
	}
}