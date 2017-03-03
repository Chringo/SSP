#include "AnimationHandler.h"

AnimationHandler::AnimationHandler()
{
	
}

AnimationHandler::~AnimationHandler()
{
}

void AnimationHandler::Initialize(GraphicsAnimationComponent ** graphicAnimComponents, int * noActiveComponents)
{
	this->m_animGraphicsComponents = graphicAnimComponents;
	this->m_nrOfGraphicsAnimationComponents = noActiveComponents;

	this->m_nrOfAnimComps = 0;
	this->m_maxAnimComps = 16;

	this->m_AnimCompIndex = 0;

	this->localTransforms.resize(JOINT_COUNT);
	this->localScales.resize(JOINT_COUNT);
	this->relationTransform.resize(JOINT_COUNT);
	blendKeysPerAnimation.resize(2);

	/*Initialize and reserve storage for a total of 16 animation components in the initialize.*/
	for (int i = 0; i < this->m_maxAnimComps; i++)
	{
		this->m_AnimComponentList.push_back(CreateAnimationComponent());
	}
}

void AnimationHandler::Update(float dt)
{
	/*Convert the delta-time to be in seconds unit format.*/
	float seconds = (dt / 1000000.f);

	/*Iterate each component and check if it's active and update animation.*/
	for (int aCompIndex = 0; aCompIndex < this->m_nrOfAnimComps; aCompIndex++)
	{
		/*Go to the next component if the source state at the current frame is a nullptr.*/
		if (this->m_AnimComponentList[aCompIndex]->source_State == nullptr)
			continue;

		if (this->m_AnimComponentList[aCompIndex]->source_State->stateIndex != AnimationStates::RAGDOLL_STATE )
		{
			if (this->m_AnimComponentList[aCompIndex]->target_State != nullptr)
			{
				if (this->m_AnimComponentList[aCompIndex]->target_State->stateIndex == AnimationStates::RAGDOLL_STATE)
				{
					this->m_AnimComponentList[aCompIndex]->target_State = nullptr;
					continue;
				}
			}

		if (this->m_AnimComponentList[m_AnimCompIndex]->active == TRUE && this->m_AnimComponentList[aCompIndex]->source_State != nullptr)
		{
			/*Set the current animation component index.*/
			SetAnimCompIndex(aCompIndex);

			/*If only one animation is playing, there should be no transition.*/
			if (m_AnimComponentList[m_AnimCompIndex]->blendFlag == Blending::NO_TRANSITION)
			{
				/*Increment source animation's local time and multiply by speed factor and velocity.*/
				float playingSpeed = m_AnimComponentList[m_AnimCompIndex]->playingSpeed;
				float velocity = m_AnimComponentList[m_AnimCompIndex]->velocity;
				
				m_AnimComponentList[m_AnimCompIndex]->source_Time += (seconds * velocity) * playingSpeed;
	
				/*If the animation reaches the last frame, either reset animation or switch to two different idle states.*/
				if (m_AnimComponentList[m_AnimCompIndex]->source_Time >= m_AnimComponentList[m_AnimCompIndex]->source_State->endTime)
				{
					/*Animation is looping. Reset the local time of the source animation.*/
					if (m_AnimComponentList[m_AnimCompIndex]->source_State->isLooping == true)
					{
						m_AnimComponentList[m_AnimCompIndex]->source_Time = 0;
					}
					else
					{
						/*If the player picks up the ball, go to player ball idle, otherwise regular idle.*/
						if (m_AnimComponentList[m_AnimCompIndex]->source_State->stateIndex == AnimationStates::PLAYER_PICKUP)
						{
							m_AnimComponentList[m_AnimCompIndex]->previousState = m_AnimComponentList[m_AnimCompIndex]->currentState;
							SetAnimationComponent(AnimationStates::PLAYER_BALL_IDLE, 0.55f, Blending::SMOOTH_TRANSITION, true, false, 0.9f, 1.0f);
							m_AnimComponentList[m_AnimCompIndex]->currentState = AnimationStates::PLAYER_BALL_IDLE;
						}
						else
						{
							m_AnimComponentList[m_AnimCompIndex]->previousState = m_AnimComponentList[m_AnimCompIndex]->currentState;
							SetAnimationComponent(AnimationStates::PLAYER_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true, false, 1.0f, 1.0f);
							m_AnimComponentList[m_AnimCompIndex]->currentState = AnimationStates::PLAYER_IDLE;
						}

						 
					}
				}
				/*Interpolate the keyframes of this animation.*/
				InterpolateKeys(m_AnimComponentList[m_AnimCompIndex]->source_State, m_AnimComponentList[m_AnimCompIndex]->source_Time);
			}

			/*If two animations are playing, there should be a SMOOTH transition or FROZEN transition.*/
			else if (m_AnimComponentList[m_AnimCompIndex]->blendFlag == Blending::SMOOTH_TRANSITION
				|| m_AnimComponentList[m_AnimCompIndex]->blendFlag == Blending::FROZEN_TRANSITION)
			{
				/*Go to the next component if the target state or the source state at the current frame is a nullptr.*/
				if (this->m_AnimComponentList[aCompIndex]->target_State == nullptr 
					&& this->m_AnimComponentList[aCompIndex]->source_State == nullptr)
					continue;

				/*Transition is complete. Swap the animations and remove the old animation.*/
				if (m_AnimComponentList[m_AnimCompIndex]->m_TransitionComplete == true)
				{
					m_AnimComponentList[m_AnimCompIndex]->blendFlag = Blending::NO_TRANSITION;
					m_AnimComponentList[m_AnimCompIndex]->m_TransitionComplete = false;

					/*After the blending is finished, the target state will simply become the new source state.*/
					m_AnimComponentList[m_AnimCompIndex]->source_State = m_AnimComponentList[m_AnimCompIndex]->target_State;
					/*Assign the target local time to source local time.*/
					m_AnimComponentList[m_AnimCompIndex]->source_Time = m_AnimComponentList[m_AnimCompIndex]->target_Time;

					/*Reset target state and it's local time.*/
					m_AnimComponentList[m_AnimCompIndex]->target_State = nullptr;
					m_AnimComponentList[m_AnimCompIndex]->target_Time = 0;
				}

				/*Blending is not complete. Proceed the transition process.*/
				else
					Blend(seconds);
			}
		}
			/*If the component is not active or if there was an error loading the animation, skip this update until further.*/
			else if (this->m_AnimComponentList[m_AnimCompIndex]->active != TRUE )
			{
				continue;
			}
		}
		/*Ragdoll physics is currently happening, continue to the next component in the list.*/
		else
		{
			continue;
		}
	}
}

void AnimationHandler::ClearAnimationComponents()
{
	/*Delete all content in the std::vector and the vector itself is cleared.*/
	for (size_t i = 0; i < this->m_AnimComponentList.size(); i++)
	{
		delete this->m_AnimComponentList[i];
	}

	this->m_maxAnimComps = 16;
	this->m_nrOfAnimComps = 0;
	m_AnimComponentList.clear();
	m_AnimComponentList.shrink_to_fit();

	//Create new empty components
	for (int i = 0; i < this->m_maxAnimComps; i++)
	{
		this->m_AnimComponentList.push_back(CreateAnimationComponent());
	}
}

AnimationComponent* AnimationHandler::CreateAnimationComponent()
{
	/*Creates a empty animation component.*/
	AnimationComponent* animComp = new AnimationComponent; 
	animComp->active = 0;
	animComp->source_State = nullptr;
	animComp->target_State = nullptr;
	animComp->source_Time = 0.f;
	animComp->target_Time = 0.f;
	animComp->transitionDuration = 0.f;
	animComp->transitionTimeLeft = 0.f;
	animComp->blendFlag = Blending::NO_TRANSITION;
	animComp->skeleton = nullptr;
	animComp->animation_States = nullptr;

	return animComp;
}

AnimationComponent * AnimationHandler::GetNextAvailableComponent()
{
	/*Gets the next available empty component in the list. */
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

void AnimationHandler::ShutDown()
{
	/*Delete all content in the std::vector and the vector itself is cleared.*/
	for (size_t i = 0; i < this->m_AnimComponentList.size(); i++)
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

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animState->stateIndex)->GetAllJoints();

	for ( int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is at the first keyframe.*/
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

			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

			DirectX::XMStoreFloat4x4(&this->localScales[jointIndex], scaleMat);
			DirectX::XMStoreFloat4x4(&this->localTransforms[jointIndex], localTransform);
		}
		/*The current time is at the last keyframe.*/
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

			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

			DirectX::XMStoreFloat4x4(&this->localScales[jointIndex], scaleMat);
			DirectX::XMStoreFloat4x4(&this->localTransforms[jointIndex], localTransform);
		}
		/*The current time is at between two keyframes.*/
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

					//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
					DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

					/*Update the local transform for each joint in the skeleton.*/
					DirectX::XMStoreFloat4x4(&this->localTransforms[jointIndex], localTransform);
					/*Storing the scale transformations for handling scaling of joints individually.*/
					DirectX::XMStoreFloat4x4(&this->localScales[jointIndex], scaleMat);
				}
			}
		}
	}

	/*Calculate the final matrices for each joint in the skeleton hierarchy.*/
	CalculateFinalTransform();
}

void AnimationHandler::BlendKeys(float transitionTime)
{
	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	for (int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		DirectX::XMVECTOR transAnim1 = DirectX::XMLoadFloat3(&this->blendKeysPerAnimation[0][jointIndex].trans);
		DirectX::XMVECTOR transAnim2 = DirectX::XMLoadFloat3(&this->blendKeysPerAnimation[1][jointIndex].trans);

		DirectX::XMVECTOR scaleAnim1 = DirectX::XMLoadFloat3(&this->blendKeysPerAnimation[0][jointIndex].scale);
		DirectX::XMVECTOR scaleAnim2 = DirectX::XMLoadFloat3(&this->blendKeysPerAnimation[1][jointIndex].scale);

		DirectX::XMVECTOR quatAnim1 = DirectX::XMLoadFloat4(&this->blendKeysPerAnimation[0][jointIndex].quat);
		DirectX::XMVECTOR quatAnim2 = DirectX::XMLoadFloat4(&this->blendKeysPerAnimation[1][jointIndex].quat);

		/*Normalized blend factor between 0 to 1 is calculated here.*/
		float blendFactor = transitionTime / m_AnimComponentList[m_AnimCompIndex]->transitionDuration;

		DirectX::XMVECTOR lerpBlendTrans = DirectX::XMVectorLerp(transAnim1, transAnim2, blendFactor);
		DirectX::XMVECTOR lerpBlendScale = DirectX::XMVectorLerp(scaleAnim1, scaleAnim2, blendFactor);
		DirectX::XMVECTOR lerpBlendQuat = DirectX::XMQuaternionSlerp(quatAnim1, quatAnim2, blendFactor);

		DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(lerpBlendTrans);
		DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(lerpBlendScale);
		DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(lerpBlendQuat);

		DirectX::XMMATRIX localTransform = XMMatrixMultiply(transMat, quatMat);

		DirectX::XMStoreFloat4x4(&this->localTransforms[jointIndex], localTransform);

		/*Storing the scale transformations for handling scaling of joints individually.*/
		DirectX::XMStoreFloat4x4(&this->localScales[jointIndex], scaleMat);
	}

	CalculateFinalTransform();
}

void AnimationHandler::ExtractSourceKeys(float sourceTime, float globalTime)
{
	int animStateIndex = m_AnimComponentList[m_AnimCompIndex]->source_State->stateIndex;

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animStateIndex)->GetAllJoints();

	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	for ( int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		BlendKeyframe blendKey;

		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is at the first keyframe.*/
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

			DirectX::XMStoreFloat3(&blendKey.trans, trans);
			DirectX::XMStoreFloat3(&blendKey.scale, scale);
			DirectX::XMStoreFloat4(&blendKey.quat, quat);

			this->blendKeysPerAnimation[0].push_back(blendKey);
		}

		/*The current time is at the last keyframe.*/
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

			DirectX::XMStoreFloat3(&blendKey.trans, trans);
			DirectX::XMStoreFloat3(&blendKey.scale, scale);
			DirectX::XMStoreFloat4(&blendKey.quat, quat);

			this->blendKeysPerAnimation[0].push_back(blendKey);
		}

		/*The current time is at between two keyframes.*/
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

					DirectX::XMStoreFloat3(&blendKey.trans, DirectX::XMVectorLerp(trans1, trans2, lerpFactor));
					DirectX::XMStoreFloat3(&blendKey.scale, DirectX::XMVectorLerp(scale1, scale2, lerpFactor));
					DirectX::XMStoreFloat4(&blendKey.quat, DirectX::XMQuaternionSlerp(quat1, quat2, lerpFactor));

					this->blendKeysPerAnimation[0].push_back(blendKey);
				}
			}
		}
	}

	/*If the blending type is Smooth Transition, increment the source time until blending is completed.
	If the blending type is Frozen Transition, let the time be frozen when the blending process started.*/
	if(m_AnimComponentList[m_AnimCompIndex]->blendFlag == Blending::SMOOTH_TRANSITION
		&& m_AnimComponentList[m_AnimCompIndex]->blendFlag != Blending::FROZEN_TRANSITION)
		m_AnimComponentList[m_AnimCompIndex]->source_Time += globalTime;
}

void AnimationHandler::ExtractTargetKeys(float targetTime, float globalTime)
{
	int animStateIndex = m_AnimComponentList[m_AnimCompIndex]->target_State->stateIndex;

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animStateIndex)->GetAllJoints();

	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	for (unsigned int jointIndex = 0; (int)jointIndex < jointCount; jointIndex++)
	{
		BlendKeyframe blendKey;

		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is at the first keyframe.*/
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

			DirectX::XMStoreFloat3(&blendKey.trans, trans);
			DirectX::XMStoreFloat3(&blendKey.scale, scale);
			DirectX::XMStoreFloat4(&blendKey.quat, quat);

			this->blendKeysPerAnimation[1].push_back(blendKey);
		}

		/*The current time is at the last keyframe.*/
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

			DirectX::XMStoreFloat3(&blendKey.trans, trans);
			DirectX::XMStoreFloat3(&blendKey.scale, scale);
			DirectX::XMStoreFloat4(&blendKey.quat, quat);

			this->blendKeysPerAnimation[1].push_back(blendKey);
		}

		/*The current time is at between two keyframes.*/
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

					DirectX::XMStoreFloat3(&blendKey.trans, DirectX::XMVectorLerp(trans1, trans2, lerpFactor));
					DirectX::XMStoreFloat3(&blendKey.scale, DirectX::XMVectorLerp(scale1, scale2, lerpFactor));
					DirectX::XMStoreFloat4(&blendKey.quat, DirectX::XMQuaternionSlerp(quat1, quat2, lerpFactor));

					this->blendKeysPerAnimation[1].push_back(blendKey);
				}
			}
		}
	}
	/*Incrementing the target animation's local time.*/
	m_AnimComponentList[m_AnimCompIndex]->target_Time += globalTime;
}

void AnimationHandler::SetAnimationComponent(int animationState, float transitionDuration, Blending blendingType, bool isLooping, bool lockAnimation, float playingSpeed, float velocity)
{
	/*Sets a current used animation component and updates the information.*/
	this->m_AnimComponentList[this->m_AnimCompIndex]->transitionDuration = transitionDuration;
	this->m_AnimComponentList[this->m_AnimCompIndex]->target_State = this->m_AnimComponentList[this->m_AnimCompIndex]->
		animation_States->at(animationState)->GetAnimationStateData();
	this->m_AnimComponentList[this->m_AnimCompIndex]->target_State->stateIndex = animationState;
	this->m_AnimComponentList[this->m_AnimCompIndex]->blendFlag = blendingType;
	this->m_AnimComponentList[this->m_AnimCompIndex]->target_State->isLooping = isLooping;
	this->m_AnimComponentList[this->m_AnimCompIndex]->playingSpeed = playingSpeed;
	this->m_AnimComponentList[this->m_AnimCompIndex]->lockAnimation = lockAnimation;
	this->m_AnimComponentList[this->m_AnimCompIndex]->velocity = velocity;
	
}

void AnimationHandler::Blend(float secondsElapsed)
{
	/*Increment the transition time remaining with the global time.*/
	m_AnimComponentList[m_AnimCompIndex]->transitionTimeLeft += secondsElapsed;

	/*If the transition time have reached the duration of the transition.*/
	if (m_AnimComponentList[m_AnimCompIndex]->transitionTimeLeft >= m_AnimComponentList[m_AnimCompIndex]->transitionDuration)
	{
		m_AnimComponentList[m_AnimCompIndex]->m_TransitionComplete = true;
		m_AnimComponentList[m_AnimCompIndex]->transitionTimeLeft = 0;
	}

	/*Transition is still proceeding. Update both animations and blend between them.*/
	else
	{
		/*Extract source animation's keyframes.*/
		ExtractSourceKeys(m_AnimComponentList[m_AnimCompIndex]->source_Time, secondsElapsed);
		/*Extract target animation's keyframes.*/
		ExtractTargetKeys(m_AnimComponentList[m_AnimCompIndex]->target_Time, secondsElapsed);

		/*Blend the both the two extracted animations' keyframes together.*/
		BlendKeys(m_AnimComponentList[m_AnimCompIndex]->transitionTimeLeft);
	}
}

void AnimationHandler::SetAnimCompIndex(int animCompIndex)
{
	/*Sets the current animation component index for easier access of different lists in each iteration.*/
	this->m_AnimCompIndex = animCompIndex;
}

void AnimationHandler::CalculateFinalTransform()
{
	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	/*The root joint does not have a parent, which means it's local transform is not changed with a parent/child relation.*/
	if (m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->joints[0].parentIndex == -1)
	{
		this->relationTransform[0] = this->localTransforms[0];
	}

	for (int i = 1; i < jointCount; i++)
	{
		/*Calculates the relation between each child and parent when going to a new pose each frame.*/
		int parentIndex = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->joints[i].parentIndex;

		DirectX::XMMATRIX child = DirectX::XMLoadFloat4x4(&this->localTransforms[i]);

		DirectX::XMMATRIX parent = DirectX::XMLoadFloat4x4(&this->relationTransform[parentIndex]);
		
		DirectX::XMMATRIX toRoot = DirectX::XMMatrixMultiply(child, parent);

		DirectX::XMStoreFloat4x4(&this->relationTransform[i], toRoot);
	}

	for (int i = 0; i < jointCount; i++)
	{
		/*Multiplying the child and parent local matrices with inverse bindpose to go to global space.*/
		DirectX::XMMATRIX* inverseBindPose = &static_cast<DirectX::XMMATRIX>(m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->joints[i].invBindPose);

		DirectX::XMMATRIX toRoot = DirectX::XMLoadFloat4x4(&this->relationTransform[i]);

		/*The scale matrix is applied after the skeleton hierarchy have been traversed, hence we only want to apply scale to an individual joint without inheritance.*/
		DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(
			DirectX::XMVECTOR{ this->localScales[i].m[0][0], this->localScales[i].m[1][1], this->localScales[i].m[2][2]});

		DirectX::XMMATRIX finalMat = DirectX::XMMatrixMultiply(scaleMat, toRoot);

		m_animGraphicsComponents[m_AnimCompIndex]->finalJointTransforms[i] = DirectX::XMMatrixMultiply(*inverseBindPose, finalMat);
	}
}