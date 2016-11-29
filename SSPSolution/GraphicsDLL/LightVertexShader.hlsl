cbuffer LightConstantBuffer
{
	matrix viewMatrix;
	matrix projectionMatrix;

	float2 resolution;

	float4 camPos;
};

struct VSInput
{
	float3 position : POSITION;
    float2 UV : TEXCOORD0;
};

struct PSInput
{
	float4 position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

PSInput main(VSInput input)
{
	PSInput output;


	//Multiply the position with world-, view- and projectionmatrix
	output.position = float4(input.position, 1.0f);
    output.UV = input.UV;
	//output will be in clip space of the light projection plane
	return output;
}