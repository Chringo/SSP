#ifndef RESOURCELIB_MODELHANDLER_MODELHANDLER_H
#define RESOURCELIB_MODELHANDLER_MODELHANDLER_H
#include "Resource.h"
#include "Model.h"
#include "MeshHandler.h"
#include "MaterialHandler.h"
#include "SkeletonHandler.h"
namespace Resources
{
#pragma region Comment
	/*
		AUTHOR: Martin Clementson

		This class holds the models and handlers for the subdata of all models.

		A model is basically a container of pointers to raw data such as, material,mesh,skeleton
		GetModel is used to get a specific model 
		This class was optimized for the game. It pre allocated memory for a certain amount
		of models. Then it never deallocate them during runtime. It keeps track of available containers.
		When  a model is unloaded its buffers are released and the model object is put into "emptyContainers" which
		means that its ready to be used with new data.
	*/
#pragma endregion
	class ModelHandler
	{
	private:
		std::unordered_map<unsigned int, ResourceContainer> m_models;
		std::deque<Model*> m_emptyContainers;
		std::vector<Model> m_containers;

		MeshHandler*		m_meshHandler;
		MaterialHandler*	m_materialHandler;
		SkeletonHandler*	m_skeletonHandler;
		ID3D11Device*		m_device = nullptr;

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

