Texture2D backBufferTex		: register(t6);
Texture2D normalTex		         : register(t2);
SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);


static const float WIN_WIDTH = 1280.0f;
static const float WIN_HEIGHT = 720.0f;
static const float INV_WIN_WIDTH = 1.0/1280.0f;
static const float INV_WIN_HEIGHT = 1.0/720.0f;

static const float EDGE_THRESHOL_MAX = 0.125f;
static const float EDGE_THRESHOL_MIN = 0.0312f;

static const int ITERATIONS = 12;
static const float QUALITY[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 2.0f, 4.0f, 8.0f };
static const float SUBPIXEL_QUALITY = 0.75f;

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};


float rgb2luma(float3 rgb)
{
    //luminosity calculation with weighted rgb channels in accordance to the human eye's
    //sensitivity to the individal colors.
    return sqrt(dot(rgb, float3(0.299, 0.587, 0.114)));
}

float4 PS_main(VS_OUT input) : SV_TARGET
{
	float4 bbSamp = float4(backBufferTex.Sample(pointSampler, input.UV));
    float4 final;
    //return bbSamp;
    //EDGE DETECTION
    
    //luma for current pixel
    float lumaCenter = rgb2luma(bbSamp.rgb);

    //luma in all four direct neighbouring pixels
    float lumaDown = rgb2luma(backBufferTex.Sample(pointSampler, input.UV, int2(0, -1)).rgb);
    float lumaUp = rgb2luma(backBufferTex.Sample(pointSampler, input.UV, int2(0, 1)).rgb);
    float lumaLeft = rgb2luma(backBufferTex.Sample(pointSampler, input.UV, int2(-1, 0)).rgb);
    float lumaRight = rgb2luma(backBufferTex.Sample(pointSampler, input.UV, int2(1, 0)).rgb);

    //find maximum and minimun luma in neighbours
    float lumaMin = min(lumaCenter, min(min(lumaDown, lumaUp), min(lumaLeft, lumaRight)));
    float lumaMax = max(lumaCenter, max(max(lumaDown, lumaUp), max(lumaLeft, lumaRight)));

    //Get the luma delta
    float delta = lumaMax - lumaMin;

    if (delta < max(EDGE_THRESHOL_MIN, lumaMax * EDGE_THRESHOL_MAX))
    {
        return bbSamp;
    }

    //Get remaining corner luma
    float lumaDownLeft = rgb2luma(backBufferTex.Sample(pointSampler, input.UV, int2(-1, -1)).rgb);
    float lumaUpRight = rgb2luma(backBufferTex.Sample(pointSampler, input.UV, int2(1, 1)).rgb);
    float lumaUpLeft = rgb2luma(backBufferTex.Sample(pointSampler, input.UV, int2(-1, 1)).rgb);
    float lumaDownRight = rgb2luma(backBufferTex.Sample(pointSampler, input.UV, int2(1, -1)).rgb);

    //Combine the four edge lumas
    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;

    //Same for corners
    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;

    //get an estimated gradient along the x and y axis
    float edge_x = abs(-2.0f * lumaLeft + lumaLeftCorners) + abs(-2.0f * lumaCenter + lumaDownUp) * 2.0f + abs(-2.0f * lumaRight + lumaRightCorners);
    float edge_y = abs(-2.0f * lumaUp + lumaUpCorners) + abs(-2.0f * lumaCenter + lumaLeftRight) * 2.0f + abs(-2.0f * lumaDown + lumaDownCorners);

    //is the local edge horizontal or vertical?
    bool isHorizontal = (edge_x >= edge_y);

    //select the two neighboring texels in the opposite direction to the local edge
    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;

    //get gradients in this direction
    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;

    //which direction is the steepest?
    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    //gradient in the corresponding direction, noramlized
    float gradientScaled = 0.25f * max(abs(gradient1), abs(gradient2));

    //chose the stepLength (one pixel) according to the edge direction
    float stepLength = isHorizontal ? INV_WIN_HEIGHT : INV_WIN_WIDTH;

    //average luma in the correct direction
    float lumaLocalAvg = 0.0f;
    
    if(is1Steepest)
    {
        //switch the direction
        stepLength = -stepLength;
        lumaLocalAvg = 0.5f * (luma1 + lumaCenter);
    }
    else
    {
        lumaLocalAvg = 0.5f * (luma2 + lumaCenter);
    }

    //shift the UV in the correct direction by half a pixel
    float2 currentUV = input.UV;
    if (isHorizontal)
    {
        currentUV.y += stepLength * 0.5f;
    }
    else
    {
        currentUV.x += stepLength * 0.5f;
    }

    //compute offset for each iteration step in the correct direction
    float2 offset = isHorizontal ? float2(INV_WIN_WIDTH, 0.0f) : float2(0.0f, INV_WIN_HEIGHT);
    //compute the uvs to explore on each side of the edge, orthoganally
    float2 uv1 = currentUV - offset;
    float2 uv2 = currentUV + offset;

    //read the lumas at both current extremites of the exploration segment and compute the delta wrt to the local avarage luma
    float lumaEnd1 = rgb2luma(backBufferTex.Sample(pointSampler, uv1).rgb);
    float lumaEnd2 = rgb2luma(backBufferTex.Sample(pointSampler, uv2).rgb);
    lumaEnd1 -= lumaLocalAvg;
    lumaEnd2 -= lumaLocalAvg;

    //if the luma deltas at the current extremities are largen than the local gradient, we have reached the side of the edge
    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    if(!reached1)
        uv1 -= offset;
    if(!reached2)
        uv2 += offset;
    if(!reachedBoth)
    {
        for (int i = 2; i < ITERATIONS; i++)
        {
            // If needed, read luma in 1st direction, compute delta
            if (!reached1)
            {
                lumaEnd1 = rgb2luma(backBufferTex.Sample(pointSampler, uv1).rgb);
                lumaEnd1 -= lumaLocalAvg;
            }
            // If needed, read luma in opposite direction, compute delta
            if (!reached2)
            {
                lumaEnd2 = rgb2luma(backBufferTex.Sample(pointSampler, uv2).rgb);
                lumaEnd2 -= lumaLocalAvg;
            }
            //if the luma deltas at the current extremities are larger than the local gradients we've reached the side of the edge
            bool reached1 = abs(lumaEnd1) >= gradientScaled;
            bool reached2 = abs(lumaEnd2) >= gradientScaled;
            bool reachedBoth = reached1 && reached2;
            if (!reached1)
            {
                uv1 -= offset * QUALITY[i];
            }
            if (!reached2)
            {
                uv2 += offset * QUALITY[i];
            }
            if (reachedBoth)
                break;
        }
    }

    //compute the distances to each extremity of the edge
    float distance1 = isHorizontal ? (input.UV.x - uv1.x) : (input.UV.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - input.UV.x) : (uv2.y - input.UV.y);

    //in which direction is the edge closer?
    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    //leght of of the edge
    float edgeThickness = (distance1 + distance2);

    //uv offset: read in the direction of the closest side of the edge
    float pixelOffset = -distanceFinal / edgeThickness + 0.5f;

    //is the luma at the center smaller than the local avg?
    bool isLumaCenterSmaller = lumaCenter < lumaLocalAvg;

    //if the luma at center is smaller than its neighbour, the delta luma at each end should be positive (same variation)
    // (in the direction of the closer side of the edge)
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0f) != isLumaCenterSmaller;

    //if the luma variation is incorrect, do not offset
    float finalOffset = correctVariation ? pixelOffset : 0.0;

    //Subpixel shifting
    //full weighted avg of the luma over the 3x3 neighbour
    float lumaAvg = (1.0f / 12.0f) * (2.0f * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
    // Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood
    float subPixelOffset1 = clamp(abs(lumaAvg - lumaCenter) / delta, 0.0f, 1.0f);
    float subPixelOffset2 = (-2.0f * subPixelOffset1 + 3.0f) * subPixelOffset1 * subPixelOffset1;
    //compute the subpixel offset based on this delta
    float subpixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

    //pick the biggest of the two offsets
    finalOffset = max(finalOffset, subpixelOffsetFinal);

    //compute the final uv coordinates
    float2 finalUV = input.UV;
    if(isHorizontal)
    {
        finalUV.y += finalOffset * stepLength;
    }
    else
    {
        finalUV.x += finalOffset * stepLength;
    }

    final = backBufferTex.Sample(pointSampler, finalUV);

    return final;
    return final = float4(1.0,1.0,1.0,1.0);
    
}