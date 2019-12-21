#include "NGA/NGACommandContext.h"

#if defined(NGA_D3D11)

#include "NGA/NGAInputLayout.h"
#include "NGA/NGAResources.h"
#include "NGA/NGAResourceViews.h"
#include "NGA/NGASamplerState.h"
#include "NGACoreInternalDX11.h"

namespace na
{
	void NGACommandContext::SetPrimitiveTopology(NGAPrimitiveTopology primTopology)
	{
		NgaDx11State.mContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)primTopology);
	}

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

	void NGACommandContext::BindInputLayout(const NGAInputLayout &inputLayout)
	{
		NgaDx11State.mContext->IASetInputLayout(inputLayout.mInputLayout);
	}

	void NGACommandContext::BindShader(const NGAShader &shader)
	{
		switch (shader.GetType())
		{
		case NGAShaderType::VERTEX:
			NgaDx11State.mContext->VSSetShader(shader.mVertexShader, nullptr, 0);
			break;

		case NGAShaderType::PIXEL:
			NgaDx11State.mContext->PSSetShader(shader.mPixelShader, nullptr, 0);
			break;
		}
	}

	void NGACommandContext::BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot)
	{
		if (stage & NGA_SHADER_STAGE_VERTEX) {
			NgaDx11State.mContext->VSSetConstantBuffers(slot, 1, &constantBuffer.mBuffer);
		}

		if (stage & NGA_SHADER_STAGE_PIXEL) {
			NgaDx11State.mContext->PSSetConstantBuffers(slot, 1, &constantBuffer.mBuffer);
		}
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

	void NGACommandContext::ClearRenderTarget(const NGARenderTargetView &renderTargetView, const float *clearColor)
	{
		NgaDx11State.mContext->ClearRenderTargetView(renderTargetView.mView, clearColor);
	}

	void NGACommandContext::ClearDepthStencilView(const NGADepthStencilView &depthStencilView)
	{
		NgaDx11State.mContext->ClearDepthStencilView(depthStencilView.mView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void NGACommandContext::BindRenderTarget(const NGARenderTargetView &renderTarget, const NGADepthStencilView &depthStencilView)
	{
		NgaDx11State.mContext->OMSetRenderTargets(1, &renderTarget.mView, depthStencilView.mView);
	}

	void NGACommandContext::MapBufferData(const NGABuffer &buffer, void *data)
	{
		const NGABufferUsage usage = buffer.mDesc.mUsage;
		NA_ASSERT_RETURN((usage & NGA_BUFFER_USAGE_CPU_WRITE) || (usage & NGA_BUFFER_USAGE_CPU_READ_WRITE));

		D3D11_MAPPED_SUBRESOURCE res;

		HRESULT hr = NgaDx11State.mContext->Map(buffer.mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		NA_ASSERT_RETURN(SUCCEEDED(hr), false, "ID3D11DeviceContext::Map() failed with HRESULT %X", hr);

		memcpy(res.pData, data, buffer.mDesc.mSizeInBytes);

		NgaDx11State.mContext->Unmap(buffer.mBuffer, 0);
	}

	void NGACommandContext::DrawIndexed(unsigned int indexCount)
	{
		NgaDx11State.mContext->DrawIndexed(indexCount, 0, 0);
	}
}

#endif // defined(NGA_D3D11)