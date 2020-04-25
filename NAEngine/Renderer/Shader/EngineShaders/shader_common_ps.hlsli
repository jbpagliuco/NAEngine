#if !defined(SHADER_COMMON_PS_HLSLI)
#define SHADER_COMMON_PS_HLSLI

#include <shader_common.hlsli>

// Constants
static const float SHADOW_EPSILON = 0.001f;
static const float SMAP_SIZE = (float)SHADOWMAP_SIZE;
static const float SMAP_DX = 1.0f / SMAP_SIZE;
static const float SHADOW_BIAS = 0.0f;

// Functions
float ComputeShadowFactor(float4 projTexCoord)
{
	// Complete the projection
	projTexCoord.xyz /= projTexCoord.w;

	// Points outside the light volume are in shadow
	if (projTexCoord.x < -1.0f || projTexCoord.x > 1.0f ||
		projTexCoord.y < -1.0f || projTexCoord.y > 1.0f ||
		projTexCoord.z < 0.0f)
	{
		return 0.0f;
	}

	// Transform from NDC space to texture space
	projTexCoord.x = +0.5f * projTexCoord.x + 0.5f;
	projTexCoord.y = -0.5f * projTexCoord.y + 0.5f;

	// Depth in NDC space
	float depth = projTexCoord.z - SHADOW_BIAS;

	// Sample shadow map to get nearest depth to light
	float s0 = shadowMap.Sample(shadowMapSampler, projTexCoord.xy).r;
	float s1 = shadowMap.Sample(shadowMapSampler, projTexCoord.xy + float2(SMAP_DX, 0.0f)).r;
	float s2 = shadowMap.Sample(shadowMapSampler, projTexCoord.xy + float2(0.0f, SMAP_DX)).r;
	float s3 = shadowMap.Sample(shadowMapSampler, projTexCoord.xy + float2(SMAP_DX, SMAP_DX)).r;

	float result0 = depth <= s0 + SHADOW_EPSILON;
	float result1 = depth <= s1 + SHADOW_EPSILON;
	float result2 = depth <= s2 + SHADOW_EPSILON;
	float result3 = depth <= s3 + SHADOW_EPSILON;

	// Transform to texel space
	float2 texelPos = SMAP_SIZE * projTexCoord.xy;

	// Determine the interpolation amounts
	float2 t = frac(texelPos);

	return lerp(lerp(result0, result1, t.x),
				lerp(result2, result3, t.x), t.y);
}

LightingResult ComputeFullLighting(float specularExp, float3 V, float3 P, float3 N, float4 projTexCoord)
{
	LightingResult lit;
	lit.diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	lit.specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float shadowFactor = ComputeShadowFactor(projTexCoord);

	for (int i = 0; i < numLights; ++i) {
		LightingResult result = ComputeLighting(lights[i], specularExp, V, P, N);
		float4 diffuse = result.diffuse;
		float4 specular = result.specular;

		// Only apply shadows to directional lights, since they're the only lights that can actually cast shadows.
		if (lights[i].type == LIGHT_TYPE_DIRECTIONAL) {
			diffuse *= shadowFactor;
			specular *= shadowFactor;
		}

		lit.diffuse += diffuse;
		lit.specular += specular;
	}

	lit.diffuse = saturate(lit.diffuse);
	lit.specular = saturate(lit.specular);

	return lit;
}

float3 ComputeNormalFromMap(float4 normalMapTexel, float3 T, float3 B, float3 N)
{
	normalMapTexel = (normalMapTexel * 2.0f) - 1.0f;
	
	float3x3 TBN = float3x3(T, B, N);
	float3 bumpNormal = mul(normalMapTexel.xyz, TBN);
	return normalize(bumpNormal);
}

#endif // defined(SHADER_COMMON_PS_HLSLI)