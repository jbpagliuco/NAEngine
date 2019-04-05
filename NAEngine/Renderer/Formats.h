#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

namespace na
{
	enum class Format
	{
#if defined(NA_D3D11)
		R32_FLOAT					= DXGI_FORMAT_R32_FLOAT,
		R32G32_FLOAT				= DXGI_FORMAT_R32G32_FLOAT,
		R32G32B32_FLOAT				= DXGI_FORMAT_R32G32B32_FLOAT,
		R32G32B32A32_FLOAT			= DXGI_FORMAT_R32G32B32A32_FLOAT
#endif
	};

}