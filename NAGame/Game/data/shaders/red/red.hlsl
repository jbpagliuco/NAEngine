#include <shader_common_vs.hlsli>
#include <shader_common_ps.hlsli>

// Vertex/Pixel inputs
struct VertexInput : CommonVertexInput
{
};

struct PixelInput : CommonPixelInput
{
};

// Material data
cbuffer cbMaterial : register(CB_REGISTER_MATERIAL)
{
	float4 matDiffuse;
	float4 matSpecular;
};

// Vertex shader
PixelInput vsMain(VertexInput input)
{
	PixelInput output = (PixelInput)0;
	SetCommonPixelInputValues((CommonPixelInput)output, (CommonVertexInput)input);

	return output;
}

// Pixel shader
float4 psMain(PixelInput input) : SV_TARGET
{
	float3 P = input.position;
	float3 N = normalize(input.normal);
	float3 V = normalize(eyePosition - P).xyz;

	LightingResult lit = ComputeFullLighting(matSpecular.w, V, P, N, input.projTexCoord);

	float4 ambient = matDiffuse * globalAmbient;
	float4 diffuse = matDiffuse * lit.diffuse;
	float4 specular = float4(matSpecular.xyz * lit.specular.xyz, 1.0f);

	return ambient + diffuse + specular;
}