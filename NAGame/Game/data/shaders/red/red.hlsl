#include <shader_common_vs.hlsli>
#include <shader_common_ps.hlsli>

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

cbuffer cbMaterial : register(CB_REGISTER_MATERIAL)
{
	float4 matDiffuse;
	float4 matSpecular;
};

PixelInput vsMain(VertexInput input)
{
	float4 pos = float4(input.position, 1.0f);

	PixelInput output;

	output.svpos = mul(mul(viewProj, world), pos);
	output.position = mul(world, pos);

	float4 normal = float4(normalize(input.normal), 0.0f);
	output.normal = mul(normal, worldInverseTranspose).xyz;

	output.texCoord = input.texCoord;

	return output;
}

float4 psMain(PixelInput input) : SV_TARGET
{
	float3 P = input.position;
	float3 N = normalize(input.normal);
	float3 V = normalize(eyePosition - P).xyz;

	LightingResult lit = ComputeFullLighting(matSpecular.w, V, P, N);

	float4 ambient = matDiffuse * globalAmbient;
	float4 diffuse = matDiffuse * lit.diffuse;
	float4 specular = float4(matSpecular.xyz * lit.specular.xyz, 1.0f);

	return ambient + diffuse + specular;
}