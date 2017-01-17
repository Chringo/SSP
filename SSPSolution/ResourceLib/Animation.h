#ifndef RESOURCELIB_ANIMATION_ANIMATION_H
#define RESOURCELIB_ANIMATION_ANIMATION_H

#include "Resource.h"

namespace Resources 
{
	class Animation :
		public Resource
	{
	public:
		struct Keyframe 
		{
			float timeValue = 0;
			float translation[3]{0.0f,0.0f,0.0f };
			float quaternion[4]{0.0f,0.0f,0.0f,0.0f};
			float scale[3]		{0.0f,0.0f,0.0f };
		};
		struct AnimationJoint 
		{
			unsigned int keyframeCount	= 0;
			Keyframe*	 keyframes	    = nullptr;
		}; 
		struct AnimationData 
		{
			unsigned int jointCount		= 0;
			AnimationJoint* joints		= nullptr;
		};
	private:
		AnimationData m_anim;
	public:
		Animation();
		Animation(Resource::RawResourceData* resData);
		Animation(Resource::RawResourceData* resData, AnimationData* anim);
		virtual ~Animation();

		Resources::Status Create(Resource::RawResourceData* resData, AnimationData* animData);
		Resources::Status Destroy(); // Deincrement references to connected data, free container

		const unsigned int*   GetJointCount()		{ return &m_anim.jointCount; };
		const AnimationJoint* GetJoint(int& index)	{ return ((unsigned int)index < m_anim.jointCount ? &m_anim.joints[index] : nullptr); };
		const AnimationJoint* GetAllJoints() const	{ return m_anim.joints; };

		virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);

	private:
		void SetAnimationData(AnimationData* anim);
	};
}

#endif