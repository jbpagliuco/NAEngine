#pragma once

#include <array>

#include "Base/Streaming/AssetFactory.h"

#include "Renderer/NGA/NGACommon.h"
#include "Resources/IndexBuffer.h"
#include "Resources/VertexBuffer.h"

namespace na
{
	struct MeshIndexBufferData
	{
		MeshIndexBufferData() = default;
		MeshIndexBufferData(IndexType* indices, size_t numIndices);

		IndexType *indices;
		size_t numIndices;
	};

	struct MeshData
	{
		void *vertices;
		size_t numVertices;
		size_t vertexStride;

		std::vector<MeshIndexBufferData> indexBuffers;

		NGAVertexFormatDesc mVertexFormat;
		NGAPrimitiveTopology mPrimitiveTopology;
	};

	class Mesh : public AssetFactory<Mesh>
	{
	public:
		bool Initialize(const MeshData &meshData);
		void Shutdown();

		int GetNumGroups()const;

		void Render(int group = 0);

		inline const NGAVertexFormatDesc& GetVertexFormatDesc()const { return mVertexFormat; }

	public:
		static constexpr size_t MAX_INDEX_BUFFERS = 2;

	private:
		VertexBuffer mVertexBuffer;

		IndexBuffer mIndexBuffers[MAX_INDEX_BUFFERS];
		int mNumIndexBuffers;

		NGAVertexFormatDesc mVertexFormat;
		NGAPrimitiveTopology mPrimitiveTopology;
	};
}