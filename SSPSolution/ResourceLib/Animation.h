#ifndef RESOURCELIB_ANIMATION_ANIMATION_H
#define RESOURCELIB_ANIMATION_ANIMATION_H

#include "Resource.h"
#include "../GraphicsDLL/AnimationStateEnums.h"

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
			float quaternion[4] {0.0f,0.0f,0.0f,0.0f };
			float scale[3]		{0.0f,0.0f,0.0f };
		};

		struct AnimationState
		{
			/*Animation state information.*/
			int stateIndex = 0;
			float startTime = 0.f;
			float endTime = 0.f;
			bool isLooping = false;
			Blending blendFlag = Blending::NO_TRANSITION;
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
		AnimationState m_StateData;
	public:
		DLL_OPERATION Animation();
		DLL_OPERATION Animation(Resource::RawResourceData* resData);
		DLL_OPERATION Animation(Resource::RawResourceData* resData, AnimationData* anim);
		DLL_OPERATION virtual ~Animation();

		DLL_OPERATION Resources::Status Create(Resource::RawResourceData* resData, AnimationData* animData);
		DLL_OPERATION Resources::Status CreateFromBBF(Resource::RawResourceData* resData, AnimationData* animData);
		DLL_OPERATION Resources::Status Destroy(); // Deincrement references to connected data, free container

		DLL_OPERATION const unsigned int*   GetJointCount()		{ return &m_anim.jointCount; };
		DLL_OPERATION const AnimationJoint* GetJoint(int& index)	{ return ((unsigned int)index < m_anim.jointCount ? &m_anim.joints[index] : nullptr); };
		DLL_OPERATION const Animation::AnimationJoint* GetAllJoints() const	{ return m_anim.joints; };
		DLL_OPERATION AnimationState* GetAnimationStateData() { return &m_StateData; };

		DLL_OPERATION virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);

	private:
		void SetAnimationData(AnimationData* anim);
	};
}

#endif