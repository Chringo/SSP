
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
    float4x4 ShadowViewMatrix;
    float4x4 ShadowProjectionMatrix;

    int numCasters;
    float spadding1, spadding2, spadding3;
}
StructuredBuffer<PointLight> pointlights : register(t8);
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

static const uint MAX_SHADOWMAP_AMOUNT = 4;


[maxvertexcount(200)]
//Geometry shader!
void SHADOW_GS_main( //GO THROUGH VARIABLES
	triangle VS_OUT input[3],
	inout TriangleStream<GS_OUT> output)
{
	//matrix combinedMatrix = mul(world, mul(view, projection));
    GS_OUT element;
    uint rt_index = 0; //Current shadow map to write to
    float4x4 eachViewMatrix = ShadowViewMatrix;
    for (int eachLight = 0; eachLight < numCasters; eachLight++) // go through all the lights that casts shadows
    {
        if (rt_index < MAX_SHADOWMAP_AMOUNT) //check that we havent reached maximum shadowmaps
        {
			[unroll]
            for (int i = 0; i < 3; i++) //loop through the verts of the face
            {
                element.rtIndex = rt_index;
                eachViewMatrix._44_34_24_14.xyz = pointlights[eachLight].position.xyz;
                matrix combinedMatrix1 = mul(eachViewMatrix, ShadowProjectionMatrix);
                element.position = mul(input[i].position, combinedMatrix1);

                output.Append(element);
            }
            output.RestartStrip();
            rt_index += 1; // add 1 to the shadowmap index.
        }
    }
}

