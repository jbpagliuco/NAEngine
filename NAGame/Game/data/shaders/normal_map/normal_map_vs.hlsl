#include <shader_common_vs.hlsli>

// Vertex/Pixel inputs
struct VertexInput {
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
	float2 texCoord : TEXCOORD0;
	float3 tangent : TANGENT0;
};

struct PixelInput {
	float4 svpos : SV_POSITION;
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
	float3 tangent : TANGENT0;
};

PixelInput main(VertexInput input)
{
	float4 pos = float4(input.position, 1.0f);

	PixelInput output;

	output.svpos = mul(mul(viewProj, world), pos);
	output.position = mul(world, pos);

	float4 normal = float4(normalize(input.normal), 0.0f);
	output.normal = mul(normal, worldInverseTranspose).xyz;

	float4 tangent = float4(normalize(input.tangent), 0.0f);
	output.tangent = mul(worldInverseTranspose, tangent).xyz;

	output.texCoord = input.texCoord;
	
	return output;
}