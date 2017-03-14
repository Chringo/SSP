#include "MaterialHandler.h"



Resources::MaterialHandler::MaterialHandler()
{
}

Resources::MaterialHandler::MaterialHandler(size_t materialAmount, ID3D11Device* device)
{
	this->m_emptyContainers.resize(materialAmount);

	this->m_materials.reserve(materialAmount);
	this->m_containers.push_back(new std::vector<Material>);
	this->m_containers.at(0)->reserve(materialAmount);
	this->m_containers.at(0)->insert(m_containers.at(0)->begin(), materialAmount, Material());
	for (size_t i = 0; i < materialAmount; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(0)->at(i);
	}

	m_textureHandler = new TextureHandler(materialAmount);
	if (device != nullptr) {
		this->m_device = device;
		m_textureHandler->SetDevice(device);
	}

}


Resources::MaterialHandler::~MaterialHandler()
{
	delete m_placeholder;
	delete m_textureHandler;
	for (size_t i = 0; i < m_containers.size(); i++)
	{
		delete m_containers.at(i);
	}
}

Resources::Status Resources::MaterialHandler::ClearUnusedMemory()
{
	m_emptyContainers.shrink_to_fit();

	for (size_t i = 0; i < m_containers.size(); i++)
	{
		m_containers.at(i)->shrink_to_fit();
	}
	m_containers.shrink_to_fit();
	m_textureHandler->ClearUnusedMemory();
	return Resources::Status::ST_OK;
}

Resources::Status Resources::MaterialHandler::GetMaterial(const unsigned int & id, ResourceContainer *& materialPtr)
{
	std::unordered_map<unsigned int, ResourceContainer>::iterator got = m_materials.find(id);
	if (got == m_materials.end()) { // if the texture does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		materialPtr = &got->second;
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::MaterialHandler::LoadMaterial( unsigned int & id, ResourceContainer *& materialPtr)
{

	std::unordered_map<unsigned int, ResourceContainer>::iterator got = m_materials.find(id);
	if (got != m_materials.end())
		return Status::ST_RES_ALREADY_LOADED;


	char* data = nullptr;
	size_t size = 0;

	Status st = FileLoader::GetInstance()->LoadResource(id, data, &size);
	if (st != ST_OK)
		return st;

	Resource::RawResourceData* resData = (Resource::RawResourceData*)data;
	if (resData->m_resType != RES_MATERIAL)
	{
#ifdef _DEBUG
		std::cout << "Wrong resource type. Wanted Material, got type: " << resData->m_id << std::endl;
#endif // _DEBUG

		return ST_WRONG_RESTYPE;
	}


	Material* newMaterial = GetEmptyContainer(); //Get an empty container

	st = newMaterial->Create(resData);
	if (st != ST_OK)
		return st;

	MaterialHeader matData = *(MaterialHeader*)(data + sizeof(Resource::RawResourceData));

	for (size_t i = 0; i < 5; i++) // set the textures
	{
		if (matData.textureIDs[i] != 0){
			ResourceContainer* temp = nullptr;
			
			st = m_textureHandler->GetTexture(matData.textureIDs[i], temp);
			if (st == ST_RES_MISSING) {
				st = m_textureHandler->LoadTexture(matData.textureIDs[i], temp);
				if (st != ST_OK) {
					newMaterial->SetTexture(&m_textureHandler->GetPlaceHolderTextures()[i], TextureType(i));
				}
				else
					newMaterial->SetTexture((Texture*)temp->resource, TextureType(i));
			}
			else {
				 temp->refCount += 1;
				 newMaterial->SetTexture((Texture*)temp->resource, TextureType(i));
			}
		}
	}

	newMaterial->SetValues(
		matData.m_Metallic,
		matData.m_Roughness,
		matData.m_EmissiveValue
	);

	
	m_materials[resData->m_id] = ResourceContainer(newMaterial, 1);
	materialPtr = &m_materials[resData->m_id];
	m_emptyContainers.pop_front();

	return Resources::Status::ST_OK;
}

Resources::Status Resources::MaterialHandler::UnloadMaterial( const unsigned int & id)
{
	ResourceContainer* materialRes = nullptr;

	Status st = GetMaterial(id, materialRes);
	switch (st)
	{
	case ST_OK:
		materialRes->refCount -= 1;
		if (materialRes->refCount <= 0)
		{
			Material* mat = ((Material*)materialRes->resource);
			Texture** texs = mat->GetAllTextures();
			for (size_t i = 0; i < 5; i++)
			{
				unsigned int id = texs[i]->GetId();
				if (id == 0)
					continue;
				m_textureHandler->UnloadTexture(id);
			}
			((Material*)materialRes->resource)->Destroy();
			m_emptyContainers.push_back(((Material*)materialRes->resource));
			m_materials.erase(id);
#ifdef _DEBUG
			std::cout << "Texture : " << id << ". Has been Unloaded" << std::endl;
#endif // _DEBUG
		}
	default:

		return st;
	}
	return Status::ST_OK;
}

Resources::Material * Resources::MaterialHandler::GetPlaceHolderMaterial()
{
	if (m_placeholder == nullptr)
	{
		this->CreatePlaceHolder();
	}
	return m_placeholder;
}

Resources::Status Resources::MaterialHandler::CreatePlaceHolder()
{

	if (m_device != nullptr) {
		delete m_placeholder;
		this->m_placeholder = new Material();
		Resource::RawResourceData data;
		data.m_id = -11;
		data.m_resType = RES_MATERIAL;
		m_placeholder->Create(&data);
		Texture* textures = m_textureHandler->GetPlaceHolderTextures();
		Status st = Status::ST_OK;
		for (size_t i = 0; i < 5; i++)
		{
		 st = m_placeholder->SetTexture(&textures[i], TextureType(i));
		}

		return st;
	}

	return Resources::ST_DEVICE_MISSING;
}

Resources::Material * Resources::MaterialHandler::GetEmptyContainer()
{
	if (m_emptyContainers.size() < 1)
	{
		m_containers.push_back(new std::vector<Material>(20));
		for (size_t i = 0; i < 20; i++)
		{
			m_emptyContainers.push_back(&m_containers.at(m_containers.size() - 1)->at(i));
		}
	}
	return m_emptyContainers.front();

}
