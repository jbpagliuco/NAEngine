#pragma once

#include <stdint.h>

#include "Buffer.h"

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11Buffer PlatformIndexBuffer;
#endif

	typedef uint32_t IndexType;

	class IndexBuffer : public Buffer
	{
	public:
		bool Initialize(IndexType *indices, size_t numIndices, BufferUsage usage);
		void Shutdown();

		void Bind();

		PlatformIndexBuffer* GetBuffer()const;
		size_t GetNumIndices()const;
		size_t GetIndexSize()const;

	private:
		PlatformIndexBuffer *mBuffer;

		size_t mNumIndices;
	};
}