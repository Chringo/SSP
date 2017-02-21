cbuffer worldMatrix : register(b0)
{
    float4x4 worldMatrix;
}

cbuffer frame : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;

    float4 camPos;
    float timer,
    padding1, padding2, padding3;
}
cbuffer skeleton : register(b4)
{
    float4x4 joints[22];
}


struct VS_IN
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT0;
    float2 UV : TEXCOORD0;

    float4 weights : WEIGHTS;
    int4 influences : INFLUENCE; //currently a 32bit uinteger, does it need to be 32bit?
};

struct VS_OUT
{
    float4 position : SV_POSITION;
};

VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
	
    float3 skinnedPos = float3(0.f, 0.f, 0.f);
    float3 skinnedNormal = float3(0.f, 0.f, 0.f);
    float3 skinnedTan = float3(0.f, 0.f, 0.f);
    float weight;
    int influences;
	/*Vertex blending is performed here. With the following: weights, influences and the matrix of each joint.*/
    for (int i = 0; i < 4; i++)
    {
        influences = input.influences[i];

        if (influences != -1)
        {
            weight = input.weights[i];

            skinnedPos    += mul(weight, mul(float4(input.Pos, 1.0f),     joints[influences])).xyz;
            skinnedNormal += mul(weight, mul(float4(input.Normal, 0.0f),  joints[influences])).xyz;
            skinnedTan    += mul(weight, mul(float4(input.Tangent, 0.0f), joints[influences])).xyz;
        }
    }

   //matrix WV = mul(viewMatrix, projectionMatrix);
   //matrix WVP = mul(worldMatrix, WV);

    output.position = mul(float4(skinnedPos, 1), worldMatrix);
  
    return output;
}