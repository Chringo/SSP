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

namespace LIGHT
{
	class GRAPHICSDLL_API LightHandler
	{

	private:
		LightHandler();
		~LightHandler();

		std::vector<LIGHT::Light*> m_LightVector;

		ID3D11Device*			  m_gDevice;
		ID3D11DeviceContext*	  m_gDeviceContext;
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