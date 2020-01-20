#pragma once

#include "NGADefs.h"

#if defined(NGA_D3D11)
#include <d3d11.h>
#endif

namespace na
{
	enum class NGAPrimitiveTopology
	{
#if defined(NGA_D3D11)
		TRIANGLE_LIST = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
#endif
	};
	enum class NGACullMode
	{
#if defined(NGA_D3D11)
		CULL_BACK = D3D11_CULL_BACK
#endif
	};

	enum class NGAFillMode
	{
#if defined(NGA_D3D11)
		SOLID = D3D11_FILL_SOLID
#endif
	};

	enum class NGAIndexBufferType
	{
		IBT_16BIT,
		IBT_32BIT
	};
}