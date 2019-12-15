#pragma once

#include "NGACommon.h"

namespace na
{
	class NGABuffer;

	class NGACommandContext
	{
	public:
		void BindIndexBuffer(const NGABuffer &indexBuffer, NGAIndexBufferType indexBufferType);
		void BindVertexBuffer(const NGABuffer &vertexBuffer, size_t vertexStride);
	};
}