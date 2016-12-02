#include "AnimationHandler.h"



Resources::AnimationHandler::AnimationHandler()
{
}


Resources::AnimationHandler::AnimationHandler(size_t animAmount)
{
	this->m_emptyContainers.resize(animAmount);
	this->m_animations.reserve(animAmount);
	this->m_containers.reserve(animAmount);
	this->m_containers.insert(m_containers.begin(), animAmount, Animation());
	for (size_t i = 0; i < animAmount; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(i);
	}
}

Resources::AnimationHandler::~AnimationHandler()
{
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
	std::string path = "../ResourceLib/AssetFiles/pCube1_AnimLayer1.anim";
	Status st = FileLoader::GetInstance()->LoadFile(path, data, &size);
	if (st != ST_OK)
		return st;

	if (m_emptyContainers.size() < 1)
	{
		m_containers.push_back(Animation());
		m_emptyContainers.push_back(m_containers.end()._Ptr);
	}
	Animation* newAnim = m_emptyContainers.front(); //Get an empty container
	Resource::RawResourceData resData;
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

	animData.jointCount = *(unsigned int*)data;
	data += sizeof(unsigned int);
	animData.keyframeCount = *(unsigned int*)data;
	data += sizeof(unsigned int);

	animData.keyframes = (Animation::Keyframe*)data;

	st = newAnim->Create(&resData, &animData); 
	if (st != ST_OK)
		return st;

	m_animations[id] = ResourceContainer(newAnim, 1);
	m_emptyContainers.pop_front();
	animPtr = &m_animations[id];

	return Resources::Status::ST_OK;
}

Resources::Status Resources::AnimationHandler::UnloadAnimation(const unsigned int & id)
{
	return Resources::Status();
}
