#pragma once

#include "Renderer/RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

#include "Renderer/Buffer.h"

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11Buffer PlatformConstantBuffer;
#endif

	class ConstantBuffer
	{
	public:
		bool Initialize(BufferUsage usage, void *pData, size_t dataByteLength);
		void Shutdown();

		bool Map(void *pData);
		
		PlatformConstantBuffer* GetBuffer()const;
		size_t GetSize()const;

	private:
		PlatformConstantBuffer *mBuffer;
	
		BufferUsage mUsage;
		size_t mSize;
	};
}