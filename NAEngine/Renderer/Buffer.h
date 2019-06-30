#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

namespace na
{
	enum class BufferUsage
	{
#if defined(NA_D3D11)
		DEFAULT = D3D11_USAGE_DEFAULT,
		IMMUTABLE = D3D11_USAGE_IMMUTABLE,
		DYNAMIC = D3D11_USAGE_DYNAMIC,
		STAGING = D3D11_USAGE_STAGING
#endif
	};

	class Buffer
	{
	};
}