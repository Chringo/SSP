#include "Model.h"



Resources::Model::Model(Resource::RawResourceData resData)
	:Resource(resData)
{
	m_resourceData.m_resType = RES_MODEL;
}

Resources::Model::Model() : Resource()
{
	m_resourceData.m_resType = RES_MODEL;
}


Resources::Model::~Model()
{
	delete m_rawData;
}

Resources::Model::Model(const Model & obj)
{
	this->m_resourceData = obj.m_resourceData;
	if (obj.m_rawData != nullptr)
	{
		m_rawData = new RawModelData(*obj.m_rawData);
	}
	m_modelMesh = obj.m_modelMesh;
	m_material  = obj.m_material;
	m_skeleton  = obj.m_skeleton;
}

Resources::Status Resources::Model::Create(Resource::RawResourceData* resData, RawModelData* modelData , bool keepRawData)
{
	m_resourceData = *resData;
	m_resourceData.m_resType = RES_MODEL;
	if (keepRawData == true && modelData != nullptr)
	{
		delete m_rawData; m_rawData = new RawModelData; *m_rawData = *modelData;

	}
	return Resources::Status::ST_OK;
}

Resources::Status Resources::Model::Destroy()
{

	m_modelMesh = nullptr;
	m_material  = nullptr;
	m_skeleton  = nullptr;
	return Resources::Status::ST_OK;
}

std::shared_ptr<char> Resources::Model::GetDataAsBinary(size_t * size, bool * result)
{
	return std::shared_ptr<char>();
}
