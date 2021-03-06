
cbuffer camera : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;

    float4 camPos;
    float timer,
    padding1, padding2, padding3;

}


struct VS_IN
    {
    float3 Pos           : POSITION;
    float3 Normal        : NORMAL;
    float3 Tangent       : TANGENT0;
    float2 UV            : TEXCOORD0;

    float4x4 worldMatrix : WORLD;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT0;
    float2 UV : TEXCOORD0;

    float4 wPos : WORLDPOSITION;
};




VS_OUT VS_main(VS_IN input)
{
    VS_OUT output;


    output.Pos   = mul(float4(input.Pos, 1.0f), input.worldMatrix);
    output.wPos = output.Pos;
    output.Pos   = mul(output.Pos, viewMatrix);
    output.Pos   = mul(output.Pos, projectionMatrix);

    output.Normal = mul(float4(input.Normal, 0.0f), input.worldMatrix);
    output.Tangent = mul(float4(input.Tangent, 0.0f), input.worldMatrix);
  
    output.UV = input.UV;

    return output;
}

