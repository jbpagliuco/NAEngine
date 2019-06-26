#if !defined(SHADER_COMMON_PS_HLSLI)
#define SHADER_COMMON_PS_HLSLI

#include <shader_common.hlsli>
#include <light.hlsli>

// Constant Buffers
cbuffer cbLights : register(b0)
{
	float4 globalAmbient;
	float3 eyePosition;
	int numLights;
	Light lights[MAX_LIGHTS];
};

cbuffer cbMaterial : register(b1)
{
	float4 color;
};

#endif // defined(SHADER_COMMON_PS_HLSLI)