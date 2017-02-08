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


SHADOW_VS_OUT VS_main(VS_IN input)
{
    SHADOW_VS_OUT output = (SHADOW_VS_OUT) 0;
    output.position = float4(input.Pos, 1.0f);

    return output;
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
cbuffer LightInfo : register(b3)
{
    uint NUM_POINTLIGHTS;
    uint NUM_AREALIGHTS;
    uint NUM_DIRECTIONALLIGHTS;
    uint NUM_SPOTLIGHTS;
    float3 AMBIENT_COLOR;
    float AMBIENT_INTENSITY;
}

static const uint MAX_SHADOWMAP_AMOUNT = 1;

StructuredBuffer<PointLight> pointlights : register(t8);


[maxvertexcount(200)]
//Geometry shader!
void GS_main( 
	triangle SHADOW_VS_OUT input[3],
	inout TriangleStream<SHADOW_GS_OUT> output)
{
	//matrix combinedMatrix = mul(world, mul(view, projection));
    SHADOW_GS_OUT element;
    uint rt_index = 0; //Current shadow map to write to

   //for (int d = 0; d < numDirLights; d++) // go through all the direction lights
   //{
   //
   //    if (dirLights[d].castShadow != 0) //if the light casts shadows
   //    {
   //        if (rt_index < MAX_SHADOWMAP_AMOUNT) //check that we havent reached maximum shadowmaps
   //        {
   //
	//				[unroll]
   //            for (int i = 0; i < 3; i++) //loop through the verts of the face
   //            {
   //                element.rtIndex = rt_index;
   //                matrix combinedMatrix = mul(world, mul(dirLights[d].lightView, dirLights[d].lightProjection));
   //                element.position = mul(input[i].position, combinedMatrix);
   //
   //                output.Append(element);
   //            }
   //            output.RestartStrip();
   //            rt_index += 1; // add 1 to the shadowmap index.
   //        }
   //    }
	//
   //}
    //for (int i = 0; i < numPointLights; i++)
    //{
    //    if (pointlights[i].castShadow != 0)
    //    {
    //
    //
    //        if (rt_index < MAX_SHADOWMAP_AMOUNT) //check that we havent reached maximum shadowmaps
    //        {
    //
	//				[unroll]
    //            for (int j = 0; j < 3; j++) //loop through the verts of the face
    //            {
    //                element.rtIndex = rt_index;
    //                matrix combinedMatrix = mul(world, mul(pointlights[i].lightView, pointlights[i].lightProjection));
    //                element.position = mul(input[j].position, combinedMatrix);
    //
    //                output.Append(element);
    //            }
    //            output.RestartStrip();
    //            rt_index += 1; // add 1 to the shadowmap index.
    //        }
    //    }
    //}
}