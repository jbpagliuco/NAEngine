#pragma once

#include "Base/Streaming/AssetFactory.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"

namespace na
{
	struct MeshData
	{
		void *vertices;
		size_t numVertices;
		size_t vertexStride;

		IndexType *indices;
		size_t numIndices;

		VertexFormatDesc mVertexFormat;
	};

	class Mesh : public AssetFactory<Mesh>
	{
	public:
		bool Initialize(const MeshData &meshData);
		void Shutdown();

		void Render();

		inline const VertexFormatDesc& GetVertexFormatDesc()const { return mVertexFormat; }

	private:
		VertexBuffer mVertexBuffer;
		IndexBuffer mIndexBuffer;

		VertexFormatDesc mVertexFormat;
	};
}