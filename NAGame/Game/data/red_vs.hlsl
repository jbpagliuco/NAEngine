// Constant Buffers
cbuffer cbCamera : register(b0) {
	matrix viewProj;
};

cbuffer cbPerObject : register(b1) {
	matrix world;
};

// Vertex/Pixel inputs
struct VertexInput {
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct PixelInput {
	float4 svpos : SV_POSITION;
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

PixelInput main(VertexInput input)
{
	PixelInput output;

	float4 pos = float4(input.position, 1.0f);

	output.svpos = mul(pos, mul(world, viewProj));
	output.position = mul(pos, world);

	//float4 normal = float4(normalize(input.normal), 0.0f);
	//output.normal = mul(worldInverseTranspose, normal).xyz;
	output.normal = float3(0.0f, 0.0f, 0.0f);

	output.texCoord = input.texCoord;

	return output;
}