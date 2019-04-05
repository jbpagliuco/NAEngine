#include "IndexBuffer.h"

#include "RendererD3D.h"

namespace na
{
	bool IndexBuffer::Initialize(IndexType *indices, size_t numIndices, BufferUsage usage)
	{
		NA_ASSERT_RETURN_VALUE(indices != nullptr, false);
		NA_ASSERT_RETURN_VALUE(numIndices > 0, false);

		mNumIndices = numIndices;

#if defined(NA_D3D11)
		D3D11_BUFFER_DESC desc;
		desc.Usage = (D3D11_USAGE)usage;
		desc.ByteWidth = (UINT)(sizeof(IndexType) * mNumIndices);
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = indices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HRESULT hr = NA_RDevice->CreateBuffer(&desc, &data, &mBuffer);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(false), false, "Failed to create index buffer.");
#endif

		return true;
	}

	void IndexBuffer::Shutdown()
	{
		NA_SAFE_RELEASE(mBuffer);
	}

	void IndexBuffer::Bind()
	{
#if defined(NA_D3D11)
		NA_COMPILE_TIME_ASSERT(sizeof(IndexType) == 4, "If this fails, we need to update the format below.");
		NA_RContext->IASetIndexBuffer(mBuffer, DXGI_FORMAT_R32_UINT, 0);
#endif
	}

	PlatformIndexBuffer* IndexBuffer::GetBuffer()const
	{
		return mBuffer;
	}

	size_t IndexBuffer::GetNumIndices()const
	{
		return mNumIndices;
	}

	size_t IndexBuffer::GetIndexSize()const
	{
		return sizeof(IndexType);
	}
}