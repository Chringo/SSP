
cbuffer camera : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;

    float4 camPos;
    float timer,
    padding1, padding2, padding3;

}
struct PointLight //Must be 16 bit aligned!
{
    bool isActive;
    float3 isActivePADDING;
    float3 color;
    float intensity;
    float4 position;
    float radius;
    float constantFalloff;
    float linearFalloff;
    float quadraticFalloff;
};
cbuffer shadow : register(b5)
{
    float4x4 ShadowViewMatrix[6];
    float4x4 ShadowProjectionMatrix;
}
StructuredBuffer<PointLight> pointlights : register(t6);
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
    float4 position : SV_POSITION;
};
struct GS_OUT
{
    float4 position : SV_POSITION;
    uint rtIndex    : SV_RenderTargetArrayIndex;
};

VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.position = mul(float4(input.Pos, 1), input.worldMatrix);

    return output;
}



