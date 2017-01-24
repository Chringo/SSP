#include "AnimationHandler.h"



Resources::AnimationHandler::AnimationHandler()
{
}


Resources::AnimationHandler::AnimationHandler(size_t animAmount)
{
	this->m_emptyContainers.resize(animAmount);
	this->m_animations.reserve(animAmount);

	this->m_containers.push_back(new std::vector<Animation>);

	this->m_containers.at(0)->insert(m_containers.at(0)->begin(), animAmount, Animation());
	for (size_t i = 0; i < animAmount; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(0)->at(i);
	}
}

Resources::AnimationHandler::~AnimationHandler()
{
	for each (std::vector<Animation>* container in m_containers)
	{
		delete container;
	}
}

Resources::Status Resources::AnimationHandler::GetAnimation(const unsigned int & id, ResourceContainer *& animPtr)
{
	std::unordered_map<unsigned int, ResourceContainer>::iterator got = m_animations.find(id);
	if (got == m_animations.end()) { // if the skeleton does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		animPtr = &got->second;
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::AnimationHandler::LoadAnimation(const unsigned int & id, ResourceContainer *& animPtr)
{

	char* data = nullptr;
	size_t size = 0;
	std::string path = "../ResourceLib/AssetFiles/SkelMesh1_AnimLayer1.anim";
	Status st = FileLoader::GetInstance()->LoadFile(path, data, &size);
	if (st != ST_OK)
		return st;

	
	Animation* newAnim = GetEmptyContainer(); //Get an empty container
	Resource ::RawResourceData resData;
	Animation::AnimationData animData;

	resData.m_id = ((MainHeader*)data)->id;
	resData.m_resType = Resources::ResourceType(((MainHeader*)data)->resourceType);
	if (resData.m_resType != RES_ANIMATION) {
#ifdef _DEBUG
		std::cout << "Wrong resource type. Wanted Animation, got type: " << resData.m_id << std::endl;
#endif // _DEBUG
		return ST_WRONG_RESTYPE;
	}
	data += sizeof(MainHeader);

	//write jointCount

	//for (  each joint )
		// write keyframe count
		// for ( each keyframe ) 
			// write keyframe 
	animData.jointCount = *(unsigned int*)data;
	data += sizeof(unsigned int);
	std::vector<Animation::AnimationJoint> joints(animData.jointCount);
	for (size_t i = 0; i < animData.jointCount; i++)
	{
		joints.at(i).keyframeCount = *(unsigned int*)data;
		data += sizeof(unsigned int);
		joints.at(i).keyframes     = (Animation::Keyframe*)data;
		data += sizeof(Animation::Keyframe) * joints.at(i).keyframeCount;
	}
	animData.joints = joints.data();
	st = newAnim->Create(&resData, &animData); 
	if (st != ST_OK)
		return st;

	m_animations[id] = ResourceContainer(newAnim, 1);
	m_emptyContainers.pop_front();
	animPtr = &m_animations[id];
	joints.clear();
	return Resources::Status::ST_OK;
}

Resources::Status Resources::AnimationHandler::UnloadAnimation(const unsigned int & id)
{
	ResourceContainer* animRes = nullptr;

	Status st = GetAnimation(id, animRes);
	switch (st)
	{

	case ST_OK:
		animRes->refCount -= 1;
		if (animRes->refCount <= 0)
		{
			((Animation*)animRes->resource)->Destroy();
			m_emptyContainers.push_back(((Animation*)animRes->resource));
			m_animations.erase(id);
#ifdef _DEBUG
			std::cout << "Animation : " << id << ". Has been Unloaded" << std::endl;
#endif // _DEBUG
		}
	default:
		return st;
	}
	return Resources::Status::ST_OK;
}

Resources::Animation * Resources::AnimationHandler::GetEmptyContainer()
{
	if (m_emptyContainers.size() < 1)
	{
		m_containers.push_back(new std::vector<Animation>(20));
		for (size_t i = 0; i < 20; i++)
		{
			m_emptyContainers.push_back(&m_containers.at(m_containers.size() - 1)->at(i));
		}
	}
	return m_emptyContainers.front();
}
