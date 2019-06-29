#include <shader_common_ps.hlsli>

struct PixelInput {
	float4 svpos : SV_POSITION;
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

float4 main(PixelInput input) : SV_TARGET
{
	float3 P = input.position;
	float3 N = normalize(input.normal);
	float3 V = normalize(eyePosition - P).xyz;

	LightingResult lit;
	lit.diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	lit.specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < numLights; ++i) {
		LightingResult result = ComputeLighting(lights[i], material, V, P, N);
		lit.diffuse += result.diffuse;
		lit.specular += result.specular;
	}

	lit.diffuse = saturate(lit.diffuse);
	lit.specular = saturate(lit.specular);

	float4 ambient = material.ambient * globalAmbient;
	float4 diffuse = material.diffuse * lit.diffuse;
	float4 specular = float4(material.specular.xyz * lit.specular.xyz, 1.0f);

	return ambient + diffuse + specular;
}