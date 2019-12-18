#pragma once

#include "NGACommon.h"

namespace na
{
	class NGABuffer;
	class NGASamplerState;
	class NGAShaderResourceView;

	class NGACommandContext
	{
	public:
		void BindIndexBuffer(const NGABuffer &indexBuffer, NGAIndexBufferType indexBufferType);
		void BindVertexBuffer(const NGABuffer &vertexBuffer, size_t vertexStride);

		void BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot);
		void BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot);
		void BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot);

		void MapBufferData(const NGABuffer &buffer, void *data);
	};
}