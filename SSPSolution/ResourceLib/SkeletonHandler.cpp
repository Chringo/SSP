#include "SkeletonHandler.h"



Resources::SkeletonHandler::SkeletonHandler()
{
}


Resources::SkeletonHandler::SkeletonHandler(size_t skelAmount, ID3D11Device * device)
{
	this->m_emptyContainers.resize(skelAmount);

	this->m_skeletons.reserve(skelAmount);
	
	this->m_containers.push_back(new std::vector<Skeleton>);

	this->m_containers.at(0)->insert(m_containers.at(0)->begin(), skelAmount, Skeleton());
	for (size_t i = 0; i < skelAmount; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(0)->at(i);
	}

	this->m_animHandler = new AnimationHandler(skelAmount);
	if (device != nullptr) {
		this->m_device = device;
	}


}

Resources::Status Resources::SkeletonHandler::GetSkeleton(const unsigned int & id, ResourceContainer *& skelPtr)
{
	std::unordered_map<unsigned int, ResourceContainer>::iterator got = m_skeletons.find(id);
	if (got == m_skeletons.end()) { // if the skeleton does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		skelPtr = &got->second;
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::SkeletonHandler::LoadSkeleton(const unsigned int & id, ResourceContainer *& skelPtr)
{

	char* data = nullptr;
	size_t size = 0;
	std::string path = "../ResourceLib/AssetFiles/SkelMesh1.skel";
 	Status st = FileLoader::GetInstance()->LoadFile(path, data, &size);
	if (st != ST_OK)
		return st;
	
	
		
	Skeleton* newSkeleton = GetEmptyContainer(); //Get an empty container
	Resource::RawResourceData resData;
	Skeleton::RawSkeletonData skelData;

	resData.m_id	  = ((MainHeader*)data)->id;
	resData.m_resType = Resources::ResourceType(((MainHeader*)data)->resourceType);
	if (resData.m_resType != RES_SKELETON){
#ifdef _DEBUG
		std::cout << "Wrong resource type. Wanted skeleton, got type: " << resData.m_id << std::endl;
#endif // _DEBUG
		return ST_WRONG_RESTYPE;
	}
	data += sizeof(MainHeader);

	skelData.jointCount		 = ((SkeletonHeader*)data)->jointCount;
	unsigned int* animCount  = &((SkeletonHeader*)data)->animLayerCount;
	data					+= sizeof(SkeletonHeader);
	skelData.joints		     = (Skeleton::Joint*)data;
	data				    += sizeof(JointHeader) * skelData.jointCount;

	st = newSkeleton->Create(&resData, &skelData);
	if (st != ST_OK)
		return st;

	skelData.joints = nullptr;

	/*
	Load all animations for the skeleton
	for each anim
	m_animationHandler
	newSkeleton->AddAnimation
	*/
	ResourceContainer* animPtr;
	
	m_animHandler->LoadAnimation(UINT(213), animPtr);
	newSkeleton->AddAnimation((Animation*)animPtr->resource, 0);

	//int animsloaded = 0;
	//for (size_t i = 0; i < *animCount; i++)
	//{
	//	ResourceContainer* animPtr;
	//	unsigned int* id = &((LayerIdHeader*)data)->id;
	//	m_animHandler->LoadAnimation(UINT(213), animPtr);
	//	//st = m_animationHandler->GetAnimation(id, &animPtr);
	//	//switch (st) {
	//	//	case Status::ST_RES_MISSING: { //if it doesent exist
	//	//		//Status mSt = m_animationHandler->LoadAnimation(id, animPtr); //load the animation
	//	//		if (mSt != ST_OK) {
	//	//			continue;
	//	//		}
	//	//		else
	//	//			newSkeleton->AddAnimation((Animation*)animPtr->resource, animsloaded);
	//	//		break;
	//	//	}
	//	//	case Status::ST_OK: {
	//	//		animPtr->refCount += 1;
	//	//		newSkeleton->AddAnimation((Animation*)animPtr->resource, animsloaded);
	//	//		break;
	//	//	}
	//		animsloaded++;
	//	//}
	//}

	m_skeletons[id] = ResourceContainer(newSkeleton, 1);	 // put it into the map
	m_emptyContainers.pop_front();							 // remove from empty container queue

	skelPtr = &m_skeletons[id];

	return Resources::Status::ST_OK;
}

Resources::Status Resources::SkeletonHandler::UnloadSkeleton(const unsigned int & id)
{
	ResourceContainer* skelRes = nullptr;

	Status st = GetSkeleton(id, skelRes);
	switch (st)
	{
	case ST_OK:
		skelRes->refCount -= 1;
		if (skelRes->refCount <= 0)
		{
			 const std::vector<unsigned int>* animationsIds = ((Skeleton*)skelRes->resource)->GetAllAnimationIds();
			 for (size_t i = 0; i < animationsIds->size(); i++)
			 {
				 if (animationsIds->at(i) == 0)
					 continue;
				 m_animHandler->UnloadAnimation(animationsIds->at(i));
			 }
			((Skeleton*)skelRes->resource)->Destroy();
			m_emptyContainers.push_back(((Skeleton*)skelRes->resource));
			m_skeletons.erase(id);
#ifdef _DEBUG
			std::cout << "Skeleton : " << id << ". Has been Unloaded" << std::endl;
#endif // _DEBUG
		}
	default:
		return st;
	}
	return Resources::Status::ST_OK;
}

Resources::SkeletonHandler::~SkeletonHandler()
{
	for (size_t i = 0; i < m_containers.size(); i++)
	{
		delete m_containers.at(i);
	}
	delete m_animHandler;
}

Resources::Skeleton * Resources::SkeletonHandler::GetEmptyContainer()
{
	if (m_emptyContainers.size() < 1)
	{
		m_containers.push_back(new std::vector<Skeleton>(20));
		for (size_t i = 0; i < 20; i++)
		{
			m_emptyContainers.push_back(&m_containers.at(m_containers.size() - 1)->at(i));
		}
	}
	return m_emptyContainers.front();
}
