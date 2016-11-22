
struct GSInput
{
	float4 position : SV_POSITION;
};

struct PSInput
{
	float4 position : SV_POSITION;
};

[maxvertexcount(3)]
void main(triangle GSInput input[3], inout TriangleStream< PSInput > output)
{
	PSInput element;

	for (uint i = 0; i < 3; i++)
	{
		element.position = input[i].position;

		output.Append(element);
	}
}