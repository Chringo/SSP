
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


*/
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
		unsigned int NUM_LIGHTS[NUM_LT] = { 0, 0, 0, 0 };
		const unsigned int BUFFER_SHADER_SLOTS[NUM_LT] = { POINTLIGHT_BUFFER, DIRECTIONALLIGHT_BUFFER,  AREALIGHT_BUFFER,  SPOTLIGHT_BUFFER };
	

	public:
		struct LightArray {
			Light* dataPtr = nullptr;
			ID3D11ShaderResourceView* shadowMaps; //One should be generated for each light on load
			unsigned int numItems = 0;

			~LightArray() { //Destructor, 
				ReleaseShadowMaps(); //Release the TextureBuffers
			}
			void ReleaseShadowMaps() {
					if (shadowMaps != nullptr)
					{
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
		LightArray				  m_lightData[NUM_LT];
		ID3D11Device*			  m_gDevice;
		ID3D11DeviceContext*	  m_gDeviceContext;

		ID3D11Buffer* m_lightBuffers[NUM_LT]					    = { nullptr,nullptr,nullptr,nullptr }; //Light constBuffers
		ID3D11ShaderResourceView*  m_structuredBuffers[NUM_LT]		= { nullptr,nullptr,nullptr,nullptr }; //Data is handled in shader resource views

	private:
		GRAPHICSDLL_API bool CreateStructuredBuffer (LIGHT_TYPE type,int amount);
		GRAPHICSDLL_API bool ReleaseStructuredBuffer(LIGHT_TYPE type);
		GRAPHICSDLL_API size_t GetStructByteSize    (LIGHT_TYPE type);
	public: //inits etc

		GRAPHICSDLL_API void Initialize(ID3D11Device*, ID3D11DeviceContext*);
		GRAPHICSDLL_API static LightHandler* GetInstance();

	public: //dataFlow
		GRAPHICSDLL_API LightArray* Get_Light_List(LIGHT_TYPE type) { return (type >= LIGHT_TYPE::NUM_LT ? nullptr : &m_lightData[type]); };
		GRAPHICSDLL_API bool UpdateStructuredBuffer (LIGHT_TYPE type);
		GRAPHICSDLL_API bool SetStaticShadowsToGPU();
		GRAPHICSDLL_API bool SetBuffersAsActive();
		GRAPHICSDLL_API bool SetLightData(Light* lightArray, unsigned int numLights, LIGHT_TYPE type);
		GRAPHICSDLL_API void SetAmbientLight(float r, float g, float b, float intensity);
		GRAPHICSDLL_API bool LoadLevelLight(LevelData::Level* level);
		GRAPHICSDLL_API bool SetShadowCastingLight(Light* light);
		GRAPHICSDLL_API bool SetShadowCastingLight(int index);

		

		//Returns either an index to the internal lightdata or -1 for no lights found
		GRAPHICSDLL_API int GetClosestLightIndex(LIGHT_TYPE type, DirectX::XMFLOAT3 pos);
		//Stores either an index to the internal lightdata or -1 for no lights found
		GRAPHICSDLL_API void GetClosestLightIndex(LIGHT_TYPE type, DirectX::XMFLOAT3 pos, int & storeIn);
	};
}
#endif