Texture2D albedoTex		 : register(t0);
Texture2D metalTex		 : register(t1);
Texture2D roughTex		 : register(t2);
Texture2D normalTex		 : register(t3);
Texture2D aoTex			 : register(t4);

SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);


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
   
    float3 tangent : TANGENT;
    float3 biTangent : BINORMAL;
    
    float4 wPos : WORLDPOSITION;
};

struct PS_OUT
{
    float3 color : SV_Target0;
    float3 metal : SV_Target1;
    float3 rough : SV_Target2;
    float3 AO : SV_Target3;
    float3 normal : SV_Target4;
    float4 wPosition : SV_Target5;
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
    float3 T = normalize(tangent - dot(tangent, N) * N); //Read page 582
    float3 B = cross(N, T); //Bitangent
 
                                                //Create the "Texture Space" matrix
    float3x3 TBN = float3x3(T, B, N);
 
    //Convert normal from normal map to texture space and store in input.normal
 
    float3 bumpedNormal = mul(normalT, TBN);
 
    return normalize(bumpedNormal);
 
}



//-----------------------------------------------------------------------------------------
// GEOMETRY SHADER
//-----------------------------------------------------------------------------------------
[maxvertexcount(6)]
void GS_main(triangle VS_OUT input[3],
 inout TriangleStream <GS_OUT> output)
{
    GS_OUT element = (GS_OUT)0;


    /*float4 vec1 = input[0].Pos - input[1].Pos;
    float4 vec2 = input[1].Pos - input[2].Pos;
    float4 vec3 = input[2].Pos - input[0].Pos;

    float4 newNormal = float4(cross(vec1.xyz, vec2.xyz), 1);
    newNormal = normalize(newNormal);*/

    for (uint vertex = 0; vertex < 3; vertex++)
    {
        element.Pos = input[vertex].Pos;
        element.Normal = input[vertex].Normal; //mul(float4(input[vertex].Normal, 0.0), worldMatrix).rgb;
		//element.Normal = newNormal.xyz;
        element.UV = input[vertex].UV;
       
        element.wPos = input[vertex].wPos;

        element.tangent = input[vertex].Tangent; //mul(float4(input[vertex].Tangent, 0.0), worldMatrix);

        //element.biTangent = -cross(element.Normal, element.tangent);

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

    float3 normalSamp = normalTex.Sample(linearSampler, input.UV).rgb;

    output.color = albedoTex.Sample(linearSampler, input.UV).rgb;
    output.metal = metalTex.Sample(linearSampler, input.UV).rgb;
    output.rough = roughTex.Sample(linearSampler, input.UV).rgb;
    output.AO = aoTex.Sample(linearSampler, input.UV).rgb;
    output.normal = normalToWorldSpace(normalSamp, input.Normal, input.tangent);
    output.wPosition = input.wPos;
   
    //output.metal_rough_AO = aoTex.Sample(pointSampler, input.UV);

    return output;
};

