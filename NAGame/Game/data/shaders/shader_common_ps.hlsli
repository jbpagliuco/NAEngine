#if !defined(SHADER_COMMON_PS_HLSLI)
#define SHADER_COMMON_PS_HLSLI

#include <shader_common.hlsli>

cbuffer cbMaterial : register(b0)
{
	float4 color;
};

#endif // defined(SHADER_COMMON_PS_HLSLI)