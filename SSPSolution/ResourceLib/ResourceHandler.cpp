#include "ResourceHandler.h"


#ifdef  _DEBUG


void Resources::ResourceHandler::ResetQueryCounter()
{
	queriesPerFrame = 0;
}
#endif //  _DEBUG

Resources::ResourceHandler::ResourceHandler()
{

	this->m_modelHandler = new ModelHandler(50);
	
}

Resources::ResourceHandler::ResourceHandler(ID3D11Device * device, ID3D11DeviceContext * context)
{
	this->m_device = device;
	this->m_context = context;
	this->m_modelHandler = new ModelHandler(50);
	m_modelHandler->SetDevice(device);


}


Resources::ResourceHandler::~ResourceHandler()
{

	delete m_modelHandler;
	delete m_CurrentLevel;
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


	/* T e s t */
	FileLoader* fileLoader = Resources::FileLoader::GetInstance();
	if (!fileLoader->OpenFile(Resources::FileLoader::Files::BPF_FILE))
		std::cout << "Could not open resource file"<<std::endl;
		//return ST_ERROR_OPENING_FILE;

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
	if(m_CurrentLevel != nullptr)
		UnloadLevel(m_CurrentLevel); //Unload the previous level
	//m_CurrentLevel = ne;
	fileLoader->CloseFile(Resources::FileLoader::Files::BPF_FILE);
#ifdef _DEBUG
	this->ResetQueryCounter();
#endif // _DEBUG
	return Resources::Status::ST_OK;
}

Resources::Status Resources::ResourceHandler::LoadLevel(LevelData::ResourceHeader * levelResources, unsigned int numResources)
{
	if (m_device == nullptr) {
		std::cout << "No device is set. Cannot load resources" << std::endl;
		return Status::ST_DEVICE_MISSING;
	}
	/*
	- Load level information,
	- Load all models that are in the level.
	- Construct the models, load resources if needed and add a reference counter to all the resources used in the model

	- Unload the last level, decrement the reference counterof all the resources.
	- if a reference counter hits 0, unload the resource
	*/
	
	if (m_CurrentLevel != nullptr) {
		UnloadLevel(m_CurrentLevel); //Unload the previous level
	}

	FileLoader* fileLoader = Resources::FileLoader::GetInstance();
	if (!fileLoader->OpenFile(Resources::FileLoader::Files::BPF_FILE))
	{
		std::cout << "Could not open BPF file" << std::endl;
		return ST_ERROR_OPENING_FILE;
	}

	LevelResources* newLevel = new LevelResources;
	newLevel->ids = new unsigned int[numResources];
	newLevel->numResources = numResources;
	m_CurrentLevel = newLevel;
	

	// for each model in level
	Status st;
	for (size_t i = 0; i < numResources; i++)
	{
		//Get id of the model 
		unsigned int id = levelResources[i].id;
		newLevel->ids[i] = id;

		ResourceContainer* modelPtr = nullptr;
		
		st = m_modelHandler->GetModel(id, modelPtr);
		switch (st)
		{
		case Resources::Status::ST_RES_MISSING:
		{
#ifdef _DEBUG
			//std::cout << "Model not loaded, loading" << std::endl;
#endif // _DEBUG
			//Load the model
			Status modelSt = m_modelHandler->LoadModel(id, modelPtr); //if this fails, placeholder will take the place

#ifdef _DEBUG
			if (modelSt != ST_OK) {

				std::cout << "Model not found in BPF, ID: " << id << std::endl;
			}
#endif // _DEBUG
			break;
		}
		case Resources::Status::ST_OK:
			modelPtr->refCount += 1; //Add the reference count
			break;
		}
	}
	
	fileLoader->CloseFile(Resources::FileLoader::Files::BPF_FILE);

	
	this->ClearUnusedMemory();
#ifdef _DEBUG
	this->ResetQueryCounter();
#endif // _DEBUG
	return Resources::Status::ST_OK;
}

Resources::Status Resources::ResourceHandler::UnloadCurrentLevel()
{
	Resources::Status status = ST_OK;
	if (m_CurrentLevel != nullptr) {

		UnloadLevel(m_CurrentLevel); //Unload the previous level
		delete m_CurrentLevel;
		m_CurrentLevel = nullptr;
	}
	else
		status = Status::ST_RES_MISSING;
	return  status;
}
Resources::Status Resources::ResourceHandler::ClearUnusedMemory()
{
	return  m_modelHandler->ClearUnusedMemory();
}
Resources::ResourceHandler * Resources::ResourceHandler::GetInstance()
{
	static ResourceHandler instance;
	return &instance;
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


Resources::Status Resources::ResourceHandler::GetModel(unsigned int id, Model*& modelPtr) 
{
	ResourceContainer* modelCont = nullptr;
	Status st = m_modelHandler->GetModel(id, modelCont);

#ifdef _DEBUG
	this->queriesPerFrame += 1;
#endif // _DEBUG


	switch (st)
	{
	case Status::ST_OK:
		modelPtr = (Model*)modelCont->resource;
		break;
	case Status::ST_RES_MISSING:
		/*return placeholder MODEL*/
		modelPtr = m_modelHandler->GetPlaceholderModel();
		return  Resources::Status::ST_RES_MISSING;
		break;
	default:
		return st;
	}

	return  Resources::Status::ST_OK;
}

Resources::Status Resources::ResourceHandler::UnloadLevel(LevelResources* levelRes)
{

	//for each model in level
	//get id of model and unload id
	Status st;
	for (size_t i = 0; i < levelRes->numResources; i++)
	{
		st = m_modelHandler->UnloadModel(levelRes->ids[i]);
#ifdef _DEBUG
		if (st != ST_OK)
		{
			MessageBox(NULL, TEXT("Error in unloading model"), TEXT("ERROR"), MB_OK);
			//return st;
		}
#endif // _DEBUG
	}
	


	return Resources::Status::ST_OK;
}

