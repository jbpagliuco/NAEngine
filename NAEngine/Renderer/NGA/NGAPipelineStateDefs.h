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
		CULL_NONE = D3D11_CULL_NONE,
		CULL_BACK = D3D11_CULL_BACK,
		CULL_FRONT = D3D11_CULL_FRONT
#endif
	};

	enum class NGAFillMode
	{
#if defined(NGA_D3D11)
		SOLID = D3D11_FILL_SOLID
#endif
	};

	enum class NGADepthFunc
	{
#if defined(NGA_D3D11)
		NEVER = D3D11_COMPARISON_NEVER,
		ALWAYS = D3D11_COMPARISON_ALWAYS,
		EQUAL = D3D11_COMPARISON_EQUAL,
		NOT_EQUAL = D3D11_COMPARISON_NOT_EQUAL,
		LESS = D3D11_COMPARISON_LESS,
		LESS_EQUAL = D3D11_COMPARISON_LESS_EQUAL,
		GREATER = D3D11_COMPARISON_GREATER,
		GREATER_EQUAL = D3D11_COMPARISON_GREATER_EQUAL,
#endif
	};

	enum class NGAIndexBufferType
	{
		IBT_16BIT,
		IBT_32BIT
	};
}