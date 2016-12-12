

cbuffer camera : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;

    float4 camPos;
    float4 padding1;
    float4 padding2;
    float4 padding3;
}

struct VS_IN
{
    float3 Pos : POSITION;
    
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;


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
   
    output.Pos = mul(output.Pos, WV);

    return output;
}