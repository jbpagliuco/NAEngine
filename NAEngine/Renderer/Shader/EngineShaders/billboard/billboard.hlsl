#include <shader_common_vs.hlsli>
#include <shader_common_ps.hlsli>

cbuffer cbFixed : register(CB_REGISTER_USER0)
{
	float2 quadTexCoords[4] = 
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
};



// Stage inputs
struct VertexInput
{
	float3 position : POSITION0;
};

struct GeometryInput
{
	float3 position : POSITION0;
};

struct PixelInput
{
	float4 svpos : SV_POSITION;
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

// Resources
Texture2D BillboardTexture : register(TEX_REGISTER_USER0);
SamplerState Sampler : register(SAM_REGISTER_USER0);



// Vertex shader
GeometryInput vsMain(VertexInput input)
{
	GeometryInput output;
	output.position = mul(world, float4(input.position, 1.0f)).xyz;

	return output;
}

// Geometry shader
[maxvertexcount(4)]
void gsMain(point GeometryInput input[1], inout TriangleStream<PixelInput> outputStream)
{
	float2 size = float2(8, 8);

	float2 quadTexCoords[4] = 
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};



	float halfWidth  = 0.5f * size.x;

	// Create coordinate system facing the camera
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 forward = eyePosition - input[0].position;
	forward.y = 0.0f;
	forward = normalize(forward);
	float3 right = cross(forward, up);

	right *= halfWidth;
	up *= size.y;

	// Build vertices
	float4 v[4];
	v[0] = float4(input[0].position - right, 1.0f);
	v[1] = float4(input[0].position - right + up, 1.0f);
	v[2] = float4(input[0].position + right, 1.0f);
	v[3] = float4(input[0].position + right + up, 1.0f);

	// Append vertices
	PixelInput output;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		output.svpos	= mul(viewProj, v[i]);
		output.position = v[i].xyz;
		output.normal	= forward;
		output.texCoord = quadTexCoords[i];

		outputStream.Append(output);
	}
}

// Pixel shader
float4 psMain(PixelInput input) : SV_Target
{
	float4 texColor = BillboardTexture.Sample(Sampler, input.texCoord);

	// TODO: Apply lighting
	return texColor;
}