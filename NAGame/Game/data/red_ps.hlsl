struct PixelInput {
	float4 svpos : SV_POSITION;
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
	return float4(input.position, 1.0f);
}