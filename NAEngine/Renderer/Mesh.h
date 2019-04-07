#pragma once

#include "Base/Streaming/Stream.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace na
{
	struct MeshData
	{
		void *vertices;
		size_t numVertices;
		size_t vertexStride;

		IndexType *indices;
		size_t numIndices;
	};

	class Mesh : public Factory<Mesh>
	{
	public:
		bool Initialize(const MeshData &meshData);
		void Shutdown();

		void Render();

	private:
		VertexBuffer mVertexBuffer;
		IndexBuffer mIndexBuffer;
	};
}