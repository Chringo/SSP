#ifndef RESOURCELIB_RESOURCEHANDLER_RESOURCEHANDLER_H
#define RESOURCELIB_RESOURCEHANDLER_RESOURCEHANDLER_H
#include "DefineHeader.h"
#include "ModelHandler.h"
#include "FileLoader.h"

namespace Resources
{

	class DLL_OPERATION ResourceHandler
	{
		

	private:
		
		ModelHandler* m_modelHandler   = nullptr;

		ID3D11Device* m_device		   = nullptr;
		ID3D11DeviceContext* m_context = nullptr;

		unsigned int loadedLevel = 0;
	public:
		ResourceHandler();
		ResourceHandler(ID3D11Device* device, ID3D11DeviceContext* context);
		virtual ~ResourceHandler();

		Resources::Status LoadLevel(unsigned int id);


		/* Set */
		void SetDeviceAndContext(ID3D11Device* device, ID3D11DeviceContext* context);
		void SetDevice(ID3D11Device* device);
		void SetContext(ID3D11DeviceContext* context);
		/* Get */
		ID3D11Device* GetDevice()			   const { return this->m_device; };
		ID3D11DeviceContext* GetContext()      const { return this->m_context; };
	
		Resources::Status  GetModel(unsigned int id, Model*& modelPtr) const;


		Resources::Status UnloadLevel(unsigned int& id); //this will be private later (public for tests)
	private:
	};
}

#endif