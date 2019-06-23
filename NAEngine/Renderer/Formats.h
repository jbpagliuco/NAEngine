#pragma once

#include "RenderDefs.h"

#include <string>

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

namespace na
{
	enum class Format
	{
		R32_FLOAT = 0,
		R32G32_FLOAT,
		R32G32B32_FLOAT,
		R32G32B32A32_FLOAT
	};

	Format GetFormatFromString(const std::string &s);
	size_t GetFormatByteSize(Format format);

#if defined(NA_D3D11)
	DXGI_FORMAT GetDXGIFormat(Format format);
#endif
}