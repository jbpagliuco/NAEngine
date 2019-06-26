#include <shader_common_vs.hlsli>

// Vertex/Pixel inputs
struct VertexInput {
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct PixelInput {
	float4 svpos : SV_POSITION;
};

PixelInput main(VertexInput input)
{
	float4 pos = float4(input.position, 1.0f);

	PixelInput output;
	output.svpos = mul(pos, mul(world, viewProj));

	return output;
}

//PixelInput output;
//
//float4 pos = float4(input.position, 1.0f);
//
//output.svpos = mul(pos, mul(world, viewProj));
//output.position = mul(pos, world);
//
//float4 normal = float4(normalize(input.normal), 0.0f);
//output.normal = mul(worldInverseTranspose, normal).xyz;
//
//output.texCoord = input.texCoord;
//
//return output;