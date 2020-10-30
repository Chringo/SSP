#include "Resource.h"



Resources::Resource::Resource()
{
	this->m_resourceData.m_id = 0;

}

Resources::Resource::Resource(RawResourceData data)
{
	this->m_resourceData = data;
}


Resources::Resource::~Resource()
{
}

const bool Resources::Resource::IsType(ResourceType type) const
{
	return m_resourceData.m_resType == type ? true : false;
}


const unsigned int Resources::Resource::GetId() const
{
	return m_resourceData.m_id;
}

 const std::string Resources::Resource::GetName() const
{
	return "";//m_resourceData.m_name;
}

std::shared_ptr<char> Resources::Resource::GetDataAsBinary(size_t* size, bool* result)
{
	*size = sizeof(RawResourceData);
	
	if (result != nullptr)
		*result = true;

	return std::shared_ptr<char>((char*)&m_resourceData);

}


