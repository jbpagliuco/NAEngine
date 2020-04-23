#if !defined(SHADER_COMMON_HLSLI)
#define SHADER_COMMON_HLSLI

#define MAX_LIGHTS 4

#define MATERIAL_CB_REGISTER b2

#if !defined(HLSL_CPP_INCLUDE)
// Constant Buffers
cbuffer cbCamera : register(b0) {
	matrix viewProj;
};

cbuffer cbPerObject : register(b1) {
	matrix world;
	matrix worldInverseTranspose;
};
#endif // !defined(HLSL_CPP_INCLUDE)

#endif // defined(SHADER_COMMON_HLSLI)