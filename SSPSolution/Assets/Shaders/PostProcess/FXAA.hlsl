Texture2D backBufferTex		: register(t6);
Texture2D normalTex		         : register(t2);
SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);


uniform float WIN_WIDTH = 1280;
uniform float WIN_HEIGHT = 720;

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float4 PS_main(VS_OUT input) : SV_TARGET
{
    float2 uv = float2((input.UV.x / WIN_WIDTH) - 0.5f, (input.UV.y / WIN_HEIGHT) - 0.5);
    
	float4 bbSamp = float4(backBufferTex.Sample(pointSampler, input.UV));
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);


	float4 final = bbSamp;
    return final;
    
}