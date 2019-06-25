#include "../shader_common.h"
#include "../shader_common_ps.h"

struct PixelInput {
	float4 svpos : SV_POSITION;
};

float4 main(PixelInput input) : SV_TARGET
{
	return color;
}