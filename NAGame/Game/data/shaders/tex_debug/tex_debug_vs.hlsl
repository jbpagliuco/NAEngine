#include "shader_common.hlsl"

// Vertex/Pixel inputs
struct VertexInput {
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct PixelInput {
	float4 svpos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

PixelInput main(VertexInput input)
{
	float4 pos = float4(input.position, 1.0f);
	
	PixelInput output;
	output.svpos = mul(pos, mul(world, viewProj));
	output.texCoord = input.texCoord;

	return output;
}