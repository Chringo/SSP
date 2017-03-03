
#ifndef GRAPHICSDLL_LIGHTHANDLER_H
#define GRAPHICSDLL_LIGHTHANDLER_H
#include <DirectXMath.h>

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif
#include <d3d11_1.h>
#include <vector>
#include "LightStructs.h"
#include "ConstantBufferHandler.h"
#include "../SSP_Editor/LevelHeaders.h"
#ifdef _DEBUG
#include <iostream>
#endif
/*
	Author:Martin Clementson

	This class handles the interaction of lights and the shaders

	This assumes that the individual light data is stored elsewhere.
	It holds pointers to arrays of each individual light type.
	If an update to the gpu is desired. Call UpdateStructuredBuffer()



	Edit: 21/2 2017

	The project decided that we should use cubemapping for shadows to point lights.
	This changed the const buffer, the depth stencils and textures
	When it comes to holding the data, this class holds the data in the game, however in the editor the light data is still held elsewhere

	Edit: 27/2 2017

	The class has been reworked to be optimized. The other types of lights has been removed to save ram and vram, Some remnants might be found in the future

*/
#define LIGHT_CHECK_PAUSE_TIME (1.0f / 10.0f)

//#define CHECK_IF_EXITED_LIGHT_RADIUS
namespace LIGHTING
{
	class LightHandler
	{
	private:
		enum LIGHT_BUFFER_SLOTS // Determines the slots that the buffers are set in the shader
		{
			POINTLIGHT_BUFFER		 = 6 , // IMPORTANT: In the shader, these buffers needs to be registered as a t buffer
			DIRECTIONALLIGHT_BUFFER	 = 7 , // not register(sX); BUT, register(tX); 
			AREALIGHT_BUFFER		 = 8,
			SPOTLIGHT_BUFFER		 = 9
		};
		 unsigned int NUM_LIGHTS = 0;
		 const unsigned int BUFFER_SHADER_SLOT = 6;
		

	public:
		struct LightArray {
			Point* dataPtr = nullptr;
			ID3D11ShaderResourceView* shadowMaps; //One should be generated for each light on load
			unsigned int numItems		 = 0;
			unsigned int numShadowLights = 0;
			int shadowLightIndex[MAX_SHADOW_LIGHTS]; // An array of int that represents the indices of the lights that casts shadows
			~LightArray() { //Destructor, s
				ReleaseShadowMaps(); //Release the TextureBuffers
			}
			void ReleaseShadowMaps() {
					if (shadowMaps != nullptr){
						shadowMaps->Release();
						shadowMaps = nullptr;
					}
			}
		};
		
	private:
		LightHandler();
		~LightHandler();
		ConstantBufferHandler::ConstantBuffer::shadow::cbData m_shadowCb;
		ConstantBufferHandler::ConstantBuffer::light::pData m_constBufferData;
		LightArray				  m_lightData;
		ID3D11Device*			  m_gDevice;
		ID3D11DeviceContext*	  m_gDeviceContext;

		ID3D11Buffer* m_lightBuffer = nullptr; //Light constBuffer
		ID3D11ShaderResourceView*  m_structuredBuffer = nullptr; //Data is handled in shader resource views

		//Timer variables
		//Time since we last updated the light for shadow mapping
		float m_activeLightCheckTimer;
		//The light index for shadow mapping
		int m_activeLightIndex;

	private:
		GRAPHICSDLL_API bool CreateStructuredBuffer (int amount);
		GRAPHICSDLL_API bool ReleaseStructuredBuffer();
		GRAPHICSDLL_API size_t GetStructByteSize    (LIGHT_TYPE type);

	public: //inits etc
		GRAPHICSDLL_API void Initialize(ID3D11Device*, ID3D11DeviceContext*);
		GRAPHICSDLL_API static LightHandler* GetInstance();
		GRAPHICSDLL_API	int Update(float dT, DirectX::XMFLOAT3 pointOfInterest);

	public: //dataFlow
		GRAPHICSDLL_API LightArray* Get_Light_List() { return  &m_lightData; };
		GRAPHICSDLL_API bool UpdateStructuredBuffer ();
		GRAPHICSDLL_API bool SetStaticShadowsToGPU();
		GRAPHICSDLL_API bool SetBufferAsActive();
		GRAPHICSDLL_API bool SetLightData(Point* lightArray, unsigned int numLights);
		GRAPHICSDLL_API void SetAmbientLight(float r, float g, float b, float intensity);
		GRAPHICSDLL_API bool LoadLevelLight(LevelData::Level* level);
		GRAPHICSDLL_API bool SetShadowCastingLight(Point* light);
		GRAPHICSDLL_API bool SetShadowCastingLight(int index);

		//Returns either an index to the internal lightdata or -1 for no lights found
		GRAPHICSDLL_API int GetClosestLightIndex( DirectX::XMFLOAT3 pos);
		//Stores either an index to the internal lightdata or -1 for no lights found
		GRAPHICSDLL_API void GetClosestLightIndex( DirectX::XMFLOAT3 pos, int & storeIn);
	};
}
#endif