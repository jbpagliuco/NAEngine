#if !defined(MATERIAL_HLSLI)
#define MATERIAL_HLSLI

struct MaterialProperties
{
	float4 ambient;
	float4 diffuse;

	// (r, g, b, exp)
	float4 specular;
};


#endif // !defined(MATERIAL_HLSLI)