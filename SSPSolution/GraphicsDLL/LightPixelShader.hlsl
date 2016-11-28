Texture2D diffTexture : register(t0);
Texture2D specularTexture : register(t1);
Texture2D normalTexture: register(t2);
Texture2D worldPosTexture: register(t3);

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);

cbuffer LightConstantBuffer
{
	matrix viewMatrix;
	matrix projectionMatrix;

	float4 camPos;
};

struct PSInput
{
	float4 position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
	float4 diffColor;
	
	diffColor = diffTexture.Sample(pointSampler, input.UV);

	return diffColor;
}