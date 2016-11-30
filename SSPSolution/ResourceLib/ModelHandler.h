#ifndef RESOURCELIB_MODELHANDLER_MODELHANDLER_H
#define RESOURCELIB_MODELHANDLER_MODELHANDLER_H
#include "Resource.h"
#include "Model.h"
#include "MeshHandler.h"
#include "MaterialHandler.h"
namespace Resources
{

	class ModelHandler
	{
	private:
		std::unordered_map<unsigned int, ResourceContainer> m_models;
		std::deque<Model*> m_emptyContainers;
		std::vector<Model> m_containers;

		MeshHandler* m_meshHandler;
		MaterialHandler* m_materialHandler;
		ID3D11Device* m_device = nullptr;

		Model* placeHolderModel = nullptr;
		ModelHandler();
	public:
		Resources::Status GetModel( unsigned int& id, ResourceContainer *&modelPtr);
		Model* GetPlaceholderModel();
		ModelHandler(size_t modelAmount, ID3D11Device* device = nullptr);

		Resources::Status LoadModel(unsigned int& id, ResourceContainer*& modelPtr);
		Resources::Status UnloadModel(unsigned int& id);
		virtual ~ModelHandler();
	
		void SetDevice(ID3D11Device* device);
	private:
		bool CreatePlaceHolder();
	};

}

#endif

