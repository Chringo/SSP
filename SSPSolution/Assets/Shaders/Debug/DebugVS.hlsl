

cbuffer camera : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;

    float4 camPos;
    float timer, p1, p2, p3;
}

struct VS_IN
{
    float3 Pos : POSITION;
    float3 Color : COLOR;
    
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR;

};


//-----------------------------------------------------------------------------------------
// VERTEX SHADER
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    matrix WV = mul(viewMatrix, projectionMatrix);
 

    output.Pos = float4(input.Pos, 1);
   
    output.Pos = mul(output.Pos, WV);
    output.Color = input.Color;
    return output;
}