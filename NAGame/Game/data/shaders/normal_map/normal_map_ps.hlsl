#include <shader_common_ps.hlsli>

struct PixelInput {
	float4 svpos : SV_POSITION;
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
	float3 tangent : TANGENT0;
};

cbuffer cbMaterial : register(PS_CB_USER_REGISTER)
{
	float4 matDiffuse;
	float4 matSpecular;
};

Texture2D DiffuseTexture;
SamplerState DiffuseSampler;

Texture2D NormalTexture;
SamplerState NormalSampler;

float4 main(PixelInput input) : SV_TARGET
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