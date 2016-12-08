Texture2D colorTex		: register(t0);
Texture2D metal         : register(t1);
Texture2D rough         : register(t2);
Texture2D AO            : register(t3);
Texture2D normalTex		: register(t4);
Texture2D wPosTex		: register(t5);

SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);

//cbuffer worldMatrix : register(b0)
//{
//	matrix worldMatrix;
//}

cbuffer camera : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;

    float4 camPos;
    float4 padding1;
    float4 padding2;
    float4 padding3;
}

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};


struct LIGHT
{
	float3 lightPos;
	float3 lightDir;
	float3 lightColor;
	float3 lightAmbient;
};


LIGHT initLight()
{
    LIGHT light;
    light.lightPos = float3(0.0f, 0.0f, -1.5f);
	light.lightDir = float4(0.0f, 0.5f, 1.0f, 1.0f);
    light.lightColor = float3(1.0f, 1.0f, 1.0f);
    light.lightAmbient = float3(0.2f, 0.2f, 0.2f);

    return light;
}

LIGHT initCustomLight(float3 pos, float3 color)
{
    LIGHT light;
    light.lightPos = pos;
	light.lightDir = float4(0.0f, 0.5f, 1.0f, 1.0f);
    light.lightColor = color;
    light.lightAmbient = float3(0.2f, 0.2f, 0.2f);

    return light;
}

//PBR FUNTIONS
//sRGB n Linear
float3 linearToSRGB( in float3 linearCol)
{
    float3 sRGBLow = linearCol * 12.92;
    float3 sRGBHigh = (pow(abs(linearCol), 1.0 / 2.4) * 1.055) - 0.055;
    float3 sRGB = (linearCol <= 0.0031308) ? sRGBLow : sRGBHigh;
    return sRGB;
}

float3 sRGBtoLinear(in float3 sRGBCol)
{
    float3 sRGBLow = sRGBCol / 12.92;
    float3 sRGBHigh = (pow(abs(sRGBCol), 1.0 / 2.4) / 1.055) - 2.4;
    float3 linearRGB = (sRGBCol <= 0.04045) ? sRGBLow : sRGBHigh;
    return linearRGB;
}

float3 schlick(float3 f0, float f90, float u) //f0 is spec/metalness, f90 is spec/metal factor
{
    return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
}

float DisneyDiffuse(float NdotV, float NdotL, float LdotH, float linearRoughness)
{
    float energyBias = lerp(0, 0.5, linearRoughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, linearRoughness);
    float fd90 = energyBias + 2.0 * LdotH * LdotH * linearRoughness;
    float3 f0 = float3(1.0, 1.0, 1.0);
    float lightScatter = schlick(f0, fd90, NdotL).r;
    float viewScatter = schlick(f0, fd90, NdotV).r;
    
    return lightScatter * viewScatter * energyFactor;
}

float V_SmithGGXCorrelated(float NdotL, float NdotV, float alphaG)
{
    // Original formulation of G_SmithGGX Correlated
    // lambda_v = (-1 + sqrt ( alphaG2 * (1 - NdotL2 ) / NdotL2 + 1)) * 0.5 f;
    // lambda_l = (-1 + sqrt ( alphaG2 * (1 - NdotV2 ) / NdotV2 + 1)) * 0.5 f;
    // G_SmithGGXCorrelated = 1 / (1 + lambda_v + lambda_l );
    // V_SmithGGXCorrelated = G_SmithGGXCorrelated / (4.0 f * NdotL * NdotV );

    // This is the optimize version
    float alphaG2 = alphaG * alphaG;
    // Caution : the " NdotL *" and " NdotV *" are explicitely inversed , this is not a mistake .
    float Lambda_GGXV = NdotL * sqrt((-NdotV * alphaG2 + NdotV) * NdotV + alphaG2);
    float Lambda_GGXL = NdotV * sqrt((-NdotL * alphaG2 + NdotL) * NdotL + alphaG2);

    return 0.5f / (Lambda_GGXV + Lambda_GGXL);
}


float GGX(float NdotH, float m)
{
    //divide by PI happens later
    float m2 = m * m;
    float f = (NdotH * m2 - NdotH) * NdotH + 1;
    return m2 / (f * f);
}

float4 PS_main(VS_OUT input) : SV_Target
{
    uint lightCount = 3;
    float Pi = 3.14159265359;
    float EPSILON = 1e-5f;

    float4 diffuseLight = float4(0, 0, 0, 0);
    float4 specularLight = float4(0, 0, 0, 0);

    LIGHT light[3]; 
    light[0] = initCustomLight(float3(0.0, 0.0, -3.0), float3(1., 1., 1.));
    light[1] = initCustomLight(float3(0.0, 0.0, -3.5), float3(1., 1., 1.));
    light[2] = initCustomLight(float3(0.5, 1.2, -2.0), float3(1., 1., 1.));

    //SAMPLING
    float4 wPosSamp = wPosTex.Sample(pointSampler, input.UV);
    float3 metalSamp = (metal.Sample(pointSampler, input.UV));
    float3 roughSamp = (rough.Sample(pointSampler, input.UV));
    float3 AOSamp = (AO.Sample(pointSampler, input.UV)).rgb;
    float3 colorSamp = (colorTex.Sample(pointSampler, input.UV));
    float3 N = (normalTex.Sample(pointSampler, input.UV));



    //METALNESS (F90)
    float metalness = metalSamp.r;
    float f90 = metalness;
    f90 = 0.16f * metalness * metalness;

    //ROUGHNESS (is same for both diffuse and specular, ala forstbite)
    float linearRough = (saturate(roughSamp + EPSILON)).r;
    float roughness =  linearRough * linearRough;
    //float sRGBrough = linearToSRGB(met_rough_ao_Samp.ggg).g; //takes float3, could cause error

    //AO
    float AO = AOSamp.b;

    //DIFFUSE & SPECULAR
    float3 diffuseColor = lerp(colorSamp.rgb, 0.0f.rrr, metalness);
    float3 f0 = lerp(0.03f.rrr, colorSamp.rgb, metalness);
    float3 specularColor = lerp(f0, colorSamp.rgb, metalness);

    //N = normalize(N);
    float3 V = normalize(wPosSamp.xyz - camPos.xyz);
    float NdotV = abs(dot(N, V)) + EPSILON;
    
    //FOR EACH LIGHT
    for (uint i = 0; i < lightCount; i++)
    {
        //PBR variables 
        float3 L = normalize((wPosSamp.xyz) - light[i].lightPos);
        float3 H = normalize(V + L);
        float lightPower = 0;

        float LdotH = saturate((dot(L, H)));
        float NdotH = saturate((dot(N, H)));
        float NdotL = saturate((dot(N, L)));
        float VdotH = saturate((dot(V, H)));


        
        //if (dot(camPos - light.lightPos, normalize(light.lightDir)) > 0) //just for lights with direction. Or selfshadowing, or maby just needed for everything... pallante tänka påat atm

        //else //lights with no direction


        lightPower = 1.00f; //could add falloff factor
        
        //DO SHADOW STUFF HERE

        //DIFFUSE
        float fd = DisneyDiffuse(NdotV, NdotL, LdotH, linearRough.r) / Pi; //roughness should be linear
        diffuseLight += float4(fd.xxx * light[i].lightColor * lightPower * diffuseColor.rgb, 1);

        //SPECULAR
        float3 f = schlick(f0, f90, LdotH);
        float vis = V_SmithGGXCorrelated(NdotV, NdotL, roughness); //roughness should be sRGB
        float d = GGX(NdotH, roughness); //roughness should be sRGB

        float3 fr = d * f * vis / Pi;

        specularLight += float4(fr * specularColor * light[i].lightColor * lightPower, 1);

        //return V.rgbr;
    }


    //COMPOSITE
    float3 diffuse = saturate(diffuseLight).rgb;
    float3 specular = specularLight.rgb;

    //float4 finalColor = float4(specular, 1);
    float4 finalColor = float4(saturate(diffuse), 1);
    finalColor.rgb += saturate(specular);


    
    return finalColor;





    ////STANDARD NORMAL/LIGHT CALC N RENDER
    //float3 lightDirection = normalize(light.lightPos - (wPosSamp.xyz));
    //float lightIntensity = saturate(dot(N, lightDirection));

    //float3 norColor = saturate(light.lightColor * lightIntensity);
    
    //return float4(saturate(colorSamp.xyz * norColor).xyz + (colorSamp.xyz * light.lightAmbient), 1.0);
    ////END

    //float4 AOsamp = AOTex.Sample(linearSampler, input.UV);


    //return normSamp;
};
