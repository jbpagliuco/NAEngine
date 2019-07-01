#include <shader_common_ps.hlsli>

struct PixelInput {
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL0;
};

float4 main(PixelInput input) : SV_TARGET
{
	return float4(normalize(input.normal), 1.0f);
}