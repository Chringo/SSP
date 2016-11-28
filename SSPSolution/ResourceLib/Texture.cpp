#include "Texture.h"



Resources::Texture::Texture() : Resource()
{
	m_resourceData.m_resType = ResourceType::RES_TEXTURE;
	m_filePath[0] = '\0';
}


Resources::Texture::~Texture()
{
	Destroy();
}

Resources::Texture::Texture(Resource::RawResourceData resData): Resource(resData)
{
	m_resourceData.m_resType = ResourceType::RES_TEXTURE;
}

Resources::Status Resources::Texture::Create(Resource::RawResourceData * resData)
{
	Destroy();
	memcpy((char*)m_resourceData.m_name, (char*)resData->m_name, 256);
	m_resourceData.m_id = resData->m_id;
	m_resourceData.m_resType = RES_TEXTURE;
	return Resources::Status();
}

Resources::Status Resources::Texture::Destroy()
{
	if (!Resources::SAFE_RELEASE(this->textureView))
		return Resources::Status::ST_BUFFER_ERROR;
	textureView		= nullptr;

	if (!Resources::SAFE_RELEASE(this->textureResource))
		return Resources::Status::ST_BUFFER_ERROR;
	textureResource = nullptr;


	m_filePath[0] = '\0';

	char name[5] = { 'N', 'O', 'N', 'E','\0' };
	memcpy(m_resourceData.m_name, name, sizeof(char) * 5);
	this->m_resourceData.m_id = 0;

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Texture::SetTexture(ID3D11ShaderResourceView * view, ID3D11Resource * texture)
{
	Status st = Destroy();
	if (st == ST_OK)
	{
		this->textureResource = texture;
		this->textureView	  = view;
		return ST_OK;
	}
	return Status::ST_BUFFER_ERROR;
}

std::shared_ptr<char> Resources::Texture::GetDataAsBinary(size_t * size, bool * result)
{
	return std::shared_ptr<char>();
}
