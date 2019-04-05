#include "VertexBuffer.h"

#include "Base/Util/Util.h"

#include "RendererD3D.h"

namespace na
{
	bool VertexBuffer::Initialize(void *vertices, size_t numVertices, size_t vertexStride, BufferUsage usage)
	{
		NA_ASSERT_RETURN_VALUE(vertices != nullptr, false);
		NA_ASSERT_RETURN_VALUE(numVertices > 0, false);

		mNumVertices = numVertices;
		mVertexStride = vertexStride;

#if defined(NA_D3D11)
		D3D11_BUFFER_DESC desc;
		desc.Usage = (D3D11_USAGE)usage;
		desc.ByteWidth = (UINT)(vertexStride * mNumVertices);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HRESULT hr = NA_RDevice->CreateBuffer(&desc, &data, &mBuffer);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(false), false, "Failed to create vertex buffer.");
#endif

		return true;
	}

	void VertexBuffer::Shutdown()
	{
		NA_SAFE_RELEASE(mBuffer);
	}

	void VertexBuffer::Bind()
	{
#if defined(NA_D3D11)
		UINT stride = (UINT)mVertexStride;
		UINT offset = 0;
		NA_RContext->IASetVertexBuffers(0, 1, &mBuffer, &stride, &offset);
#endif
	}

	PlatformVertexBuffer* VertexBuffer::GetBuffer()const
	{
		return mBuffer;
	}

	size_t VertexBuffer::GetNumVertices()const
	{
		return mNumVertices;
	}
	
	size_t VertexBuffer::GetVertexStride()const
	{
		return mVertexStride;
	}

}