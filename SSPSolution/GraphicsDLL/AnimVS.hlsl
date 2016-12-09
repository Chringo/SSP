
cbuffer worldMatrix : register(b0)
{
    float4x4 worldMatrix;
}

cbuffer camera : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;

    float4 camPos;
    float4 padding1;
    float4 padding2;
    float4 padding3;
}
cbuffer skeleton : register(b5)
{
    float4x4 joint[32];
}


struct VS_IN
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT0;
    float2 UV : TEXCOORD0;

    float4 weights : WEIGHT;
    int4 influences : INFLUENCE;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT0;
    float2 UV : TEXCOORD0;

    float4 wPos : WORLDPOSITION;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    //INSERT VERTEX BLENDING

	return output;
}