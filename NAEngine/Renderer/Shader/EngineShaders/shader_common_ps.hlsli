#if !defined(SHADER_COMMON_PS_HLSLI)
#define SHADER_COMMON_PS_HLSLI

#include <shader_common.hlsli>
#include <material.hlsli>
#include <light.hlsli>

// Constant Buffers
cbuffer cbLights : register(b0)
{
	float4 globalAmbient;
	float3 eyePosition;
	int numLights;
	Light lights[MAX_LIGHTS];
};

cbuffer cbMaterial : register(b1)
{
	MaterialProperties material;
};


LightingResult ComputeFullLighting(MaterialProperties material, float3 V, float3 P, float3 N)
{
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

	return lit;
}

#endif // defined(SHADER_COMMON_PS_HLSLI)