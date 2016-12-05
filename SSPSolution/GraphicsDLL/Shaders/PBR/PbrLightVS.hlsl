struct VS_IN
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD0;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

//-----------------------------------------------------------------------------------------
// VertexShader
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.Pos = float4(input.Pos, 1);
    output.UV = input.UV;

    return output;
}
