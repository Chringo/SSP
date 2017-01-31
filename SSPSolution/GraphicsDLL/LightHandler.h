#pragma once
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


//Dont forget to clench your butthole so no heresy gets in!
namespace LIGHT
{
	class GRAPHICSDLL_API LightHandler
	{
	private:
		static const int MAX_NUM_POINTLIGHTS = 10;
	private:
		LightHandler();
		~LightHandler();

		std::vector<LIGHT::Light*> m_LightVector;

		ID3D11Device*			  m_gDevice;
		ID3D11DeviceContext*	  m_gDeviceContext;

		ID3D11Buffer* lightBuffers[NUM_LT] = { nullptr }; //Light constBuffers
		//PointLightStruct*    pointLightStruct = nullptr;
		//SpotLightStruct*     spotLightStruct = nullptr;
		//DirLightStruct*	      dirLightStruct = nullptr;
		ID3D11ShaderResourceView*  pointLightStructuredBuffer = nullptr;
		ID3D11ShaderResourceView*  spotLightStructuredBuffer  = nullptr;
		ID3D11ShaderResourceView*  dirLightStructuredBuffer   = nullptr;

	public: //inits etc

		void Initialize(ID3D11Device*, ID3D11DeviceContext*);
		static LightHandler* getInstance();

	public: //dataFlow
		std::vector<LIGHT::Light*>* Get_Light_List() { return &this->m_LightVector; };
		LIGHT::Light* Get_Light(unsigned int id);
		void Add_Light(LIGHT::Light* light);
		void Remove_Light(unsigned int id);
	};
}
#endif