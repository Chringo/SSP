#ifndef RESOURCELIB_RESOURCEHANDLER_RESOURCEHANDLER_H
#define RESOURCELIB_RESOURCEHANDLER_RESOURCEHANDLER_H
#include "DefineHeader.h"
#include "ModelHandler.h"
#include "FileLoader.h"

#include "../SSP_Editor/LevelHeaders.h"

namespace Resources
{

	class  ResourceHandler
	{

	


	private:
		struct LevelResources {
			unsigned int *ids = nullptr;
			unsigned int numResources;
			LevelResources() {};
			~LevelResources(){
				delete ids; 
				ids = nullptr;
				numResources = 0;
			}
		};
		ModelHandler* m_modelHandler   = nullptr;
		ID3D11Device* m_device		   = nullptr;
		ID3D11DeviceContext* m_context = nullptr;

		LevelResources* m_CurrentLevel = nullptr;
		ResourceHandler();
		ResourceHandler(ID3D11Device* device, ID3D11DeviceContext* context);
	public:
		DLL_OPERATION virtual ~ResourceHandler();

		DLL_OPERATION Resources::Status LoadLevel(unsigned int id);
		DLL_OPERATION Resources::Status LoadLevel(LevelData::ResourceHeader* levelResources, unsigned int numResources); 

	DLL_OPERATION static ResourceHandler* GetInstance();
	/* Set */
	DLL_OPERATION void SetDeviceAndContext(ID3D11Device* device, ID3D11DeviceContext* context);
	DLL_OPERATION void SetDevice(ID3D11Device* device);
	DLL_OPERATION void SetContext(ID3D11DeviceContext* context);
	/* Get */
	DLL_OPERATION ID3D11Device* GetDevice()			   const { return this->m_device; };
	DLL_OPERATION ID3D11DeviceContext* GetContext()      const { return this->m_context; };

	DLL_OPERATION Resources::Status  GetModel(unsigned int id, Model*& modelPtr) ;


	private:
		Resources::Status UnloadLevel(LevelResources* levelRes); 

#ifdef _DEBUG
	private:
		int queriesPerFrame = 0;
	public:
		DLL_OPERATION void ResetQueryCounter();
		DLL_OPERATION int GetQueryCounter() { return queriesPerFrame; };
#endif // _DEBUG
	};
}

#endif