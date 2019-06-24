#if !defined(SHADER_COMMON_VS_H)
#define SHADER_COMMON_VS_H

// Constant Buffers
cbuffer cbCamera : register(b0) {
	matrix viewProj;
};

cbuffer cbPerObject : register(b1) {
	matrix world;
	matrix worldInverseTranspose;
};

#endif // defined(SHADER_COMMON_VS_H)