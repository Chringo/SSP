SamplerState shaderSampler;

cbuffer CBPerObj
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

	float4 difColor;

	float4 cameraPos;
};

struct PSInput
{
	float4 position : SV_POSITION;
};

struct PSOutput
{
	float4 color : SV_Target0;
	float4 specular : SV_Target2;
	float4 normal : SV_Target3;
	float4 worldPos : SV_Target4;
};

PSOutput main(PSInput input)
{
	PSOutput output;

	output.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	
	output.specular = float4(0.0f, 0.0f, 0.0f, 1.0f);;

	output.normal = float4(0.0f, 0.0f, 0.0f, 1.0f);

	output.worldPos = float4(0.0f, 0.0f, 0.0f, 1.0f);

	return output;
}