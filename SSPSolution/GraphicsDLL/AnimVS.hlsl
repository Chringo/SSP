
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

    float4 weights : WEIGHTS;
    uint4 influences : INFLUENCE; //currently a 32bit uinteger, does it need to be 32bit?
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
    VS_OUT output = (VS_OUT) 0;
    //wPos, Normal and Tangent should be output in worldspace. Make it so.

    //INSERT VERTEX BLENDING
    matrix WV = mul(viewMatrix, projectionMatrix);
    matrix WVP = mul(worldMatrix, WV);

    output.Pos = float4(input.Pos, 1);
    output.wPos = mul(output.Pos, worldMatrix);
    output.Pos = mul(output.Pos, WVP);

    output.Normal = mul(float4(input.Normal, 1), worldMatrix).rgb;
    output.UV = input.UV;
    output.Tangent = mul(float4(input.Tangent, 1), worldMatrix).rgb;

    

	return output;
}