cbuffer cbPerFrame : register(b0)
{
	matrix view;
	matrix viewProj;
}

cbuffer cbPerObject : register(b1)
{
	matrix world;
	matrix worldInverseTranspose;
};

// Vertex/Pixel inputs
struct VertexInput
{
	float3 positionL : POSITION0;
	float3 normalL : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct PixelInput
{
	float4 positionH : SV_POSITION;
	float3 positionV : POSITION0;
	float3 normalV : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

// Vertex shader
PixelInput vsMain(VertexInput input)
{
	PixelInput output;

	float4 positionL = float4(input.positionL, 1.0f);

	// Transform to view space
	output.positionV = mul(mul(view, world), positionL).xyz;

	float3x3 worldInvTransposeView = mul(view, worldInverseTranspose);
	output.normalV = mul(worldInvTransposeView, input.normalL);

	// Transform to homogeneous clip space
	output.positionH = mul(mul(viewProj, world), positionL);

	// Copy texture coordinates
	output.texCoord = input.texCoord;

	return output;
}

// Pixel shader
float4 psMain(PixelInput input) : SV_Target
{
	// Re-normalize normal vector
	float3 normalV = normalize(input.normalV);

	return float4(normalV, input.positionV.z);
}