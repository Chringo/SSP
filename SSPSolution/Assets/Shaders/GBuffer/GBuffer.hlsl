Texture2D albedoTex		 : register(t0);
Texture2D metalTex		 : register(t1);
Texture2D roughTex		 : register(t2);
Texture2D normalTex		 : register(t3);
Texture2D aoTex			 : register(t4);

SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);

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

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT0;
    float2 UV : TEXCOORD0;

    float4 wPos : WORLDPOSITION;
};

struct GS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : TEXCOORD1;
    float2 UV : TEXCOORD0;
    
    float4 wPos : WORLDPOSITION;
    float3x3 TBN : TBN;
};

struct PS_OUT
{
    float3 color : SV_Target0;
    float3 metalRoughAo : SV_Target1;
    float3 normal : SV_Target2;
    float4 wPosition : SV_Target3;
};

float3 normalToWorldSpace(float3 normalMapSample, float3 normal, float3 tangent) //Function for normal mapping  
{
 
    // here we build the tbn basis. to transform the sampled normal to texture space
    //then we return the normal and use it with our calculations
 
    //Convert from [0,1] to [-1,1]
    float3 normalT = 2.0f * normalMapSample - 1.0f;
 
    //Build basis
    float3 N = normal;
 
    //Make sure tangent is completely orthogonal to normal
    float3 T = tangent - dot(tangent, N) * N; //Read page 582
    float3 B = -cross(N, T); //Bitangent
 
                                                //Create the "Texture Space" matrix
    float3x3 TBN = float3x3(T, B, N);
 
    //Convert normal from normal map to texture space and store in input.normal
 
    float3 bumpedNormal = mul(normalT, TBN);
 
    return normalize(bumpedNormal);
 
}

float3x3 TBN(float3 normal, float3 tangent) //Function for normal mapping  
{
   tangent -= dot(tangent, normal) * normal; //Read page 582
    float3 B = -cross(normal, tangent); //Bitangent
                                                
    return float3x3(tangent, B, normal); //Return the "Texture Space" matrix
}

//-----------------------------------------------------------------------------------------
// GEOMETRY SHADER
//-----------------------------------------------------------------------------------------
[maxvertexcount(3)]
void GS_main(triangle VS_OUT input[3],
 inout TriangleStream <GS_OUT> output)
{
    GS_OUT element = (GS_OUT)0;


    //float3 faceEdgeA = input[1].wPos - input[0].wPos;
    //float3 faceEdgeB = input[2].wPos - input[0].wPos;

    //float2 uvEdge1 = input[1].UV - input[0].UV;
    //float2 uvEdge2 = input[2].UV - input[0].UV;
    //float3 tangent = (uvEdge2[1] * faceEdgeA - uvEdge1[1] * faceEdgeB) * (1 / (uvEdge1[0] * uvEdge2[1] - uvEdge2[0] * uvEdge1[1]));
    //tangent = normalize(tangent);

       
    //matrix invWorld = transpose(worldMatrix);



    for (uint vertex = 0; vertex < 3; vertex++)
    {
        element.Pos = input[vertex].Pos;
        element.UV = input[vertex].UV;
        element.wPos = input[vertex].wPos;

        element.TBN = TBN(input[vertex].Normal, input[vertex].Tangent);

        output.Append(element);
    }

    output.RestartStrip();
}


//-----------------------------------------------------------------------------------------
// PIXEL SHADER
//-----------------------------------------------------------------------------------------
PS_OUT PS_main(GS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    output.color = albedoTex.Sample(linearSampler, input.UV).rgb;
    output.metalRoughAo.r = metalTex.Sample(linearSampler, input.UV).r;
    output.metalRoughAo.g = roughTex.Sample(linearSampler, input.UV).r;
    output.metalRoughAo.b = aoTex.Sample(linearSampler, input.UV).r;
    //output.normal = normalSamp;
    //output.normal = input.tangent;
    output.normal = normalize(mul(2.0f * normalTex.Sample(linearSampler, input.UV).rgb - 1.0f, input.TBN));
    //output.normal = mul(float4(output.normal, 0), worldMatrix).rgb;
    output.wPosition = input.wPos;
   
    //output.metal_rough_AO = aoTex.Sample(pointSampler, input.UV);
    //float3 normalT = 2.0f * normalSamp - 1.0f;
    //float3 bumpedNormal = mul(normalT, TBN);
    //normalize
    return output;
};

