#include "AnimationHandler.h"

AnimationHandler::AnimationHandler()
{
	
}

AnimationHandler::~AnimationHandler()
{
}

void AnimationHandler::Initialize(GraphicsAnimationComponent ** graphicAnimComponents, int * noActiveComponents)
{
	/*Initialize all Graphics Animation Components arrays with joint matrices for each skeleton.*/
	this->m_animGraphicsComponents = graphicAnimComponents;
	this->m_nrOfGraphicsAnimationComponents = noActiveComponents;

	//this->m_TransitionComplete = false;

	this->m_nrOfAnimComps = 0;
	this->m_maxAnimComps = 16;

	this->m_AnimCompIndex = 0;

	for (int i = 0; i < this->m_maxAnimComps; i++)
	{
		this->m_AnimComponentList.push_back(CreateAnimationComponent());
	}
}

void AnimationHandler::Update(float dt)
{
	/*Convert the delta-time to be in seconds unit format.*/
	float speed = 2.0f;

	float seconds = dt / 1000000.f;

	/*Iterate each component and check if it's active and update animation.*/
	for (int aCompIndex = 0; aCompIndex < this->m_nrOfAnimComps; aCompIndex++)
	{
		if (this->m_AnimComponentList[m_AnimCompIndex]->active == TRUE)
		{
			SetAnimCompIndex(aCompIndex);

			switch (m_AnimComponentList[m_AnimCompIndex]->blendFlag)
			{
				/*If only one animation is playing, there should be no transition.*/
			case (Blending::NO_TRANSITION):
			{
#ifdef _DEBUG
				assert(m_AnimComponentList[m_AnimCompIndex]->source_State != nullptr);
#endif
				m_AnimComponentList[m_AnimCompIndex]->source_Time += seconds;

				/*If the animation reaches the last frame, either reset animation or switch to idle state.*/
				if (m_AnimComponentList[m_AnimCompIndex]->source_Time >= m_AnimComponentList[m_AnimCompIndex]->source_State->endTime)
				{
					/*Animation is looping. Reset the time of the animation.*/
					if (m_AnimComponentList[m_AnimCompIndex]->source_State->isLooping == TRUE)
					{
						m_AnimComponentList[m_AnimCompIndex]->source_Time = 0;
					}
					else
					{
						/*If the player picks up the ball, go to player ball idle, otherwise regular idle.*/
						if (m_AnimComponentList[m_AnimCompIndex]->source_State->stateIndex == PLAYER_PICKUP)
						{
							SetAnimationComponent(PLAYER_BALL_IDLE, 0.5f, SMOOTH_TRANSITION, true);
							m_AnimComponentList[m_AnimCompIndex]->blendFlag = SMOOTH_TRANSITION;
						}
						else
						{
							SetAnimationComponent(PLAYER_IDLE, 0.5f, SMOOTH_TRANSITION, true);
							m_AnimComponentList[m_AnimCompIndex]->blendFlag = SMOOTH_TRANSITION;
						}
						
						break;
					}
				}

				/*Interpolate the keyframes of this animation.*/
				InterpolateKeys(m_AnimComponentList[m_AnimCompIndex]->source_State, m_AnimComponentList[m_AnimCompIndex]->source_Time);

				break;
			}

			/*If two animations are playing, there should be a smooth transition.*/
			case (Blending::SMOOTH_TRANSITION):
			{
				/*Transition is complete. Swap the animations and remove the old animation.*/
				if (m_AnimComponentList[m_AnimCompIndex]->m_TransitionComplete == true)
				{
					m_AnimComponentList[m_AnimCompIndex]->blendFlag = NO_TRANSITION;
					m_AnimComponentList[m_AnimCompIndex]->m_TransitionComplete = false;
					
					/*After the blending is finished, the target state will simply become the new source state.*/
					m_AnimComponentList[m_AnimCompIndex]->source_State = m_AnimComponentList[m_AnimCompIndex]->target_State;
					m_AnimComponentList[m_AnimCompIndex]->source_Time = m_AnimComponentList[m_AnimCompIndex]->target_Time;

					m_AnimComponentList[m_AnimCompIndex]->target_State = nullptr;
					m_AnimComponentList[m_AnimCompIndex]->target_Time = 0;
				}

				/*Blending is not complete. Proceed the transition process.*/
				else
				{
					Blend(seconds);
				}

				break;
			}
			}
		}
	}
}

AnimationComponent* AnimationHandler::CreateAnimationComponent()
{
	AnimationComponent* animComp = new AnimationComponent; 
	animComp->active = 0;
	animComp->source_State = nullptr;
	animComp->target_State = nullptr;
	animComp->source_Time = 0.f;
	animComp->target_Time = 0.f;
	animComp->m_TransitionDuration = 0.f;
	animComp->m_TransitionTimeLeft = 0.f;
	animComp->blendFlag = NO_TRANSITION;
	animComp->skeleton = nullptr;
	animComp->animation_States = nullptr;

	return animComp;
}

AnimationComponent * AnimationHandler::GetNextAvailableComponent()
{
	if (this->m_nrOfAnimComps == this->m_maxAnimComps)
	{
		int previousMax = this->m_maxAnimComps;
		this->m_maxAnimComps += this->m_maxAnimComps;

		for (int i = previousMax; i < m_maxAnimComps; i++)
		{
			this->m_AnimComponentList.push_back(CreateAnimationComponent());
		}
	}
	this->m_nrOfAnimComps++;
	return this->m_AnimComponentList[this->m_nrOfAnimComps - 1];
}

void AnimationHandler::UpdateAnimationComponents(float dt)
{
	/*Iterate each animation component to check if their active or not.*/
	for (int compIndex = 0; compIndex < this->m_AnimComponentList.size(); compIndex++)
	{
		/*If the current iterating component is active, update component and data.*/
		if (this->m_AnimComponentList[compIndex]->active == true)
		{

		}

		else
		{
			/*Should something happen if their not active? Leave this for now!*/
		}
	}
}

void AnimationHandler::ShutDown()
{
	for (int i = 0; i < this->m_AnimComponentList.size(); i++)
	{
		delete this->m_AnimComponentList[i];
	}

	this->m_maxAnimComps = 16;
	this->m_nrOfAnimComps = 0;
	m_AnimComponentList.clear();
	m_AnimComponentList.shrink_to_fit();
}

void AnimationHandler::InterpolateKeys(Resources::Animation::AnimationState* animState, float currentTime)
{
	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	std::vector<DirectX::XMMATRIX> localTransforms(jointCount);

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animState->stateIndex)->GetAllJoints();

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animState->startTime)
		{
			int startFrame = (int)animState->startTime;

			DirectX::XMVECTOR trans = { animatedJoint.keyframes[startFrame].translation[0],
				animatedJoint.keyframes[startFrame].translation[1], animatedJoint.keyframes[startFrame].translation[2] };
			DirectX::XMVECTOR scale = { animatedJoint.keyframes[startFrame].scale[0],
				animatedJoint.keyframes[startFrame].scale[1], animatedJoint.keyframes[startFrame].scale[2] };
			DirectX::XMVECTOR quat = { animatedJoint.keyframes[startFrame].quaternion[0],
				animatedJoint.keyframes[startFrame].quaternion[1], animatedJoint.keyframes[startFrame].quaternion[2],
				animatedJoint.keyframes[startFrame].quaternion[3] };

			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
			DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(quat);
			DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(trans);

			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

			localTransforms[jointIndex] = localTransform;
		}
		/*The current time is the last keyframe.*/
		else if (currentTime >= animState->endTime)
		{
			int endFrame = animatedJoint.keyframeCount - 1;

			DirectX::XMVECTOR trans = { animatedJoint.keyframes[endFrame].translation[0],
				animatedJoint.keyframes[endFrame].translation[1], animatedJoint.keyframes[endFrame].translation[2] };
			DirectX::XMVECTOR scale = { animatedJoint.keyframes[endFrame].scale[0],
				animatedJoint.keyframes[endFrame].scale[1], animatedJoint.keyframes[endFrame].scale[2] };
			DirectX::XMVECTOR quat = { animatedJoint.keyframes[endFrame].quaternion[0],
				animatedJoint.keyframes[endFrame].quaternion[1], animatedJoint.keyframes[endFrame].quaternion[2],
			animatedJoint.keyframes[endFrame].quaternion[3]};

			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
			DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(quat);
			DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(trans);

			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

			localTransforms[jointIndex] = localTransform;
		}
		/*The current time is between two keyframes.*/
		else
		{
			int keyFrameCount = animatedJoint.keyframeCount;

			for (int i = 0; i < keyFrameCount; i++)
			{
				float timeKeyframe1 = animatedJoint.keyframes[i].timeValue;
				float timeKeyframe2 = animatedJoint.keyframes[i + 1].timeValue;

				/*Check if the current time is between two keyframes for each joint.*/
				if (currentTime > timeKeyframe1 && currentTime < timeKeyframe2)
				{
					/*Lerp factor is calculated for a normalized value between 0-1 for interpolation.*/
					float lerpFactor = (currentTime - timeKeyframe1) / (timeKeyframe2 - timeKeyframe1);

					DirectX::XMVECTOR trans1 = { animatedJoint.keyframes[i].translation[0], 
						animatedJoint.keyframes[i].translation[1], animatedJoint.keyframes[i].translation[2] };
					DirectX::XMVECTOR trans2 = { animatedJoint.keyframes[i + 1].translation[0],
						animatedJoint.keyframes[i + 1].translation[1], animatedJoint.keyframes[i + 1].translation[2] };
					DirectX::XMVECTOR scale1 = { animatedJoint.keyframes[i].scale[0],
						animatedJoint.keyframes[i].scale[1], animatedJoint.keyframes[i].scale[2] };
					DirectX::XMVECTOR scale2 = { animatedJoint.keyframes[i + 1].scale[0],
						animatedJoint.keyframes[i + 1].scale[1], animatedJoint.keyframes[i + 1].scale[2] };
					DirectX::XMVECTOR quat1 = { animatedJoint.keyframes[i].quaternion[0],
						animatedJoint.keyframes[i].quaternion[1], animatedJoint.keyframes[i].quaternion[2],
						animatedJoint.keyframes[i].quaternion[3]};
					DirectX::XMVECTOR quat2 = { animatedJoint.keyframes[i + 1].quaternion[0],
						animatedJoint.keyframes[i + 1].quaternion[1], animatedJoint.keyframes[i + 1].quaternion[2],
						animatedJoint.keyframes[i + 1].quaternion[3] };

					DirectX::XMVECTOR lerpTrans = DirectX::XMVectorLerp(trans1, trans2, lerpFactor);
					DirectX::XMVECTOR lerpScale = DirectX::XMVectorLerp(scale1, scale2, lerpFactor);
					DirectX::XMVECTOR lerpQuat = DirectX::XMQuaternionSlerp(quat1, quat2, lerpFactor);

					DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(lerpScale);
					DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(lerpQuat);
					DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(lerpTrans);

					DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

					/*Update the transform for each joint in the skeleton.*/
					localTransforms[jointIndex] = localTransform;
				}
			}
		}
	}

	/*Calculate the final matrices for each joint in the skeleton hierarchy.*/
	CalculateFinalTransform(localTransforms);
}

void AnimationHandler::BlendKeys(std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation, float transitionTime)
{
	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	std::vector<DirectX::XMMATRIX> localTransforms(jointCount);

	for (int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		DirectX::XMVECTOR transAnim1 = blendKeysPerAnimation[0][jointIndex].trans;
		DirectX::XMVECTOR transAnim2 = blendKeysPerAnimation[1][jointIndex].trans;

		DirectX::XMVECTOR scaleAnim1 = blendKeysPerAnimation[0][jointIndex].scale;
		DirectX::XMVECTOR scaleAnim2 = blendKeysPerAnimation[1][jointIndex].scale;

		DirectX::XMVECTOR quatAnim1 = blendKeysPerAnimation[0][jointIndex].quat;
		DirectX::XMVECTOR quatAnim2 = blendKeysPerAnimation[1][jointIndex].quat;

		float blendFactor = transitionTime / m_AnimComponentList[m_AnimCompIndex]->m_TransitionDuration;

		//std::cout << "Transition Blend Factor: " << blendFactor << std::endl;

		DirectX::XMVECTOR lerpBlendTrans = DirectX::XMVectorLerp(transAnim1, transAnim2, blendFactor);
		DirectX::XMVECTOR lerpBlendScale = DirectX::XMVectorLerp(scaleAnim1, scaleAnim2, blendFactor);
		DirectX::XMVECTOR lerpBlendQuat = DirectX::XMQuaternionSlerp(quatAnim1, quatAnim2, blendFactor);

		DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(lerpBlendTrans);
		DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(lerpBlendScale);
		DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(lerpBlendQuat);

		DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

		/*Update the transform for each joint in the skeleton.*/
		localTransforms[jointIndex] = localTransform;
	}

	CalculateFinalTransform(localTransforms);
}

void AnimationHandler::ExtractSourceKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, float sourceTime, float globalTime)
{
	int animStateIndex = m_AnimComponentList[m_AnimCompIndex]->source_State->stateIndex;

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animStateIndex)->GetAllJoints();

	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		BlendKeyframe blendKey;

		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (sourceTime <= m_AnimComponentList[m_AnimCompIndex]->source_State->startTime)
		{
			int startFrame = (int)m_AnimComponentList[m_AnimCompIndex]->source_State->startTime;

			DirectX::XMVECTOR trans = { animatedJoint.keyframes[startFrame].translation[0],
				animatedJoint.keyframes[startFrame].translation[1], animatedJoint.keyframes[startFrame].translation[2] };
			DirectX::XMVECTOR scale = { animatedJoint.keyframes[startFrame].scale[0],
				animatedJoint.keyframes[startFrame].scale[1], animatedJoint.keyframes[startFrame].scale[2] };
			DirectX::XMVECTOR quat = { animatedJoint.keyframes[startFrame].quaternion[0],
				animatedJoint.keyframes[startFrame].quaternion[1], animatedJoint.keyframes[startFrame].quaternion[2],
				animatedJoint.keyframes[startFrame].quaternion[3] };

			blendKey.trans = trans;
			blendKey.scale = scale;
			blendKey.quat = quat;

			blendKeysPerAnimation[0].push_back(blendKey);
		}

		/*The current time is the last keyframe.*/
		else if (sourceTime >= m_AnimComponentList[m_AnimCompIndex]->source_State->endTime)
		{
			int endFrame = animatedJoint.keyframeCount - 1;

			DirectX::XMVECTOR trans = { animatedJoint.keyframes[endFrame].translation[0],
				animatedJoint.keyframes[endFrame].translation[1], animatedJoint.keyframes[endFrame].translation[2] };
			DirectX::XMVECTOR scale = { animatedJoint.keyframes[endFrame].scale[0],
				animatedJoint.keyframes[endFrame].scale[1], animatedJoint.keyframes[endFrame].scale[2] };
			DirectX::XMVECTOR quat = { animatedJoint.keyframes[endFrame].quaternion[0],
				animatedJoint.keyframes[endFrame].quaternion[1], animatedJoint.keyframes[endFrame].quaternion[2],
				animatedJoint.keyframes[endFrame].quaternion[3] };

			blendKey.trans = trans;
			blendKey.scale = scale;
			blendKey.quat = quat;

			blendKeysPerAnimation[0].push_back(blendKey);
		}

		/*The current time is between two keyframes.*/
		else
		{
			int keyFrameCount = animatedJoint.keyframeCount;

			for (int i = 0; i < keyFrameCount; i++)
			{
				float timeKeyframe1 = animatedJoint.keyframes[i].timeValue;
				float timeKeyframe2 = animatedJoint.keyframes[i + 1].timeValue;

				/*Check if the current time is between two keyframes for each joint.*/
				if (sourceTime > timeKeyframe1 && sourceTime < timeKeyframe2)
				{
					/*Lerp factor is calculated for a normalized value between 0-1 for interpolation.*/
					float lerpFactor = (sourceTime - timeKeyframe1) / (timeKeyframe2 - timeKeyframe1);

					DirectX::XMVECTOR trans1 = { animatedJoint.keyframes[i].translation[0],
						animatedJoint.keyframes[i].translation[1], animatedJoint.keyframes[i].translation[2] };
					DirectX::XMVECTOR trans2 = { animatedJoint.keyframes[i + 1].translation[0],
						animatedJoint.keyframes[i + 1].translation[1], animatedJoint.keyframes[i + 1].translation[2] };
					DirectX::XMVECTOR scale1 = { animatedJoint.keyframes[i].scale[0],
						animatedJoint.keyframes[i].scale[1], animatedJoint.keyframes[i].scale[2] };
					DirectX::XMVECTOR scale2 = { animatedJoint.keyframes[i + 1].scale[0],
						animatedJoint.keyframes[i + 1].scale[1], animatedJoint.keyframes[i + 1].scale[2] };
					DirectX::XMVECTOR quat1 = { animatedJoint.keyframes[i].quaternion[0],
						animatedJoint.keyframes[i].quaternion[1], animatedJoint.keyframes[i].quaternion[2],
						animatedJoint.keyframes[i].quaternion[3] };
					DirectX::XMVECTOR quat2 = { animatedJoint.keyframes[i + 1].quaternion[0],
						animatedJoint.keyframes[i + 1].quaternion[1], animatedJoint.keyframes[i + 1].quaternion[2],
						animatedJoint.keyframes[i + 1].quaternion[3] };

					blendKey.trans = DirectX::XMVectorLerp(trans1, trans2, lerpFactor);
					blendKey.scale = DirectX::XMVectorLerp(scale1, scale2, lerpFactor);
					blendKey.quat = DirectX::XMQuaternionSlerp(quat1, quat2, lerpFactor);

					blendKeysPerAnimation[0].push_back(blendKey);
				}
			}
		}
	}

	m_AnimComponentList[m_AnimCompIndex]->source_Time += globalTime;
}

void AnimationHandler::ExtractTargetKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, float targetTime, float globalTime)
{
	int animStateIndex = m_AnimComponentList[m_AnimCompIndex]->target_State->stateIndex;

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animStateIndex)->GetAllJoints();

	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		BlendKeyframe blendKey;

		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (targetTime <= m_AnimComponentList[m_AnimCompIndex]->target_State->startTime)
		{
			int startFrame = (int)m_AnimComponentList[m_AnimCompIndex]->source_State->startTime;

			DirectX::XMVECTOR trans = { animatedJoint.keyframes[startFrame].translation[0],
				animatedJoint.keyframes[startFrame].translation[1], animatedJoint.keyframes[startFrame].translation[2] };
			DirectX::XMVECTOR scale = { animatedJoint.keyframes[startFrame].scale[0],
				animatedJoint.keyframes[startFrame].scale[1], animatedJoint.keyframes[startFrame].scale[2] };
			DirectX::XMVECTOR quat = { animatedJoint.keyframes[startFrame].quaternion[0],
				animatedJoint.keyframes[startFrame].quaternion[1], animatedJoint.keyframes[startFrame].quaternion[2],
				animatedJoint.keyframes[startFrame].quaternion[3] };

			blendKey.trans = trans;
			blendKey.scale = scale;
			blendKey.quat = quat;

			blendKeysPerAnimation[1].push_back(blendKey);
		}

		/*The current time is the last keyframe.*/
		else if (targetTime >= m_AnimComponentList[m_AnimCompIndex]->target_State->endTime)
		{
			int endFrame = animatedJoint.keyframeCount - 1;

			DirectX::XMVECTOR trans = { animatedJoint.keyframes[endFrame].translation[0],
				animatedJoint.keyframes[endFrame].translation[1], animatedJoint.keyframes[endFrame].translation[2] };
			DirectX::XMVECTOR scale = { animatedJoint.keyframes[endFrame].scale[0],
				animatedJoint.keyframes[endFrame].scale[1], animatedJoint.keyframes[endFrame].scale[2] };
			DirectX::XMVECTOR quat = { animatedJoint.keyframes[endFrame].quaternion[0],
				animatedJoint.keyframes[endFrame].quaternion[1], animatedJoint.keyframes[endFrame].quaternion[2],
				animatedJoint.keyframes[endFrame].quaternion[3] };

			blendKey.trans = trans;
			blendKey.scale = scale;
			blendKey.quat = quat;

			blendKeysPerAnimation[1].push_back(blendKey);
		}

		/*The current time is between two keyframes.*/
		else
		{
			int keyFrameCount = animatedJoint.keyframeCount;

			for (int i = 0; i < keyFrameCount; i++)
			{
				float timeKeyframe1 = animatedJoint.keyframes[i].timeValue;
				float timeKeyframe2 = animatedJoint.keyframes[i + 1].timeValue;

				/*Check if the current time is between two keyframes for each joint.*/
				if (targetTime > timeKeyframe1 && targetTime < timeKeyframe2)
				{
					/*Lerp factor is calculated for a normalized value between 0-1 for interpolation.*/
					float lerpFactor = (targetTime - timeKeyframe1) / (timeKeyframe2 - timeKeyframe1);

					DirectX::XMVECTOR trans1 = { animatedJoint.keyframes[i].translation[0],
						animatedJoint.keyframes[i].translation[1], animatedJoint.keyframes[i].translation[2] };
					DirectX::XMVECTOR trans2 = { animatedJoint.keyframes[i + 1].translation[0],
						animatedJoint.keyframes[i + 1].translation[1], animatedJoint.keyframes[i + 1].translation[2] };
					DirectX::XMVECTOR scale1 = { animatedJoint.keyframes[i].scale[0],
						animatedJoint.keyframes[i].scale[1], animatedJoint.keyframes[i].scale[2] };
					DirectX::XMVECTOR scale2 = { animatedJoint.keyframes[i + 1].scale[0],
						animatedJoint.keyframes[i + 1].scale[1], animatedJoint.keyframes[i + 1].scale[2] };
					DirectX::XMVECTOR quat1 = { animatedJoint.keyframes[i].quaternion[0],
						animatedJoint.keyframes[i].quaternion[1], animatedJoint.keyframes[i].quaternion[2],
						animatedJoint.keyframes[i].quaternion[3] };
					DirectX::XMVECTOR quat2 = { animatedJoint.keyframes[i + 1].quaternion[0],
						animatedJoint.keyframes[i + 1].quaternion[1], animatedJoint.keyframes[i + 1].quaternion[2],
						animatedJoint.keyframes[i + 1].quaternion[3] };

					blendKey.trans = DirectX::XMVectorLerp(trans1, trans2, lerpFactor);
					blendKey.scale = DirectX::XMVectorLerp(scale1, scale2, lerpFactor);
					blendKey.quat = DirectX::XMQuaternionSlerp(quat1, quat2, lerpFactor);

					blendKeysPerAnimation[1].push_back(blendKey);
				}
			}
		}
	}

	m_AnimComponentList[m_AnimCompIndex]->target_Time += globalTime;
}

void AnimationHandler::SetAnimationComponent(int animationState, float transitionDuration, Blending blendingType, bool isLooping)
{
	this->m_AnimComponentList[this->m_AnimCompIndex]->m_TransitionDuration = transitionDuration;
	this->m_AnimComponentList[this->m_AnimCompIndex]->target_State = this->m_AnimComponentList[this->m_AnimCompIndex]->
		animation_States->at(animationState)->GetAnimationStateData();
	this->m_AnimComponentList[this->m_AnimCompIndex]->target_State->stateIndex = animationState;
	this->m_AnimComponentList[this->m_AnimCompIndex]->m_TransitionDuration = transitionDuration;
	this->m_AnimComponentList[this->m_AnimCompIndex]->blendFlag = blendingType;
	this->m_AnimComponentList[this->m_AnimCompIndex]->target_State->isLooping = isLooping;
	
}

void AnimationHandler::Blend(float secondsElapsed)
{
	m_AnimComponentList[m_AnimCompIndex]->m_TransitionTimeLeft += secondsElapsed;

	/*If the transition time have reached the duration of the transition.*/
	if (m_AnimComponentList[m_AnimCompIndex]->m_TransitionTimeLeft >= m_AnimComponentList[m_AnimCompIndex]->m_TransitionDuration)
	{
		m_AnimComponentList[m_AnimCompIndex]->m_TransitionComplete = true;
		m_AnimComponentList[m_AnimCompIndex]->m_TransitionTimeLeft = 0;
	}

	/*Transition is still proceeding. Update both animations and blend them.*/
	else
	{
		std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation(2);

		/*Extract source animation's keyframes.*/
		ExtractSourceKeys(blendKeysPerAnimation, m_AnimComponentList[m_AnimCompIndex]->source_Time, secondsElapsed);
		/*Extract target animation's keyframes.*/
		ExtractTargetKeys(blendKeysPerAnimation, m_AnimComponentList[m_AnimCompIndex]->target_Time, secondsElapsed);

		/*Blend the both the two extracted animations' keyframes together.*/
		BlendKeys(blendKeysPerAnimation, m_AnimComponentList[m_AnimCompIndex]->m_TransitionTimeLeft);
	}
}

void AnimationHandler::SetAnimCompIndex(int animCompIndex)
{
	this->m_AnimCompIndex = animCompIndex;
}

void AnimationHandler::CalculateFinalTransform(std::vector<DirectX::XMMATRIX> localTransforms)
{
	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	std::vector<DirectX::XMMATRIX> toRootTransform(jointCount);

	DirectX::XMMATRIX rootLocal = localTransforms[0];

	toRootTransform[0] = rootLocal;

	for (int i = 1; i < jointCount; i++)
	{
		int parentIndex = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->joints[i].parentIndex;

		DirectX::XMMATRIX toParent = localTransforms[i];

		DirectX::XMMATRIX parentToRoot = toRootTransform[parentIndex];

		DirectX::XMMATRIX toRoot = DirectX::XMMatrixMultiply(toParent, parentToRoot);

		toRootTransform[i] = toRoot;
	}

	for (int i = 0; i < jointCount; i++)
	{
		DirectX::XMMATRIX* inverseBindPose = &static_cast<DirectX::XMMATRIX>(m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->joints[i].invBindPose);
		DirectX::XMMATRIX toRoot = toRootTransform[i];

		m_animGraphicsComponents[m_AnimCompIndex]->finalJointTransforms[i] = DirectX::XMMatrixMultiply(*inverseBindPose, toRoot);
	}
}