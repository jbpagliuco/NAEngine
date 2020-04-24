cbuffer cbPerFrame : register(b0)
{
	matrix lightWVP;
};

cbuffer cbPerObject : register(b1)
{
	matrix world;
	matrix world1;
	matrix world2;
	matrix world31;
	matrix world41;
	matrix world51;
	matrix world71;
};

// Vertex/Pixel inputs
struct VertexInput
{
	float3 position : POSITION0;
};

struct PixelInput
{
	float4 svpos : SV_POSITION;
};

// Vertex shader
PixelInput vsMain(VertexInput input)
{
	PixelInput output;

	output.svpos = mul(mul(lightWVP, world), float4(input.position, 1.0f));

	return output;
}

// Pixel shader
void psMain(PixelInput input)
{
}