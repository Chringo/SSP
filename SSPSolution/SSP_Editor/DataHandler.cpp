#include "DataHandler.h"



Resources::Status DataHandler::AddAnimations(Resources::Animation * animation)
{
	this->loadedIds[animation->GetId()] = true;
	this->m_animations.push_back(animation);
	return Resources::Status::ST_OK;
}

Resources::Status DataHandler::AddSkeleton(Resources::Skeleton * skeleton)
{
	this->loadedIds[skeleton->GetId()] = true;
	this->m_skeletons.push_back(skeleton);
	return Resources::Status::ST_OK;
}

Resources::Status DataHandler::AddModel(Resources::Model * model)
{
	this->loadedIds[model->GetId()] = true;
	this->m_models.push_back(model);
	return Resources::Status::ST_OK;
}

Resources::Status DataHandler::AddMesh(Resources::Mesh * mesh)
{
	this->m_meshes.push_back(mesh);
	this->loadedIds[mesh->GetId()] = true;
	return Resources::Status::ST_OK;
}

Resources::Status DataHandler::AddMaterial(Resources::Material * material)
{
	m_materials.push_back(material);
	this->loadedIds[material->GetId()] = true;
	return Resources::Status::ST_OK;
}

Resources::Status DataHandler::AddTexture(Resources::Texture * texture)
{

	m_textures[std::string(texture->GetFileName())] = texture;

	return Resources::Status::ST_OK;
}

bool DataHandler::IDExists(unsigned int & id)
{
	std::unordered_map<unsigned int, bool>::iterator got = loadedIds.find(id);

	if (got == loadedIds.end()) { // if  does not exists in memory
		return false;
	}
	return true;
}

DataHandler * DataHandler::GetInstance()
{
	static DataHandler datahandler;

	return &datahandler;
}

DataHandler::DataHandler()
{
	this->m_textureHandler = new Resources::TextureHandler(20);
}


DataHandler::~DataHandler()
{
	for (size_t i = 0; i < m_models.size(); i++)
	{
		delete m_models.at(i);
		m_models.at(i) = nullptr;
	};
	for (size_t i = 0; i < m_materials.size(); i++)
	{
		delete m_materials.at(i);
		m_materials.at(i) = nullptr;
	};
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		delete m_meshes.at(i);
		m_meshes.at(i) = nullptr;
	};
	for (size_t i = 0; i < m_skeletons.size(); i++)
	{
		delete m_skeletons.at(i);
		m_skeletons.at(i) = nullptr;
	}

	for (auto iterator = m_textures.begin(); iterator != m_textures.end(); ++iterator)
	{
		Resources::Texture * node = iterator->second;
		delete node;
	}
	m_textures.clear();
	
	delete m_textureHandler;
}

Resources::Model * DataHandler::GetModel(unsigned int & id)
{
	for (size_t i = 0; i < m_models.size(); i++)
	{
		if (id == m_models.at(i)->GetId())
			return m_models.at(i);
	}

	return nullptr;
}

Resources::Status DataHandler::GetTexture(std::string id, Resources::Texture *& texture)
{
	std::unordered_map<std::string, Resources::Texture*>::iterator got = m_textures.find(id);

	if (got == m_textures.end()) { // if  does not exists in memory
		texture = &m_textureHandler->GetPlaceHolderTextures()[0]; //If the texture does not exist. return placeholder
		return Resources::Status::ST_RES_MISSING;
	}
	texture = got->second;

	return Resources::Status::ST_OK;
}
