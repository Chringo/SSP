cbuffer LightConstantBuffer
{
	matrix viewMatrix;
	matrix projectionMatrix;

	float2 resolution;

	float4 camPos;
};

struct VSInput
{
	float4 position : POSITION;
};

struct PSInput
{
	float4 position : SV_POSITION;
};

PSInput main(VSInput input)
{
	PSInput output;

	//Add homogencoordinates for proper matrix multiplication
	input.position.w = 1.0f;

	//Multiply the position with world-, view- and projectionmatrix
	output.position = mul(viewMatrix, input.position);
	output.position = mul(projectionMatrix, output.position);

	//output will be in clip space of the light projection plane
	return output;
}