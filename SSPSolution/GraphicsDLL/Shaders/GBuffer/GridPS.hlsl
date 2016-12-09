struct PS_OUT
{
    float3 color : SV_Target0;
    float3 metal : SV_Target1;
    float3 rough : SV_Target2;
};

PS_OUT PS_main()
{

    PS_OUT output = (PS_OUT) 0;

    output.color = float3(.0f, .45f, .47f);
    output.metal = float3(1.f, 1.f, 1.f);
    output.rough = float3(1.f, 1.f, 1.f);
   // output.wPosition = //input


	return output;
}