#include "ResourceHandler.h"



Resources::ResourceHandler::ResourceHandler()
{

	this->m_modelHandler = new ModelHandler(20);
	
}

Resources::ResourceHandler::ResourceHandler(ID3D11Device * device, ID3D11DeviceContext * context)
{
	this->m_device = device;
	this->m_context = context;
	this->m_modelHandler = new ModelHandler(20);
	m_modelHandler->SetDevice(device);
}


Resources::ResourceHandler::~ResourceHandler()
{

	delete m_modelHandler;
}

Resources::Status Resources::ResourceHandler::LoadLevel(unsigned int id)
{
	if (m_device == nullptr){
		std::cout << "No device is set. Cannot load resources" << std::endl;
		return Status::ST_DEVICE_MISSING;
	}
	/*
		- Load level information,
		- Load all models that are in the level.
		- Construct the models, load resources if needed and add a reference counter to all the resources used in the model

		- Unload the last level, decrement the reference counterof all the resources.
		- if a reference counter hits 0, unload the resource
	

		Alternative, 
		- Unload last level first.
		- Then load the level
		- Then loop through the resources and check ref counting
	*/

	if (loadedLevel == id)
	{
#ifdef _DEBUG
		std::cout << "Level already loaded" << std::endl;
#endif // _DEBUG
		return Resources::ST_RES_ALREADY_LOADED;	

	}

	/* T e s t */
	FileLoader* fileLoader = Resources::FileLoader::GetInstance();
	fileLoader->OpenFile(Resources::FileLoader::Files::RESOURCE_FILE);
	// for each model in level
	{
		//Get id of the model from the level Instructions
		unsigned int id = 1337;
		
		ResourceContainer* modelPtr = nullptr;
		Status st;

		st = m_modelHandler->GetModel(id, modelPtr);
		switch (st)
		{
			case Resources::Status::ST_RES_MISSING:
			{
#ifdef _DEBUG
				std::cout << "Model missing, loading" << std::endl;
#endif // _DEBUG

				//Load the model
				Status modelSt = m_modelHandler->LoadModel(id, modelPtr);
				//if this fails, placeholder will take the place
				break;

			}
			case Resources::Status::ST_OK:
				modelPtr->refCount += 1; //Add the reference count
				break;
		}

	}
	if(loadedLevel != 0)
		UnloadLevel(loadedLevel); //Unload the previous level
	loadedLevel = id;
	fileLoader->CloseFile(Resources::FileLoader::Files::RESOURCE_FILE);
	return Resources::Status::ST_OK;
}



void Resources::ResourceHandler::SetDeviceAndContext(ID3D11Device * device, ID3D11DeviceContext * context)
{
	this->m_device = device;
	this->m_context = context;
	m_modelHandler->SetDevice(device);
}

void Resources::ResourceHandler::SetDevice(ID3D11Device * device)
{
	this->m_device = device;
	m_modelHandler->SetDevice(device);
}

void Resources::ResourceHandler::SetContext(ID3D11DeviceContext * context)
{
	this->m_context = context;
}


Resources::Status Resources::ResourceHandler::GetModel(unsigned int id, Model*& modelPtr) const
{
	ResourceContainer* modelCont = nullptr;
	Status st = m_modelHandler->GetModel(id, modelCont);

	switch (st)
	{
	case Status::ST_OK:
		modelPtr = (Model*)modelCont->resource;
		break;
	case Status::ST_RES_MISSING:
		/*LOAD THE MODEL | Or return placeholder MODEL*/
		modelPtr = m_modelHandler->GetPlaceholderModel();
		break;
	default:
		return st;
	}

	return  Resources::Status::ST_OK;
}

Resources::Status Resources::ResourceHandler::UnloadLevel(unsigned int & id)
{

	//for each model in level
	//get id of model and unload id
	Status st = m_modelHandler->UnloadModel(id);
	if (st != ST_OK)
		return st;

	return Resources::Status::ST_OK;
}

