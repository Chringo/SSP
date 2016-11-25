
cbuffer CBPerObj
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

	float4 difColor;

	float4 cameraPos;
};

struct VSInput
{
	float3 position : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 UV : TEXCOORD0;
};

struct GSInput
{
	float4 position : SV_POSITION;
};

GSInput main(VSInput input)
{
	GSInput output;

	//Add homogencoordinates for proper matrix multiplication
	output.position = float4(input.position, 1.0f);
	output.position = mul(worldMatrix, output.position);
	output.position = mul(viewMatrix, output.position);
	output.position = mul(projectionMatrix, output.position);
	//Multiply the position with world-, view- and projectionmatrix
	//Save the world-pos of the vertex
	output.position = output.position;


	return output;
}