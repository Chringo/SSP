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

Resources::Status Resources::AnimationHandler::LoadAnimation(const unsigned int & id, ResourceContainer *& animPtr, unsigned int index)
{
	char* data = nullptr;
	size_t size = 0;
	
	std::string path1 = "../ResourceLib/AssetFiles/SkelMesh1_IdleState.anim";
	std::string path2 = "../ResourceLib/AssetFiles/SkelMesh1_WalkState.anim";
	std::string path3 = "../ResourceLib/AssetFiles/SkelMesh1_RunState.anim";
	std::string path4 = "../ResourceLib/AssetFiles/SkelMesh1_ThrowState.anim";

	std::string path[4];
	path[0] = path1;
	path[1] = path2;
	path[2] = path3;
	path[3] = path4;

	Status st = FileLoader::GetInstance()->LoadFile(path[index], data, &size);
	if (st != ST_OK)
		return st;

	if (m_emptyContainers.size() < 1)
	{
		m_containers.push_back(Animation());
		m_emptyContainers.push_back(m_containers.end()._Ptr);
	}
	Animation* newAnim = m_emptyContainers.front(); //Get an empty container
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
