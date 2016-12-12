Texture2D backBufferTex		: register(t6);

SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);


uniform float WIN_WIDTH = 1280;
uniform float WIN_HEIGHT = 720;

cbuffer camera : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	float4 camPos;
	
	// do not need padding here. float4 = 16bit
}
struct AABB
{
	float3 pos;
	float3 ext;
};
struct Ray
{
	float3 rayPos;
	float3 rayDir;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
};

bool IntersectRayAABB(Ray ray, AABB box,   float tmin,  float tmax)
{

	float epsilon = 0.000000000000001;
	float tymin, tymax, tzmin, tzmax;
	float flag = 1.0;
	float3 pointPos = box.pos - ray.rayPos;
	float3 Buvw[3];
	float Huvw[3];
	
	Buvw[0] = normalize(float3(box.ext.x, 0.0f, 0.0f));
	Buvw[1] = normalize(float3(0.0f, box.ext.y, 0.0f));
	Buvw[2] = normalize(float3(0.0f, 0.0f, box.ext.z));

	
	Huvw[0] = abs(box.ext.x);

	Huvw[1] = abs(box.ext.y);

	Huvw[2] = abs(box.ext.z);

//for each dimension	
	bool intersect = false;
	for (int i = 0; i < 3; i++)
	{

		float e = dot(Buvw[i], pointPos);
		float f = dot(Buvw[i], ray.rayDir);
		if (abs(f) > epsilon)
		{
			float t1 = (e + Huvw[i]) / f; // Intersection with the "left" plane
			float t2 = (e - Huvw[i]) / f; // Intersection with the "right" plane
			
			if (t1 > t2)
			{ // if wrong order
				float w = t1;
				t1 = t2;
				t2 = w; // swap t1 and t2
			}
	
			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tmax)
			{
				tmax = t2;
			}
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tmin)
			{
				tmin = t1;
			}
			if (tmax < tmin)
			{
				return false;
			}
			if(tmax < 0)
			{
				return false;
			}
			
		}
		else if ((-e - Huvw[i]) > 0)
		{
			return false;
		}
		else if ((-e + Huvw[i]) < 0)
		{
			return false;
		}

	}
	return true;

}


Ray ComputeCameraRay( int u, int v)
{
	float4 camLookAt = float4(0.0f, 0.0f, 1.0f, 1.0f);
	float3 camDir = normalize(camLookAt.xyz - camPos.xyz);
	float3 camera_up = float3(0.0f, 1.0f, 0.0f);

	float3 camera_right = cross(camDir, camera_up);
	camera_up = cross(camera_right, camDir);
	double normalized_u = (u / WIN_WIDTH)   - 0.5;
	double normalized_v = (v / WIN_HEIGHT)  - 0.5;
	float3 image_point = normalized_u * camera_right +
                          normalized_v * camera_up +
                        camPos.xyz + camDir;

	Ray ray;
	ray.rayDir = image_point - camPos.xyz;
	ray.rayPos = camPos.xyz;
 
	
	return ray;
}

float4 PS_main(VS_OUT input) : SV_Target
{
	AABB testBox;
	Ray ray = ComputeCameraRay(input.UV.x,input.UV.y);
	
	float tMin = -1000000000.0f;
	float tMax = 1000000.0f;
	testBox.pos = float3(0.0f, 0.0f, 0.0f);
	testBox.ext = float3(2.0f, 1.0f, 1.0f);
	//float2 centerPoint;
	//centerPoint.x = WIN_WIDTH  / 2;
	//centerPoint.y = WIN_HEIGHT / 2;
	//ray.rayPos = camPos.xyz;
	//ray.rayDir = normalize(float3(input.UV, 0.0f) - float3(centerPoint, -0.5f));


	float4  backBufferData = backBufferTex.Sample(pointSampler, input.UV);
	float4 bbColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//if (IntersectRayAABB(ray,testBox, tMin, tMax) == true){
		bbColor = float4(0.4f, 0.0f, 0.0f, 1.0f);
	//}

	return saturate(backBufferData + bbColor);
}