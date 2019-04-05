#pragma once

#include "Buffer.h"

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11Buffer PlatformVertexBuffer;
#endif

	class VertexBuffer : public Buffer
	{
	public:
		bool Initialize(void *vertices, size_t numVertices, size_t vertexStride, BufferUsage usage);
		void Shutdown();

		void Bind();

		PlatformVertexBuffer* GetBuffer()const;
		size_t GetNumVertices()const;
		size_t GetVertexStride()const;

	private:
		PlatformVertexBuffer *mBuffer;

		size_t mNumVertices;
		size_t mVertexStride;
	};
}