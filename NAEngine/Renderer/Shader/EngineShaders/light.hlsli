#if !defined(LIGHT_HLSLI)
#define LIGHT_HLSLI

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

struct Light
{
	int type;
	bool enabled;
	float intensity;
	int _padding0;

	float3 position;
	float _padding1;

	float3 direction;
	float radius;

	float4 color;

	float spotLightAngle;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

#endif // defined(LIGHT_HLSLI)