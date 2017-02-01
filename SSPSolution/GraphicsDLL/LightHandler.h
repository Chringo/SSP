
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

namespace LIGHTING
{
	class GRAPHICSDLL_API LightHandler
	{
	private:
		enum LIGHT_BUFFER_SLOTS // Determines the slots that the buffers are set in the shader
		{
			POINTLIGHT_BUFFER		 = 5, // IMPORTANT: In the shader, these buffers needs to be registered as a t buffer
			DIRECTIONALLIGHT_BUFFER	 = 6, // not register(sX); BUT, register(tX); 
			AREALIGHT_BUFFER		 = 7,
			SPOTLIGHT_BUFFER		 = 8
		};
		enum MAX_LIGHTS {				//The max amount of any light type. Needed for the buffers.
			MAX_POINTLIGHTS = 15,		//Can be changed without problem
			MAX_DIRECTIONAL = 2,
			MAX_AREALIGHT	= 11,
			MAX_SPOTLIGHT	= 10
		};
		const int MAX_NUM_LIGHTS[NUM_LT]	  = { MAX_POINTLIGHTS,   MAX_DIRECTIONAL,          MAX_AREALIGHT,     MAX_SPOTLIGHT };
		const int BUFFER_SHADER_SLOTS[NUM_LT] = { POINTLIGHT_BUFFER, DIRECTIONALLIGHT_BUFFER,  AREALIGHT_BUFFER,  SPOTLIGHT_BUFFER };
	private:
		LightHandler();
		~LightHandler();

		std::vector<LIGHTING::Light*> m_LightVector;

		ID3D11Device*			  m_gDevice;
		ID3D11DeviceContext*	  m_gDeviceContext;

		ID3D11Buffer* lightBuffers[NUM_LT]					   = { nullptr,nullptr,nullptr,nullptr }; //Light constBuffers
		ID3D11ShaderResourceView*  m_structuredBuffers[NUM_LT] = { nullptr,nullptr,nullptr,nullptr }; //Data is handled in shader resource views

	public: //inits etc

		void Initialize(ID3D11Device*, ID3D11DeviceContext*);
		static LightHandler* GetInstance();

	public: //dataFlow
		std::vector<LIGHTING::Light*>* Get_Light_List() { return &this->m_LightVector; };
		LIGHTING::Light* Get_Light(unsigned int id);
		void Add_Light(LIGHTING::Light* light);
		void Remove_Light(unsigned int id);

	private:
		bool CreateStructuredBuffer(LIGHT_TYPE type);
		bool ReleaseStructuredBuffer(LIGHT_TYPE type);
	};
}
#endif