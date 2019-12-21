#pragma once

#include "NGACommon.h"

#include "NGAShader.h"

namespace na
{
	class NGABuffer;
	class NGADepthStencilView;
	class NGAInputLayout;
	class NGARenderTargetView;
	class NGASamplerState;
	class NGAShaderResourceView;
	struct NGAViewport;

	class NGACommandContext
	{
	public:
		void SetViewport(const NGAViewport &viewport);

		void SetPrimitiveTopology(NGAPrimitiveTopology primTopology);

		void BindIndexBuffer(const NGABuffer &indexBuffer, NGAIndexBufferType indexBufferType);
		void BindVertexBuffer(const NGABuffer &vertexBuffer, size_t vertexStride);

		void BindInputLayout(const NGAInputLayout &inputLayout);

		void BindShader(const NGAShader &shader);

		void BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot);
		void BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot);
		void BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot);

		void ClearRenderTarget(const NGARenderTargetView &renderTargetView, const float *clearColor);
		void ClearDepthStencilView(const NGADepthStencilView &depthStencilView);
		void BindRenderTarget(const NGARenderTargetView &renderTargetView, const NGADepthStencilView &depthStencilView);

		void MapBufferData(const NGABuffer &buffer, void *data);

		void DrawIndexed(unsigned int indexCount);
	};
}