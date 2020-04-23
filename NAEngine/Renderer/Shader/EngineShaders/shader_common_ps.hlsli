#if !defined(SHADER_COMMON_PS_HLSLI)
#define SHADER_COMMON_PS_HLSLI

#include <shader_common.hlsli>
#include <light.hlsli>

// Constant Buffers
cbuffer cbLights : register(CB_REGISTER_LIGHTS)
{
	float4 globalAmbient;
	float3 eyePosition;
	int numLights;
	Light lights[MAX_LIGHTS];
};


LightingResult ComputeFullLighting(float specularExp, float3 V, float3 P, float3 N)
{
	LightingResult lit;
	lit.diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	lit.specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < numLights; ++i) {
		LightingResult result = ComputeLighting(lights[i], specularExp, V, P, N);
		lit.diffuse += result.diffuse;
		lit.specular += result.specular;
	}

	lit.diffuse = saturate(lit.diffuse);
	lit.specular = saturate(lit.specular);

	return lit;
}

float3 ComputeNormalFromMap(float4 normalMapTexel, float3 T, float3 B, float3 N)
{
	normalMapTexel = (normalMapTexel * 2.0f) - 1.0f;
	
	float3x3 TBN = float3x3(T, B, N);
	float3 bumpNormal = mul(normalMapTexel, TBN);
	return normalize(bumpNormal);
}

#endif // defined(SHADER_COMMON_PS_HLSLI)