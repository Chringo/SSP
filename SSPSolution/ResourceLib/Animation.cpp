#include "Animation.h"



Resources::Animation::Animation(): Resource()
{
}

Resources::Animation::Animation(Resource::RawResourceData * resData) : Resource(*resData)
{
	this->m_resourceData.m_resType = RES_ANIMATION;
}

Resources::Animation::Animation(Resource::RawResourceData * resData, AnimationData * anim) : Resource(*resData)
{
	this->m_resourceData.m_resType = RES_ANIMATION;
	SetAnimationData(anim);
}


Resources::Animation::~Animation()
{
	Destroy();
}

Resources::Status Resources::Animation::Create(Resource::RawResourceData * resData, AnimationData * animData)
{
	this->Destroy();
	m_resourceData.m_id				= resData->m_id;
	this->m_resourceData.m_resType  = ResourceType::RES_ANIMATION;
	SetAnimationData(animData);
	return Resources::Status::ST_OK;
}

Resources::Status Resources::Animation::CreateFromBBF(Resource::RawResourceData * resData, AnimationData * animData)
{
	this->Destroy();
	m_resourceData.m_id = resData->m_id;
	this->m_resourceData.m_resType = ResourceType::RES_ANIMATION;
	this->m_anim = *animData;
	return Resources::Status::ST_OK;
}

Resources::Status Resources::Animation::Destroy()
{
	for (size_t i = 0; i < m_anim.jointCount; i++)
	{
		delete m_anim.joints[i].keyframes;
		m_anim.joints[i].keyframes = nullptr;
	}
	delete m_anim.joints;
	m_anim.joints = nullptr;

	return Resources::Status::ST_OK;
}

std::shared_ptr<char> Resources::Animation::GetDataAsBinary(size_t * size, bool * result)
{
	return std::shared_ptr<char>();
}

void Resources::Animation::SetAnimationData(AnimationData * anim)
{

	if (this->m_anim.joints != nullptr)
		Destroy();
	m_anim.jointCount			= anim->jointCount;
	m_anim.joints = new AnimationJoint[anim->jointCount];
	for (size_t i = 0; i < anim->jointCount; i++)
	{
		m_anim.joints[i].keyframeCount  = anim->joints[i].keyframeCount;
		m_anim.joints[i].keyframes		= new Keyframe[anim->joints[i].keyframeCount];
		memcpy(m_anim.joints[i].keyframes,
			anim->joints[i].keyframes,
			sizeof(Keyframe) * anim->joints[i].keyframeCount);
	}
}
