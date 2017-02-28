Texture2D colorTex		         : register(t0);
Texture2D metalRoughAo           : register(t1);
Texture2D normalTex		         : register(t2);
Texture2D wPosTex		         : register(t3);
TextureCube shadowTex            : register(t10); // 7,8,9 is taken up by light buffers, If this is changed, modify the "SetShadowDataToRead()" function in DeferredShader.h
TextureCubeArray sShadowsTexA    : register(t11);
SamplerState linearSampler       : register(s0);
SamplerState pointSampler        : register(s1);

//must match ConstantBufferHandler.h define || //Must be multiple of 4
#define MAX_SHADOW_LIGHTS 20  
#define SHADOW_BIAS  0.0000088f
cbuffer camera : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;

    float4 camPos;

}



cbuffer LightInfo : register(b3)
{
    uint   NUM_POINTLIGHTS;
    float3 AMBIENT_COLOR;
    float  AMBIENT_INTENSITY;
    uint   DYNAMIC_SHADOWLIGHT_INDEX;
    uint     padding[2];
    int SHADOWCASTING_LIGHTS[MAX_SHADOW_LIGHTS]; //Must be multiple of 4

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


StructuredBuffer<PointLight> pointlights : register(t6);

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
    light.lightPos     = float3(0.0f, 0.0f, -1.5f);
	light.lightDir     = float3(0.0f, 0.5f, 1.0f);
    light.lightColor   = float3(1.0f, 1.0f, 1.0f);
    light.lightAmbient = float3(0.2f, 0.2f, 0.2f);

    return light;
}

LIGHT initCustomLight(float3 pos, float3 color)
{
    LIGHT light;
    light.lightPos     = pos;
	light.lightDir     = float3(0.0f, 0.5f, 1.0f);
    light.lightColor   = color;
    light.lightAmbient = AMBIENT_COLOR.rrr;
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

float pointIllumination(float3 P, float3 N, float3 lightCentre, float r, float c, float l, float q, float cutoff)
{
    float3 L = P-lightCentre;
    float distance = length(L);
    float d = max(distance, 0);
    L /= distance;

    float df = (1 / r);

    float attenuation = 1 / ((c * distance) + (l * distance) + (q * distance * distance));



    return attenuation;
}

float smoothAttenuation(float3 P, float3 lightCentre, float range, float c, float l, float q)
{
    float3 L = lightCentre - P;
    float distance = length(L);

    
    float la = l * distance;
    float qa = q * distance * distance;

    

    float attenuation = 1 / (c + la + qa);
    //attenuation += 1.0f - smoothstep(range * c, range, distance);


    float final = 1.0f - smoothstep(range * attenuation, range, distance);
   // attenuation += 1.0f - smoothstep(range * qa, range, distance);

    return max(final, 0);

}

float DirectIllumination(float3 P, float N, float3 lightCentre, float r, float cutoff)
{

    float innerRadius = 0.25;
    // calculate normalized light vector and distance to sphere light surface
    float3 L = lightCentre - P;
    float distance = length(L);
    float d = max(distance - innerRadius, 0);
    L /= distance;

    // calculate basic attenuation
    float denom = d / innerRadius + 1;
    float attenuation = 1 / (denom * denom);

    // scale and bias attenuation such that:
    //   attenuation == 0 at extent of max influence
    //   attenuation == 1 when d == 0
    attenuation = (attenuation - cutoff) / (1 - cutoff);
    attenuation = max(attenuation, 0);

    float DOT = max(dot(L, N), 0);
    attenuation *= DOT;
    attenuation *= saturate (d / (r - innerRadius));

    return attenuation;
}

float VecToDepth(float3 vec)
{
    float3 absVec = abs(vec);
    float LocalZcomp = max(absVec.x, max(absVec.y, absVec.z));

    const float f = 9.0f;
    const float n = 0.0005f;

    //float NormZComp = (f + n) / (f - n) - (2 * f * n) / (f - n) / LocalZcomp;

    float NormZComp = -(f / (n - f) - (n * f) / (n - f) / LocalZcomp); //because this isnt opengl

    return NormZComp;
}

float sampleStaticShadowStencils(float3 worldPos, float3 lpos, int shadowIndex)
{
    static const float3 gCubeSampleOffset[8] =
    {
        { 0.875f, -0.375f, -0.125f },
        { 0.625f, 0.625f, -0.625f },
        { 0.375f, 0.125f, -0.875f },
        { 0.125f, -0.875f, 0.125f },
        { 0.125f, 0.875f, 0.375f },
        { 0.375f, -0.625f, 0.625f },
        { 0.625f, -0.125f, 0.875f },
        { 0.875f, 0.375f, -0.375f }
    };

    float shadowFactor = 0.0f;
    
    float bias = SHADOW_BIAS;
    float3 pixToLight = worldPos - lpos;

    [unroll]
    for (int i = 0; i < 8; i++)
    {
        float closestDepth = sShadowsTexA.Sample(linearSampler, float4(pixToLight + (gCubeSampleOffset[i] * 0.025), float(shadowIndex)));
        shadowFactor += closestDepth + bias < VecToDepth(pixToLight) ? 0.0f : 0.125f;
    }


    return min(shadowFactor, 1.0);
}


float sampleShadowStencils(float3 worldPos, float3 lpos, float currentShadowFactor)
{
    static const float3 gCubeSampleOffset[8] =
    {
        { 0.875f, -0.375f, -0.125f },
        { 0.625f, 0.625f, -0.625f },
        { 0.375f, 0.125f, -0.875f },
        { 0.125f, -0.875f, 0.125f },
        { 0.125f, 0.875f, 0.375f },
        { 0.375f, -0.625f, 0.625f },
        { 0.625f, -0.125f, 0.875f },
        { 0.875f, 0.375f, -0.375f }
    };

    float shadowFactor = 0.0f;
    
    float bias = SHADOW_BIAS;
    float3 pixToLight = worldPos - lpos;

    [unroll]
    for (int i = 0; i < 8; i++)
    {
        float closestDepth = shadowTex.Sample(linearSampler, (pixToLight) + (gCubeSampleOffset[i] * 0.025));
        shadowFactor += closestDepth + bias < VecToDepth(pixToLight) ? 0.0f : 0.125f;
    }
    

    return currentShadowFactor < shadowFactor ? currentShadowFactor : min(shadowFactor, 1.0);
    
}

float4 PS_main(VS_OUT input) : SV_Target
{


   // return shadowTex.Sample(linearSampler, float3(input.UV, 0)).rrrr;

    uint lightCount = NUM_POINTLIGHTS;
    float Pi = 3.14159265359;
    float EPSILON = 1e-5f;

    float4 diffuseLight  = float4(0, 0, 0, 0);
    float4 specularLight = float4(0, 0, 0, 0);

    LIGHT light[3]; 
    light[0] = initCustomLight(float3(10.0, -9.0, -3.0), pointlights[0].color); //float3(0.0, 0.0, -3.0), float3(1., 1., 1.));   pointlights[0].position.xyz
    //light[1] = initCustomLight(float3(14.0, -9.0, -3.0), pointlights[1].color); //float3(0.0, 0.0, -3.5), float3(1., 1., 1.));   pointlights[1].position.xyz
    //light[2] = initCustomLight(float3(18.0, -9.0,  -3.0), pointlights[2].color); //float3(0.5, 1.2, -2.0), float3(1., 1., 1.));   pointlights[2].position.xyz

    //SAMPLING
    float4 wPosSamp  = wPosTex.Sample(pointSampler, input.UV);
    float metalSamp = (metalRoughAo.Sample(pointSampler, input.UV)).r;
    float roughSamp = (metalRoughAo.Sample(pointSampler, input.UV)).g;
    float AOSamp = (metalRoughAo.Sample(pointSampler, input.UV)).b;
    float3 colorSamp = (colorTex.Sample(pointSampler, input.UV)).rgb;
    float3 N = (normalTex.Sample(pointSampler, input.UV)).rgb;
    //return N.rgbr;


    //METALNESS (F90)
    float f90 = metalSamp;
    f90 = 0.16f * metalSamp * metalSamp;

    //ROUGHNESS (is same for both diffuse and specular, ala forstbite)
    float linearRough = (saturate(roughSamp + EPSILON)).r;
    float roughness =  linearRough * linearRough;
    //float sRGBrough = linearToSRGB(met_rough_ao_Samp.ggg).g; //takes float3, could cause error

    //AO
    float AO = AOSamp;

    //DIFFUSE & SPECULAR
    float3 diffuseColor = lerp(colorSamp.rgb, 0.0f.rrr, f90);
    float3 f0 = lerp(0.03f.rrr, colorSamp.rgb, f90);
    float3 specularColor = lerp(f0, colorSamp.rgb, f90);

    //N = normalize(N);
    float3 V = normalize(camPos.xyz - wPosSamp.xyz);
    float NdotV = abs(dot(N, V)) + EPSILON;
    
    float shadowFactor = 1.0;
    int currentShadowLightIndex = 0;

    //FOR EACH LIGHT
    for (uint i = 0; i < lightCount; i++) ///TIP : Separate each light type calculations into functions. i.e : calc point, calc area, etc
    {
        float lightPower = 0;

        lightPower  = smoothAttenuation(wPosSamp.xyz, pointlights[i].position.xyz, pointlights[i].radius, pointlights[i].constantFalloff, pointlights[i].linearFalloff, pointlights[i].quadraticFalloff);
        lightPower *= (AOSamp);
        lightPower *= pointlights[i].intensity; 
        if (lightPower > 0.0f)
        {
            //PBR variables 
            float3 L = normalize(pointlights[i].position.xyz - (wPosSamp.xyz));
            float3 H = normalize(V + L);

            float LdotH = saturate((dot(L, H)));
            float NdotH = saturate((dot(N, H)));
            float NdotL = max(saturate((dot(N, L))), 0.004f); //the max function is there to reduce/remove specular artefacts caused by a lack of reflections
            float VdotH = saturate((dot(V, H)));
            
            //SHADOW
            int bajs = SHADOWCASTING_LIGHTS[currentShadowLightIndex].x;
            if (i == bajs)
            {   
                shadowFactor = sampleStaticShadowStencils(wPosSamp.xyz, pointlights[i].position.xyz, currentShadowLightIndex);

			   // lightPower *= shadowFactor;
                currentShadowLightIndex++;
            }
            if (i == DYNAMIC_SHADOWLIGHT_INDEX)
            {
                shadowFactor = sampleShadowStencils(wPosSamp.xyz, pointlights[DYNAMIC_SHADOWLIGHT_INDEX].position.xyz, shadowFactor);
                lightPower *= shadowFactor;
            }
            //DIFFUSE
            float fd = DisneyDiffuse(NdotV, NdotL, LdotH, linearRough.r) / Pi; //roughness should be linear
            diffuseLight += float4(fd.xxx * pointlights[i].color * lightPower * diffuseColor.rgb, 1);

            //SPECULAR
            float3 f  = schlick(f0, f90, LdotH);
            float vis = V_SmithGGXCorrelated(NdotV, NdotL, roughness); //roughness should be sRGB
            float d   = GGX(NdotH, roughness); //roughness should be sRGB

            float3 fr = d * f * vis / Pi;

            specularLight += float4(fr * specularColor * pointlights[i].color * lightPower, 1);

           // return diffuseLight;
        }
    }


    //return shadowFactor;
    //COMPOSITE
    float3 diffuse = saturate(diffuseLight.rgb);
    float3 ambient = saturate(colorSamp * AMBIENT_COLOR * AMBIENT_INTENSITY);
    float3 specular = specularLight.rgb;
    

    //float4 finalColor = float4(specular, 1);
    float4 finalColor = float4(saturate(diffuse), 1);
    finalColor.rgb += saturate(specular);
    finalColor.rgb += ambient;
    //finalColor.rgb *= AOSamp;

    
    return saturate(finalColor);





    ////STANDARD NORMAL/LIGHT CALC N RENDER
    //float3 lightDirection = normalize(light.lightPos - (wPosSamp.xyz));
    //float lightIntensity = saturate(dot(N, lightDirection));

    //float3 norColor = saturate(light.lightColor * lightIntensity);
    
    //return float4(saturate(colorSamp.xyz * norColor).xyz + (colorSamp.xyz * light.lightAmbient), 1.0);
    ////END

    //float4 AOsamp = AOTex.Sample(linearSampler, input.UV);


    //return normSamp;
};
