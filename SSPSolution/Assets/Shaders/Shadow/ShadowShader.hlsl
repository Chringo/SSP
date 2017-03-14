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

#define MAX_SHADOW_LIGHTS 20  
#define MAX_LIGHT_AMOUNT 60

struct PointLight //Must be 16 bit aligned!
{
    bool isActive;
    float3 _PADDING;
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
    uint DYNAMIC_SHADOWLIGHT_INDEX;
    float3 AMBIENT_COLOR;
    float AMBIENT_INTENSITY;
    int2 SHADOWCASTING_LIGHTS[MAX_SHADOW_LIGHTS]; //Must be multiple of 4

}


cbuffer shadow : register(b5)
{
    float4x4 ShadowViewMatrix[6];
    float4x4 ShadowProjectionMatrix;
}

static const uint MAX_SHADOWMAP_AMOUNT = 1;

//StructuredBuffer<PointLight> pointlights : register(t6);
cbuffer LightArrayBuffer : register(b6)
{
    
    PointLight pointlights[MAX_LIGHT_AMOUNT];

}

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

    [unroll]
    for (int j = 0; j < 6; j++) //render for all 6 axises
    {

         [unroll]
        for (int i = 0; i < 3; i++) //loop through the verts of the face
        {
            element.rtIndex = j; // output to jth depthstencil
         
            matrix combinedMatrix1 = mul(ShadowViewMatrix[j], ShadowProjectionMatrix);
            //element.position.w = input[i].position;
            element.position = mul(input[i].position, combinedMatrix1);
            output.Append(element);
        }
        output.RestartStrip();
    }
}