SamplerState shaderSampler;

Texture2D diffTexture : register(t0);
Texture2D specularTexture : register(t1);
Texture2D normalTexture: register(t2);
Texture2D worldPosTexture: register(t3);


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
	float2 UV : TEXCOORD0;
};

struct PSOutput
{
	float4 color : SV_Target0;
	float4 specular : SV_Target1;
	float4 normal : SV_Target2;
	float4 worldPos : SV_Target3;
};

PSOutput main(PSInput input)
{
	PSOutput output;

	output.color = diffTexture.Sample(shaderSampler, input.UV); //float4(0.5f, 0.5f, 0.5f, 1.0f);
	
	output.specular = float4(1.0f, 0.0f, 0.0f, 1.0f);;

	output.normal = float4(0.0f, 0.0f, -1.0f, 1.0f);

	output.worldPos = float4(0.0f, 0.0f, 0.0f, 1.0f);

	return output;
}