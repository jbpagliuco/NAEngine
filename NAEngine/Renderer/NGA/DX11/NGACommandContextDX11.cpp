#include "NGA/NGACommandContext.h"

#if defined(NGA_D3D11)

#include "NGA/NGAResources.h"
#include "NGA/NGAResourceViews.h"
#include "NGA/NGASamplerState.h"
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
		UINT stride = (UINT)vertexStride;
		UINT offset = 0;
		NgaDx11State.mContext->IASetVertexBuffers(0, 1, &vertexBuffer.mBuffer, &stride, &offset);
	}

	void NGACommandContext::BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot)
	{
		if (stage & NGA_SHADER_STAGE_VERTEX) {
			NgaDx11State.mContext->VSSetShaderResources(slot, 1, &view.mView);
		}

		if (stage & NGA_SHADER_STAGE_PIXEL) {
			NgaDx11State.mContext->PSSetShaderResources(slot, 1, &view.mView);
		}
	}

	void NGACommandContext::BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot)
	{
		if (stage & NGA_SHADER_STAGE_VERTEX) {
			NgaDx11State.mContext->VSSetSamplers(slot, 1, &samplerState.mSamplerState);
		}

		if (stage & NGA_SHADER_STAGE_PIXEL) {
			NgaDx11State.mContext->PSSetSamplers(slot, 1, &samplerState.mSamplerState);
		}
	}
}

#endif // defined(NGA_D3D11)