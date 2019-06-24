#if !defined(SHADER_COMMON_PS_H)
#define SHADER_COMMON_PS_H

cbuffer MaterialBuffer : register(b0)
{
	float4 color;
};

#endif // defined(SHADER_COMMON_PS_H)