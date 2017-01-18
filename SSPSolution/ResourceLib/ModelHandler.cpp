#include "ModelHandler.h"



Resources::ModelHandler::ModelHandler(size_t modelAmount, ID3D11Device* device )
{
	this->m_emptyContainers.resize(modelAmount);
	
	this->m_models.reserve(modelAmount);
	this->m_containers.reserve(modelAmount);
	this->m_containers.insert(m_containers.begin(), modelAmount, Model());
	for (size_t i = 0; i < modelAmount; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(i);
	}
	
	this->m_meshHandler		=  new MeshHandler(modelAmount);
	this->m_materialHandler =  new MaterialHandler(modelAmount);
	this->m_skeletonHandler =  new SkeletonHandler(modelAmount);
	if (device != nullptr) {
		this->m_device = device;
		m_meshHandler->SetDevice(device);
		m_materialHandler->SetDevice(device);
		m_skeletonHandler->SetDevice(device);
	}


	
}

Resources::ModelHandler::ModelHandler()
{

}

Resources::Status Resources::ModelHandler::GetModel( unsigned int & id, ResourceContainer *& modelPtr)
{


	std::unordered_map<unsigned int, ResourceContainer>::iterator got = m_models.find(id);
	if (got == m_models.end()){ // if the model does not exists in memory
		
		return Resources::Status::ST_RES_MISSING;
	}
	else{
		modelPtr = &got->second;
	}

	return Resources::Status::ST_OK;
}

Resources::Model * Resources::ModelHandler::GetPlaceholderModel()
{
	if (placeHolderModel == nullptr)
	{
		this->CreatePlaceHolder();
	}
	return placeHolderModel;
}


Resources::Status Resources::ModelHandler::LoadModel(unsigned int& id, ResourceContainer*& modelPtr)
{
	char* data	= nullptr;
	size_t size = 0;
	Resources::Status st = Status::ST_OK;

	st = FileLoader::GetInstance()->LoadResource(id, data, &size); //Get the raw data from file
	
	if (st != ST_OK)
		return st;
											    //additional headers could be added here,
	Model* newModel = GetEmptyContainer();		//Get an empty container
	Resource::RawResourceData* resData = (Resource::RawResourceData*)data;
	if (resData->m_resType != RES_MODEL)
	{
		#ifdef _DEBUG
			std::cout << "Wrong resource type. Wanted Model, got type: " << resData->m_resType << std::endl;
		#endif // _DEBUG
		return ST_WRONG_RESTYPE;
	}
	Resources::Model::RawModelData *raw_model_Data = (Resources::Model::RawModelData*)(data + sizeof(Resource::RawResourceData));

	newModel->Create(resData,raw_model_Data);				 //Initialize it with data
	m_models[resData->m_id] = ResourceContainer(newModel,1); //Put it into the map
 	m_emptyContainers.pop_front();							 //remove from empty container queue
	
	unsigned int meshID = raw_model_Data->meshId;
	unsigned int matID  = raw_model_Data->materialId;
	unsigned int skelID = raw_model_Data->skeletonId;

	Resources::ResourceContainer* meshPtr = nullptr;
#pragma region Load Mesh
	st = m_meshHandler->GetMesh(meshID, meshPtr); //Get the mesh

	switch (st)
	{
		case Status::ST_RES_MISSING: //if it doesent exist
		{		
			 Status mSt= m_meshHandler->LoadMesh(meshID,meshPtr); //load the mesh
			 if (mSt != ST_OK){
				newModel->SetMesh( m_meshHandler->GetPlaceHolderMesh());
			 }
			 else
				newModel->SetMesh((Mesh*)meshPtr->resource);
			 break;
		}
		case Status::ST_OK:
		{
			meshPtr->refCount += 1;
			newModel->SetMesh((Mesh*)meshPtr->resource);
			break;
		}
		default:
			return st;
	}
#pragma endregion

		
#pragma region Load Material

		ResourceContainer* matPtr;
		
		st = m_materialHandler->GetMaterial(matID, matPtr); // Get Material
		switch (st)
		{
		case Status::ST_RES_MISSING:
		{
			Status mSt = m_materialHandler->LoadMaterial(matID, matPtr);
			if (mSt != ST_OK)
				newModel->SetMaterial(m_materialHandler->GetPlaceHolderMaterial());
			else
				newModel->SetMaterial((Material*)matPtr->resource);
			break;
		}
		case Status::ST_OK:
		{
			matPtr->refCount += 1;
			newModel->SetMaterial((Material*)matPtr->resource);
			break;
		}
		default:
			return st;
		}

		//newModel->SetMaterial(m_materialHandler->GetPlaceHolderMaterial());
#pragma endregion

		
#pragma region Load Skeleton

		if(skelID != 0){
			Resources::ResourceContainer* skeletonPtr = nullptr;
			 st = m_skeletonHandler->GetSkeleton(skelID, skeletonPtr);
			switch (st){
				case Status::ST_RES_MISSING:{ //if it doesent exist
					Status mSt= m_skeletonHandler->LoadSkeleton(skelID,skeletonPtr); //load the skeleton
					if (mSt != ST_OK){
					 //Panic
					}
					else
					newModel->SetSkeleton((Skeleton*)skeletonPtr->resource);
					break;
				}
				case Status::ST_OK:{
					skeletonPtr->refCount += 1;
					newModel->SetSkeleton((Skeleton*)skeletonPtr->resource);
					break;
				}
				default:
					return st;
		}
	}

#pragma endregion
	
	return Resources::Status::ST_OK;
}


Resources::Status Resources::ModelHandler::UnloadModel(unsigned int & id)
{
	ResourceContainer* modelRes;
	Status st = GetModel(id, modelRes);
	switch (st)
	{
		case ST_OK:
		{
			modelRes->refCount -= 1;
			if (modelRes->refCount <= 0)
			{
				Model*    mod  = (Model*)modelRes->resource;
				Mesh*     mesh = mod->GetMesh();
				Material* mat  = mod->GetMaterial();
				Skeleton* skel = mod->GetSkeleton();
				if(mesh != nullptr)
					m_meshHandler->UnloadMesh(mesh->GetId());
				if(mat  != nullptr)
					m_materialHandler->UnloadMaterial(mat->GetId());
				if(skel != nullptr)
					m_skeletonHandler->UnloadSkeleton(skel->GetId());
				mod->Destroy();
				m_emptyContainers.push_back(mod);
				m_models.erase(id);
#ifdef _DEBUG
				std::cout << "Model : " << id << ". Has been Unloaded" << std::endl;
#endif // _DEBUG
			}
			break;
		}
		case ST_RES_MISSING:
			std::cout << "Could not find the model to unload| ID :" << id << std::endl;
	}
	return Resources::Status::ST_OK;
}

Resources::ModelHandler::~ModelHandler()
{
	delete m_meshHandler;
	delete m_materialHandler;
	delete m_skeletonHandler;
	delete placeHolderModel;
}

void Resources::ModelHandler::SetDevice(ID3D11Device * device)
{
 m_device = device; 
 m_meshHandler->SetDevice(device);
 m_materialHandler->SetDevice(device);
 m_skeletonHandler->SetDevice(device);
}

bool Resources::ModelHandler::CreatePlaceHolder()
{
	if (m_device != nullptr){
		delete placeHolderModel;
		this->placeHolderModel = new Model();
		Resource::RawResourceData data;
		data.m_id = -11;
		data.m_resType = RES_MODEL;
		placeHolderModel->Create(&data);
		Mesh*     placeMesh = m_meshHandler->GetPlaceHolderMesh();
		Material* placeMat  = m_materialHandler->GetPlaceHolderMaterial();

		ResourceContainer* skel;
		m_skeletonHandler->LoadSkeleton(0, skel);

		placeHolderModel->SetMesh(placeMesh);
		placeHolderModel->SetSkeleton((Skeleton*)skel->resource);
		placeHolderModel->SetMaterial(placeMat);
		return true;
	}
	
#ifdef _DEBUG
	std::cout << "No Device set, cannot create place holder" << std::endl;
#endif // _DEBUG

	return false;
}

Resources::Model * Resources::ModelHandler::GetEmptyContainer()
{
	if (m_emptyContainers.size() < 1)
	{
		m_containers.push_back(Model());
		m_emptyContainers.push_back(m_containers.end()._Ptr);
	}
	return m_emptyContainers.front();
}
