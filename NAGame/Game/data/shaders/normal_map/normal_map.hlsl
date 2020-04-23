#include <shader_common_vs.hlsli>
#include <shader_common_ps.hlsli>

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

// Material data
cbuffer cbMaterial : register(CB_REGISTER_MATERIAL)
{
	float4 matDiffuse;
	float4 matSpecular;
};

// Resources
Texture2D DiffuseTexture : register(TEX_REGISTER_USER0);
SamplerState DiffuseSampler : register(SAM_REGISTER_USER0);

Texture2D NormalTexture : register(TEX_REGISTER_USER1);
SamplerState NormalSampler : register(SAM_REGISTER_USER1);

PixelInput vsMain(VertexInput input)
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

float4 psMain(PixelInput input) : SV_TARGET
{
	float2 texCoord = float2(1.0f, 1.0f) - input.texCoord;

	float3 N = normalize(input.normal);
	float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
	float3 B = cross(N, T);
	float3 normal = ComputeNormalFromMap(NormalTexture.Sample(NormalSampler, texCoord), T, B, N);

	float3 P = input.position;
	float3 V = normalize(eyePosition - P).xyz;

	LightingResult lit = ComputeFullLighting(matSpecular, V, P, normal);

	float4 texColor = DiffuseTexture.Sample(DiffuseSampler, texCoord);

	float4 ambient = texColor * matDiffuse * globalAmbient;
	float4 diffuse = texColor * matDiffuse * lit.diffuse;
	float4 specular = float4(matSpecular.xyz * lit.specular.xyz, 1.0f);

	return ambient + diffuse + specular;
}