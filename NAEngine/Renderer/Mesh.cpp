#include "Mesh.h"

#include "Renderer.h"

namespace na
{
	NA_FACTORY_SETUP(Mesh);

	MeshIndexBufferData::MeshIndexBufferData(IndexType* indices, size_t numIndices) :
		indices(indices),
		numIndices(numIndices)
	{
	}

	bool Mesh::Initialize(const MeshData &meshData)
	{
		NA_ASSERT_RETURN_VALUE(meshData.indexBuffers.size() <= MAX_INDEX_BUFFERS, false);

		if (!mVertexBuffer.Initialize(meshData.vertices, meshData.numVertices, meshData.vertexStride)) {
			return false;
		}

		bool failed = false;
		int i = 0;
		for (const auto& indexBufferData : meshData.indexBuffers) {
			if (!mIndexBuffers[i++].Initialize(indexBufferData.indices, indexBufferData.numIndices)) {
				failed = true;
				break;
			}
		}

		if (failed) {
			Shutdown();
			return false;
		}

		mNumIndexBuffers = (int)meshData.indexBuffers.size();

		mVertexFormat = meshData.mVertexFormat;
		mPrimitiveTopology = meshData.mPrimitiveTopology;

		return true;
	}

	void Mesh::Shutdown()
	{
		mVertexBuffer.Shutdown();

		for (int i = 0; i < mNumIndexBuffers; ++i) {
			mIndexBuffers[i].Shutdown();
		}
	}

	int Mesh::GetNumGroups()const
	{
		// returns the number of index buffers or 1 which means this is just vertex data
		return max(mNumIndexBuffers, 1);
	}

	void Mesh::Render(int group)
	{
		NA_RStateManager->SetPrimitiveTopology(mPrimitiveTopology);
		NA_RStateManager->BindIndexBuffer(mIndexBuffers[group]);
		NA_RStateManager->BindVertexBuffer(mVertexBuffer);

		if (mNumIndexBuffers > 0) {
			NA_RStateManager->DrawIndexed(mIndexBuffers[group]);
		}
		else {
			NA_RStateManager->Draw(mVertexBuffer);
		}
	}
}