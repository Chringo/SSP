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
	delete m_anim.keyframes;
	m_anim.keyframes = nullptr;
}

Resources::Status Resources::Animation::Create(Resource::RawResourceData * resData, AnimationData * animData)
{
	this->Destroy();

	memcpy((char*)m_resourceData.m_name, (char*)resData->m_name, 256);
	m_resourceData.m_id = resData->m_id;
	this->m_resourceData.m_resType = ResourceType::RES_ANIMATION;
	SetAnimationData(animData);


	return Resources::Status::ST_OK;
}

Resources::Status Resources::Animation::Destroy()
{
	delete m_anim.keyframes;

	m_anim.keyframes			= nullptr;
	this->m_anim.jointCount		= 0;
	this->m_anim.keyframeCount	= 0;

	return Resources::Status::ST_OK;
}

const Resources::Animation::Keyframe * Resources::Animation::GetKeyframe(unsigned int & index)
{
	if (index >= m_anim.keyframeCount)
		return nullptr;

	Keyframe* ptr = m_anim.keyframes + (index * m_anim.jointCount);

	return ptr;
}

std::shared_ptr<char> Resources::Animation::GetDataAsBinary(size_t * size, bool * result)
{
	return std::shared_ptr<char>();
}

void Resources::Animation::SetAnimationData(AnimationData * anim)
{

	if (this->m_anim.keyframes != nullptr)
		Destroy();
	m_anim.jointCount			= anim->jointCount;
	m_anim.keyframeCount	    = anim->keyframeCount;
	m_anim.keyframes			= new Keyframe[m_anim.keyframeCount * m_anim.jointCount];
	memcpy(m_anim.keyframes, anim->keyframes, sizeof(Keyframe)* (anim->jointCount * anim->keyframeCount));
}
