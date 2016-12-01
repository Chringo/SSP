#include "Skeleton.h"



Resources::Skeleton::Skeleton() :Resource()
{
	m_animations.resize(MAX_ANIMATIONS, nullptr);
}

Resources::Skeleton::Skeleton(Resource::RawResourceData * resData) : Resource(*resData)
{
	m_animations.resize(MAX_ANIMATIONS, nullptr);
	this->m_resourceData.m_resType = ResourceType::RES_SKELETON;
}

Resources::Skeleton::Skeleton(Resource::RawResourceData * resData, RawSkeletonData * skelData)
	:Resource(*resData)
{
	this->m_resourceData.m_resType = ResourceType::RES_SKELETON;
	m_skelData = *skelData;
	m_animations.resize(MAX_ANIMATIONS,nullptr);
	
}


Resources::Skeleton::~Skeleton()
{
	delete m_animationIds;
	m_animationIds = nullptr;
}

Resources::Status Resources::Skeleton::Create(Resource::RawResourceData * resData, RawSkeletonData * skelData)
{
	this->Destroy();

	memcpy((char*)m_resourceData.m_name, (char*)resData->m_name, 256);
	m_resourceData.m_id = resData->m_id;
	this->m_resourceData.m_resType = ResourceType::RES_SKELETON;	
	m_skelData = *skelData;

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Skeleton::Destroy()
{
	m_animations.clear();		//clear
	m_animations.insert(		//set all to nullptr
		m_animations.begin(),
		m_animations.size(),
		nullptr);
	m_numAnimations = 0;
	char name[5] = { 'N', 'O', 'N', 'E','\0' };
	memcpy(m_resourceData.m_name, name, sizeof(char) * 5);
	this->m_resourceData.m_id = 0;

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Skeleton::AddAnimation(Animation * anim, int index)
{
	//TODO. make safeguards that the vector expands if the index is out of range

	if (index >= MAX_ANIMATIONS){
#ifdef _DEBUG
		std::cout << "Animation index is higher than Maximum animations. Check index or expand the limit" << std::endl;
#endif // _DEBUG
	}

	if(m_animations[index] == nullptr)
		m_numAnimations		+= 1;

	m_animations[index] = anim;
	
	return Resources::Status::ST_OK;
}

std::shared_ptr<char> Resources::Skeleton::GetDataAsBinary(size_t * size, bool * result)
{
	return std::shared_ptr<char>();
}
