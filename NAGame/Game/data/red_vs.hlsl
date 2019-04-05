struct VertexInput {
	float3 position : POSITION0;
};

struct PixelInput {
	float4 svpos : SV_POSITION;
};

PixelInput main(VertexInput input)
{
	PixelInput output;
	
	output.svpos = float4(input.position, 1.0f);

	return output;
}