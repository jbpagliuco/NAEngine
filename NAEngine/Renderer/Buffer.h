#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

namespace na
{
	enum class BufferUsage
	{
		DEFAULT,
		IMMUTABLE,
		DYNAMIC,
		STAGING
	};

	class Buffer
	{
	};
}