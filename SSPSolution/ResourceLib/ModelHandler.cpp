#include "ModelHandler.h"



Resources::ModelHandler::ModelHandler(size_t modelAmount, ID3D11Device* device )
{
	this->m_emptyContainers.resize(modelAmount);
	
	this->m_models.reserve(modelAmount);

	this->m_containers.push_back(new std::vector<Model>);
	
	this->m_containers.at(0)->insert(m_containers.at(0)->begin(), modelAmount, Model());
	for (size_t i = 0; i < modelAmount; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(0)->at(i);
	}
	
	this->m_meshHandler		=  new MeshHandler(modelAmount);
	this->m_materialHandler =  new MaterialHandler(modelAmount);
	this->m_skeletonHandler =  new SkeletonHandler(2); // start with a lower number on skeletons to save memory
	if (device != nullptr) {
		this->m_device = device;
		m_meshHandler->SetDevice(device);
		m_materialHandler->SetDevice(device);
		m_skeletonHandler->SetDevice(device);
	}

	
}

Resources::Status Resources::ModelHandler::ClearUnusedMemory()
{
	m_emptyContainers.shrink_to_fit();

	for (size_t i = 0; i < m_containers.size(); i++)
	{
		m_containers.at(i)->shrink_to_fit();
	}
	m_containers.shrink_to_fit();

	Resources::Status st = Resources::Status::ST_OK;
		st = m_meshHandler	  ->ClearUnusedMemory();
		if (st != ST_OK)
			return Status::ST_BUFFER_ERROR;
		st = m_materialHandler->ClearUnusedMemory();
		if (st != ST_OK)
			return Status::ST_BUFFER_ERROR;
		st = m_skeletonHandler->ClearUnusedMemory();
		if (st != ST_OK)
			return Status::ST_BUFFER_ERROR;

	return st;
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
#ifdef _DEBUG
				std::cout << "Could not load mesh, Using placeholder for model ID : " << id << std::endl;
#endif // _DEBUG

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
			if (mSt != ST_OK) {

				newModel->SetMaterial(m_materialHandler->GetPlaceHolderMaterial());
#ifdef _DEBUG
				std::cout << "Could not load Material, Using placeholder for model ID : " << id << std::endl;
#endif // _DEBUG
			}
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
				if (mat != nullptr) {
					Status matStat = m_materialHandler->UnloadMaterial(mat->GetId());
					if (matStat != ST_OK) {
#ifdef _DEBUG
						std::cout << "No material with id : " << mat->GetId() << " to unload, Was it using placeholder material? \n" << "Model ID: " << id << std::endl;
#endif // _DEBUG
					}
				}
				if(skel != nullptr)
					m_skeletonHandler->UnloadSkeleton(skel->GetId());
				mod->Destroy();
				m_emptyContainers.push_back(mod);
				m_models.erase(id);
#ifdef _DEBUG
				std::cout << "Model : " << id << ". Has been Unloaded" << std::endl;
#endif //_DEBUG
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
	for (size_t i = 0; i < m_containers.size(); i++)
	{
		delete m_containers.at(i);
	}
}

void Resources::ModelHandler::SetDevice(ID3D11Device * device)
{
 m_device = device; 
 m_meshHandler->SetDevice(device);
 m_materialHandler->SetDevice(device);
 m_skeletonHandler->SetDevice(device);

 ResourceContainer* temp;
 //unsigned int one = 1117267500; // player one
 unsigned int one = 885141774;
 //unsigned int two = 885141774; //player two
 unsigned int two = 1117267500;
 unsigned int three = 1680427216; // chain
 unsigned int four = 1321651915; // ball
 unsigned int five = 2539810394; // ping
 unsigned int six = 1256673809; // ball2
 FileLoader::GetInstance()->OpenFile(FileLoader::BPF_FILE);
 LoadModel(one, temp);
 LoadModel(two, temp);
 LoadModel(three, temp);
 LoadModel(four, temp);
 LoadModel(five, temp);
 LoadModel(six, temp);
 FileLoader::GetInstance()->CloseFile(FileLoader::BPF_FILE);
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
		placeHolderModel->SetMesh(placeMesh);
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
		m_containers.push_back(new std::vector<Model>(20));
		for (size_t i = 0; i < 20; i++)
		{
			m_emptyContainers.push_back(&m_containers.at(m_containers.size() - 1)->at(i));
		}
	}
	return m_emptyContainers.front();
}
