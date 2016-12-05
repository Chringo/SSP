
cbuffer worldMatrix : register(b0)
{
    matrix worldMatrix;
}

cbuffer viewProj : register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;
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