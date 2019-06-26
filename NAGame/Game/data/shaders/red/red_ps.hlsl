#include <shader_common_ps.hlsli>

struct PixelInput {
	float4 svpos : SV_POSITION;
};

float4 main(PixelInput input) : SV_TARGET
{
	return color;
}