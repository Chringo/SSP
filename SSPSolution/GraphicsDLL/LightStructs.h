
#ifndef GRAPHICSDLL_LIGHTSTRUCTS_H
#define GRAPHICSDLL_LIGHTSTRUCTS_H
#include <DirectXMath.h>
#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif
namespace LIGHTING
{
	enum GRAPHICSDLL_API LIGHT_TYPE
	{
		LT_POINT,
		LT_DIRECTIONAL,
		LT_AREA,
		LT_SPOT,

		NUM_LT
	};

	struct LightBufferData 
	{
		UINT NUM_POINTLIGHTS;
		UINT NUM_AREALIGHTS;
		UINT NUM_DIRECTIONALLIGHTS;
		UINT NUM_SPOTLIGHTS;
		float AMBIENT_COLOR[3]  = { 1.0f,1.0f,1.0f };
		float AMBIENT_INTENSITY = 0.2f;

	};

	struct GRAPHICSDLL_API LightColor
	{
		float r = 1.0f,g = 1.0f,b = 1.0f ;
		
	};
	struct GRAPHICSDLL_API LightFalloff
	{
		float constant = 1.0f, linear  =1.0f, quadratic = 1.0f;
	};

	struct GRAPHICSDLL_API Light // Any changes to these structs need to be made in the shader
	{
		BOOL isActive = TRUE; //16 bit aligned

		float padding[3];	  //16 bit aligned
		LightColor color;
		float intensity = 1.0f;

		 void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		 void* operator new[](size_t i) { return _aligned_malloc(i, 16); };
		 void operator delete[](void* p) { _aligned_free(p); };
		 void operator delete(void* p) { _aligned_free(p); };
	};

	struct GRAPHICSDLL_API Point : Light
	{
		DirectX::XMVECTOR position;
		float radius = 10.0f;
		LightFalloff falloff;
		
	};

	struct GRAPHICSDLL_API Area : Light
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR direction;
		DirectX::XMVECTOR up;
		DirectX::XMVECTOR right;

		LightFalloff falloff;

		float width;
		float height;
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};

	struct GRAPHICSDLL_API Directional : Light
	{
		DirectX::XMVECTOR direction;
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};

	struct GRAPHICSDLL_API Spot : Light
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR direction;
		LightFalloff falloff;
		//cone stuff here
		float range, cone, pad;
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};

}

#endif