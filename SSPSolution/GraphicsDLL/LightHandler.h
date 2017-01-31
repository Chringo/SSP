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

	class GRAPHICSDLL_API LightHandler
	{

	private:
		unsigned int m_LightID;
		std::vector<LIGHT::Light*> m_LightVector;
	public:
		LightHandler();
		~LightHandler();

		void Initialize();
		void Get_Light(unsigned int);
		void Add_Light(unsigned int id, LightColor col, LightFalloff fall, Light light, LIGHT_TYPE lt);
		void Remove_Light(unsigned int);
	};
#endif