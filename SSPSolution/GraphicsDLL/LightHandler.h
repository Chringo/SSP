
#ifndef GRAPHICSDLL_LIGHTHANDLER_H
#define GRAPHICSDLL_LIGHTHANDLER_H
#include <DirectXMath.h>

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif
#include <d3d11.h>
#include <vector>
#include "LightStructs.h"
#include "ConstantBufferHandler.h"

#ifdef _DEBUG
#include <iostream>
#endif
/*
	Author:Martin Clementson

	This class handles the interaction of lights and the shaders

	This assumes that the individual light data is stored elsewhere.
	It holds pointers to arrays of each individual light type.
	If an update to the gpu is desired. Call UpdateStructuredBuffer()

*/
namespace LIGHTING
{
	class GRAPHICSDLL_API LightHandler
	{
	private:
		enum LIGHT_BUFFER_SLOTS // Determines the slots that the buffers are set in the shader
		{
			POINTLIGHT_BUFFER		 = 8, // IMPORTANT: In the shader, these buffers needs to be registered as a t buffer
			DIRECTIONALLIGHT_BUFFER	 = 9, // not register(sX); BUT, register(tX); 
			AREALIGHT_BUFFER		 = 10,
			SPOTLIGHT_BUFFER		 = 11
		};
		enum MAX_LIGHTS {				//The max amount of any light type. Needed for the buffers.
			MAX_POINTLIGHTS = 15,		//Can be changed without problem
			MAX_DIRECTIONAL = 2,
			MAX_AREALIGHT	= 11,
			MAX_SPOTLIGHT	= 10
		};
		struct LightArray {
			Light* dataPtr = nullptr;
			unsigned int numItems = 0;
		};
		const int MAX_NUM_LIGHTS[NUM_LT]	  = { MAX_POINTLIGHTS,   MAX_DIRECTIONAL,          MAX_AREALIGHT,     MAX_SPOTLIGHT };
		const int BUFFER_SHADER_SLOTS[NUM_LT] = { POINTLIGHT_BUFFER, DIRECTIONALLIGHT_BUFFER,  AREALIGHT_BUFFER,  SPOTLIGHT_BUFFER };
	private:
		LightHandler();
		~LightHandler();
		LightBufferData m_constBufferData;
		LightArray m_lightData[NUM_LT];
		ID3D11Device*			  m_gDevice;
		ID3D11DeviceContext*	  m_gDeviceContext;

		ID3D11Buffer* lightBuffers[NUM_LT]					   = { nullptr,nullptr,nullptr,nullptr }; //Light constBuffers
		ID3D11ShaderResourceView*  m_structuredBuffers[NUM_LT] = { nullptr,nullptr,nullptr,nullptr }; //Data is handled in shader resource views

	public: //inits etc

		void Initialize(ID3D11Device*, ID3D11DeviceContext*);
		static LightHandler* GetInstance();

	public: //dataFlow
		LightArray* Get_Light_List(LIGHT_TYPE type) { return (type >= LIGHT_TYPE::NUM_LT ? nullptr : &m_lightData[type]); };
		bool UpdateStructuredBuffer (LIGHT_TYPE type);
		bool SetBuffersAsActive();
		bool SetLightData(Light* lightArray, unsigned int numLights, LIGHT_TYPE type);
	private:
		bool CreateStructuredBuffer (LIGHT_TYPE type);
		bool ReleaseStructuredBuffer(LIGHT_TYPE type);
		size_t GetStructByteSize    (LIGHT_TYPE type);
	};
}
#endif