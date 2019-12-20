#pragma once

#include "NGACommon.h"

#include "NGAShader.h"

namespace na
{
	class NGABuffer;
	class NGAInputLayout;
	class NGASamplerState;
	class NGAShaderResourceView;

	class NGACommandContext
	{
	public:
		void BindIndexBuffer(const NGABuffer &indexBuffer, NGAIndexBufferType indexBufferType);
		void BindVertexBuffer(const NGABuffer &vertexBuffer, size_t vertexStride);

		void BindInputLayout(const NGAInputLayout &inputLayout);

		void BindShader(const NGAShader &shader);

		void BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot);
		void BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot);
		void BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot);

		void MapBufferData(const NGABuffer &buffer, void *data);
	};
}