#if !defined(SHADER_COMMON_HLSLI)
#define SHADER_COMMON_HLSLI

#define MAX_LIGHTS 4

#define CB_REGISTER_PER_FRAME b0
#define CB_REGISTER_PER_OBJECT b1
#define CB_REGISTER_LIGHTS b2
#define CB_REGISTER_MATERIAL b3

#define TEX_REGISTER_SHADOWMAP t0
#define TEX_REGISTER_USER0 t1
#define TEX_REGISTER_USER1 t2

#define SAM_REGISTER_SHADOWMAP s0
#define SAM_REGISTER_USER0 s1
#define SAM_REGISTER_USER1 s2

#define SHADOWMAP_SIZE 1024

#if !defined(HLSL_CPP_INCLUDE)

#include <light.hlsli>

// Constant Buffers
cbuffer cbCamera : register(CB_REGISTER_PER_FRAME)
{
	matrix viewProj;
	matrix lightViewProj;
};

cbuffer cbPerObject : register(CB_REGISTER_PER_OBJECT)
{
	matrix world;
	matrix worldInverseTranspose;
};

cbuffer cbLights : register(CB_REGISTER_LIGHTS)
{
	float4 globalAmbient;
	float3 eyePosition;
	int numLights;
	Light lights[MAX_LIGHTS];
};

Texture2D shadowMap : register(TEX_REGISTER_SHADOWMAP);
SamplerState shadowMapSampler : register(SAM_REGISTER_SHADOWMAP);

struct CommonVertexInput
{
	float3 position : POSITION0;
	float3 normal   : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct CommonPixelInput
{
	float4 svpos : SV_POSITION;
	float3 position : POSITION0;
	
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;

	float2 texCoord : TEXCOORD0;
	float4 projTexCoord : TEXCOORD1;
};

#endif // !defined(HLSL_CPP_INCLUDE)

#endif // defined(SHADER_COMMON_HLSLI)