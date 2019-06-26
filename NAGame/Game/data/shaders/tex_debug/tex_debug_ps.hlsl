#include <shader_common_ps.hlsli>

struct PixelInput {
	float4 svpos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(PixelInput input) : SV_TARGET
{
	return float4(input.texCoord.x, input.texCoord.y, 0.0f, 1.0f);
}