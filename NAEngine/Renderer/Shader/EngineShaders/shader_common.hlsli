#if !defined(SHADER_COMMON_HLSLI)
#define SHADER_COMMON_HLSLI

#define MAX_LIGHTS 4

#define CB_REGISTER_CAMERA b0
#define CB_REGISTER_PER_OBJECT b1
#define CB_REGISTER_LIGHTS b2
#define CB_REGISTER_MATERIAL b3

#define TEX_REGISTER_USER0 t0
#define TEX_REGISTER_USER1 t1

#define SAM_REGISTER_USER0 s0
#define SAM_REGISTER_USER1 s1

#if !defined(HLSL_CPP_INCLUDE)
// Constant Buffers
cbuffer cbCamera : register(CB_REGISTER_CAMERA) {
	matrix viewProj;
};

cbuffer cbPerObject : register(CB_REGISTER_PER_OBJECT) {
	matrix world;
	matrix worldInverseTranspose;
};
#endif // !defined(HLSL_CPP_INCLUDE)

#endif // defined(SHADER_COMMON_HLSLI)