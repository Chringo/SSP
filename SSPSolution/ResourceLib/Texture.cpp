#include "Texture.h"



Resources::Texture::Texture() : Resource()
{
	m_resourceData.m_resType = ResourceType::RES_TEXTURE;
	m_fileName[0] = '\0';
	this->textureResource = nullptr;
	this->textureView = nullptr;
}


Resources::Texture::~Texture()
{
	Destroy();
}

Resources::Texture::Texture(Resource::RawResourceData resData): Resource(resData)
{
	m_resourceData.m_resType = ResourceType::RES_TEXTURE;
	this->textureResource = nullptr;
	this->textureView = nullptr;
}

Resources::Status Resources::Texture::Create(Resource::RawResourceData * resData)
{
	Destroy();
	m_resourceData.m_id = resData->m_id;
	m_resourceData.m_resType = RES_TEXTURE;
	return Resources::Status::ST_OK;
}

Resources::Status Resources::Texture::Destroy()
{
	if (!Resources::SAFE_RELEASE(this->textureView))
		return Resources::Status::ST_BUFFER_ERROR;
	textureView		= nullptr;

	if (!Resources::SAFE_RELEASE(this->textureResource))
		return Resources::Status::ST_BUFFER_ERROR;
	textureResource = nullptr;


	m_fileName[0] = '\0';
	this->m_resourceData.m_id = 0;

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Texture::SetTexture(ID3D11ShaderResourceView * view, ID3D11Resource * texture)
{
	//Status st = Destroy();
	if (!Resources::SAFE_RELEASE(this->textureView))
		return Resources::Status::ST_BUFFER_ERROR;
	textureView = nullptr;

	if (!Resources::SAFE_RELEASE(this->textureResource))
		return Resources::Status::ST_BUFFER_ERROR;
	textureResource = nullptr;
	//if (st == ST_OK)
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
