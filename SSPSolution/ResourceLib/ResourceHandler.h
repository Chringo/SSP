#ifndef RESOURCELIB_RESOURCEHANDLER_RESOURCEHANDLER_H
#define RESOURCELIB_RESOURCEHANDLER_RESOURCEHANDLER_H
#include "DefineHeader.h"
#include "ModelHandler.h"
#include "FileLoader.h"

#include "../SSP_Editor/LevelHeaders.h"

namespace Resources
{

	class DLL_OPERATION ResourceHandler
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
		virtual ~ResourceHandler();

		Resources::Status LoadLevel(unsigned int id);
		Resources::Status LoadLevel(LevelData::ResourceHeader* levelResources, unsigned int numResources); 

		static ResourceHandler* GetInstance();
		/* Set */
		void SetDeviceAndContext(ID3D11Device* device, ID3D11DeviceContext* context);
		void SetDevice(ID3D11Device* device);
		void SetContext(ID3D11DeviceContext* context);
		/* Get */
		ID3D11Device* GetDevice()			   const { return this->m_device; };
		ID3D11DeviceContext* GetContext()      const { return this->m_context; };
	
		Resources::Status  GetModel(unsigned int id, Model*& modelPtr) const;


	private:
		Resources::Status UnloadLevel(LevelResources* levelRes); 

#ifdef _DEBUG
	private:
		int queriesPerFrame = 0;
	public:
		void ResetQueryCounter();
		int GetQueryCounter() { return queriesPerFrame; };
#endif // _DEBUG
	};
}

#endif