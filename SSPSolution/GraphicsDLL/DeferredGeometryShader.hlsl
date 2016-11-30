
struct GSInput
{
	float4 position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 UV : TEXCOORD0;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 UV : TEXCOORD0;
};

[maxvertexcount(3)]
void main(triangle GSInput input[3], inout TriangleStream< PSInput > output)
{
	PSInput element = (PSInput)0;

	for (uint i = 0; i < 3; i++)
	{
		element.position = input[i].position;
		element.UV = input[i].UV;
		output.Append(element);
	}
    output.RestartStrip();
}