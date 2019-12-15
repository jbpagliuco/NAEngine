#include "NGA/NGACommandContext.h"

#if defined(NGA_D3D11)

#include "NGA/NGAResources.h"
#include "NGACoreInternalDX11.h"

namespace na
{
	void NGACommandContext::BindIndexBuffer(const NGABuffer &indexBuffer, NGAIndexBufferType indexBufferType)
	{
		const DXGI_FORMAT indexFormat = (indexBufferType == NGAIndexBufferType::IBT_16BIT) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		NgaDx11State.mContext->IASetIndexBuffer(indexBuffer.mBuffer, indexFormat, 0);
	}

	void NGACommandContext::BindVertexBuffer(const NGABuffer &vertexBuffer, size_t vertexStride)
	{
		UINT stride = vertexStride;
		UINT offset = 0;
		NgaDx11State.mContext->IASetVertexBuffers(0, 1, &vertexBuffer.mBuffer, &stride, &offset);
	}
}

#endif // defined(NGA_D3D11)