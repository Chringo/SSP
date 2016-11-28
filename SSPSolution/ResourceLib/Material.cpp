#include "Material.h"



Resources::Material::Material(Resource::RawResourceData resData): Resource(resData)
{
	m_resourceData.m_resType = ResourceType::RES_MATERIAL;
}

Resources::Material::Material() : Resource()
{
	m_resourceData.m_resType = ResourceType::RES_MATERIAL;
}


Resources::Material::~Material()
{

}

Resources::Status Resources::Material::Create(Resource::RawResourceData * resData)
{
	m_resourceData = *resData;
	m_resourceData.m_resType = RES_MATERIAL;

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Material::Destroy()
{

	char name[5] = { 'N', 'O', 'N', 'E','\0' };
	memcpy(m_resourceData.m_name, name, sizeof(char) * 5);
	this->m_resourceData.m_id = 0;
	for (size_t i = 0; i < 5; i++)
	{
		m_Textures[i] = nullptr;
	}
	m_Metallic		 = 0.0f ;
	m_Roughness		 = 0.0f ;
	m_EmissiveValue  = 0.0f ;

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Material::SetTexture(Texture * texture, TextureType type)
{
	if (type == TextureType::TEXTURE_UNKNOWN)
		return Status::ST_RES_MISSING;
	
	m_Textures[type] = texture;
	return Resources::Status::ST_OK;
}

std::shared_ptr<char> Resources::Material::GetDataAsBinary(size_t * size, bool * result)
{
	return std::shared_ptr<char>();
}
