#include <shader_common_ps.hlsli>

struct PixelInput {
	float4 svpos : SV_POSITION;
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

float4 main(PixelInput input) : SV_TARGET
{
	return float4(input.normal, 1.0f);
}