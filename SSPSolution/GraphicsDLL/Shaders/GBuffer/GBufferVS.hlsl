cbuffer worldMatrix : register(b0)
{
    float4x4 worldMatrix;
}

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
    float3 Pos      : POSITION;
    float3 Normal   : NORMAL;
    float3 Tangent  : TANGENT0;
    float2 UV       : TEXCOORD0;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT0;
    float2 UV : TEXCOORD0;

    float4 wPos : WORLDPOSITION;
};


//-----------------------------------------------------------------------------------------
// VERTEX SHADER
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    matrix WV = mul(viewMatrix, projectionMatrix);
    matrix WVP = mul(worldMatrix, WV);

    output.Pos = float4(input.Pos, 1);
    output.wPos = mul(output.Pos, worldMatrix);
    output.Pos = mul(output.Pos, WVP);

    //output.Pos = mul(float4(input.Pos, 1), projectionMatrix); //mul(float4(input.Pos, 1), WVP);
    output.Normal = mul(float4(input.Normal, 0.0f), worldMatrix).rgb;
	//output.Normal = input.Normal.rgb;
    output.UV = input.UV;
    output.Tangent = mul(float4(input.Tangent, 0.0f), worldMatrix).rgb;
    //output.wPos = mul(float4(input.Pos, 1), worldMatrix);

    return output;
}