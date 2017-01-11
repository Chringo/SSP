#include "Skeleton.h"



Resources::Skeleton::Skeleton() :Resource()
{
	m_animations.reserve(MAX_ANIMATIONS);
	for (size_t i = 0; i < MAX_ANIMATIONS; i++)
	{
		m_animations.push_back(nullptr);
		
	}
	//m_animations.insert(m_animations.cend(), MAX_ANIMATIONS, Animation* );
}

Resources::Skeleton::Skeleton(Resource::RawResourceData * resData) : Resource(*resData)
{
	m_animations.reserve(MAX_ANIMATIONS);
	for (size_t i = 0; i < MAX_ANIMATIONS; i++)
	{
		m_animations.push_back(nullptr);
	}
	this->m_resourceData.m_resType = ResourceType::RES_SKELETON;
}

Resources::Skeleton::Skeleton(Resource::RawResourceData * resData, RawSkeletonData * skelData)
	:Resource(*resData)
{
	this->m_resourceData.m_resType = ResourceType::RES_SKELETON;
	m_skelData = *skelData;
	m_animations.reserve(MAX_ANIMATIONS);
	for (size_t i = 0; i < MAX_ANIMATIONS; i++)
	{
		m_animations.push_back(nullptr);
	}
	
}


Resources::Skeleton::~Skeleton()
{
}

Resources::Status Resources::Skeleton::Create(Resource::RawResourceData * resData, RawSkeletonData * skelData)
{
	this->Destroy();
	m_resourceData.m_id = resData->m_id;
	this->m_resourceData.m_resType = ResourceType::RES_SKELETON;	
	m_skelData = *skelData;

	m_animations.reserve(MAX_ANIMATIONS);
	for (size_t i = 0; i < MAX_ANIMATIONS; i++)
	{
		m_animations.push_back(nullptr);
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Skeleton::Destroy()
{
	m_animations.clear();		//clear
	m_numAnimations = 0;
	m_animationIds.insert(m_animationIds.begin(), m_animationIds.size(),unsigned int(0));
	this->m_resourceData.m_id = 0;

	

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Skeleton::AddAnimation(Animation * anim, int index)
{

	if (index >= MAX_ANIMATIONS){
#ifdef _DEBUG
		std::cout << "Animation index is higher than Maximum animations. Check index or expand the limit" << std::endl;
#endif // _DEBUG
		return Status::ST_MEM_FULL;
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
