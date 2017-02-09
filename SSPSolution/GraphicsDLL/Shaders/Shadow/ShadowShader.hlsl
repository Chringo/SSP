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

struct VS_IN
{
    float3 Pos      : POSITION;
    float3 Normal   : NORMAL;
    float3 Tangent  : TANGENT0;
    float2 UV       : TEXCOORD0;
};

struct SHADOW_VS_OUT
{
    float4 position : SV_POSITION;
};
struct SHADOW_GS_OUT
{
    float4 position : SV_POSITION;
    uint rtIndex : SV_RenderTargetArrayIndex;
};




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
cbuffer LightInfo : register(b3)
{
    uint   NUM_POINTLIGHTS;
    uint   NUM_AREALIGHTS;
    uint   NUM_DIRECTIONALLIGHTS;
    uint   NUM_SPOTLIGHTS;
    float3 AMBIENT_COLOR;
    float  AMBIENT_INTENSITY;
}
cbuffer shadow : register(b5)
{
    float4x4 ShadowViewMatrix;
    float4x4 ShadowProjectionMatrix;

    int numCasters;
    float spadding1, spadding2, spadding3;
}

static const uint MAX_SHADOWMAP_AMOUNT = 1;

StructuredBuffer<PointLight> pointlights : register(t6);

SHADOW_VS_OUT VS_main(VS_IN input)
{
    SHADOW_VS_OUT output = (SHADOW_VS_OUT) 0;
    output.position = mul(float4(input.Pos, 1), worldMatrix);

    return output;
}

[maxvertexcount(200)]
//Geometry shader!
void GS_main( 
	triangle SHADOW_VS_OUT input[3],
	inout TriangleStream<SHADOW_GS_OUT> output)
{
	//matrix combinedMatrix = mul(world, mul(view, projection));
    SHADOW_GS_OUT element;
    uint rt_index = 0; //Current shadow map to write to
    float4x4 eachViewMatrix = ShadowViewMatrix;
   // for (int eachLight = 0; eachLight < 1; eachLight++) // go through all the lights that casts shadows
   // {
   //     if (rt_index < MAX_SHADOWMAP_AMOUNT) //check that we havent reached maximum shadowmaps
   //     {
	//		
   //         rt_index += 1; // add 1 to the shadowmap index.
   //     }
   // }

    [unroll]
    for (int i = 0; i < 3; i++) //loop through the verts of the face
    {
        element.rtIndex = 0;
       // eachViewMatrix._44_34_24_14.xyz = pointlights[eachLight].position.xyz;
        matrix combinedMatrix1 = mul(viewMatrix, projectionMatrix);
        element.position = mul(input[i].position, combinedMatrix1);
        element.position.w = 1.0f;
        output.Append(element);
    }
    output.RestartStrip();
}