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

	/*Initialize and reserve storage for a total of 16 animation components in the initialize.*/
	for (int i = 0; i < this->m_maxAnimComps; i++)
	{
		this->m_AnimComponentList.push_back(CreateAnimationComponent());
	}
}

void AnimationHandler::Update(float dt)
{
	/*Convert the delta-time to be in seconds unit format.*/

	float speed = 2.0f;

	float seconds = (dt / 1000000.f) * speed;

	/*Iterate each component and check if it's active and update animation.*/
	for (int aCompIndex = 0; aCompIndex < this->m_nrOfAnimComps; aCompIndex++)
	{
		if (this->m_AnimComponentList[m_AnimCompIndex]->active == TRUE)
		{
			/*Set the current animation component index.*/
			SetAnimCompIndex(aCompIndex);

			/*If only one animation is playing, there should be no transition.*/
			if (m_AnimComponentList[m_AnimCompIndex]->blendFlag == Blending::NO_TRANSITION)
			{
				/*If there is no source state, the program should make a assert.*/
				assert(m_AnimComponentList[m_AnimCompIndex]->source_State != nullptr);

				/*Increment source animation's local time.*/
				m_AnimComponentList[m_AnimCompIndex]->source_Time += seconds;

				/*If the animation reaches the last frame, either reset animation or switch to two different idle states.*/
				if (m_AnimComponentList[m_AnimCompIndex]->source_Time >= m_AnimComponentList[m_AnimCompIndex]->source_State->endTime)
				{
					/*Animation is looping. Reset the local time of the source animation.*/
					if (m_AnimComponentList[m_AnimCompIndex]->source_State->isLooping == TRUE)
					{
						m_AnimComponentList[m_AnimCompIndex]->source_Time = 0;
					}
					else
					{
						/*If the player picks up the ball, go to player ball idle, otherwise regular idle.*/
						if (m_AnimComponentList[m_AnimCompIndex]->source_State->stateIndex == PLAYER_PICKUP)
						{
							//SetAnimationComponent(PLAYER_BALL_IDLE, 0.5f, SMOOTH_TRANSITION, true);
							//m_AnimComponentList[m_AnimCompIndex]->blendFlag = SMOOTH_TRANSITION;
							SetAnimationComponent(PLAYER_IDLE, 0.5f, SMOOTH_TRANSITION, true);
							m_AnimComponentList[m_AnimCompIndex]->blendFlag = SMOOTH_TRANSITION;
						}
						else
						{
							SetAnimationComponent(PLAYER_IDLE, 0.5f, SMOOTH_TRANSITION, true);
							m_AnimComponentList[m_AnimCompIndex]->blendFlag = SMOOTH_TRANSITION;
						}
						/*All animations that are not looping gets it lock released, other animations can now contribute.*/
						m_AnimComponentList[m_AnimCompIndex]->lockAnimation = false;
					}
				}
				/*Interpolate the keyframes of this animation.*/
				InterpolateKeys(m_AnimComponentList[m_AnimCompIndex]->source_State, m_AnimComponentList[m_AnimCompIndex]->source_Time);
			}

			/*If two animations are playing, there should be a SMOOTH transition or FROZEN transition.*/
			else if (m_AnimComponentList[m_AnimCompIndex]->blendFlag == Blending::SMOOTH_TRANSITION
				|| m_AnimComponentList[m_AnimCompIndex]->blendFlag == Blending::FROZEN_TRANSITION)
			{
				/*Transition is complete. Swap the animations and remove the old animation.*/
				if (m_AnimComponentList[m_AnimCompIndex]->m_TransitionComplete == true)
				{
					m_AnimComponentList[m_AnimCompIndex]->blendFlag = NO_TRANSITION;
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
	animComp->m_TransitionDuration = 0.f;
	animComp->m_TransitionTimeLeft = 0.f;
	animComp->blendFlag = NO_TRANSITION;
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
	/*Delete all content in the std::vector and the vector itself is cleared.*/
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

	std::vector<DirectX::XMFLOAT4X4> localTransforms(jointCount);

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animState->stateIndex)->GetAllJoints();

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
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

			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

			DirectX::XMStoreFloat4x4(&localTransforms[jointIndex], localTransform);
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

			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

			DirectX::XMStoreFloat4x4(&localTransforms[jointIndex], localTransform);
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

					DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
					//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

					/*Update the local transform for each joint in the skeleton.*/
					DirectX::XMStoreFloat4x4(&localTransforms[jointIndex], localTransform);
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

	std::vector<DirectX::XMFLOAT4X4> localTransforms(jointCount);

	for (int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		DirectX::XMVECTOR transAnim1 = DirectX::XMLoadFloat3(&blendKeysPerAnimation[0][jointIndex].trans);
		DirectX::XMVECTOR transAnim2 = DirectX::XMLoadFloat3(&blendKeysPerAnimation[1][jointIndex].trans);

		DirectX::XMVECTOR scaleAnim1 = DirectX::XMLoadFloat3(&blendKeysPerAnimation[0][jointIndex].scale);
		DirectX::XMVECTOR scaleAnim2 = DirectX::XMLoadFloat3(&blendKeysPerAnimation[1][jointIndex].scale);

		DirectX::XMVECTOR quatAnim1 = DirectX::XMLoadFloat4(&blendKeysPerAnimation[0][jointIndex].quat);
		DirectX::XMVECTOR quatAnim2 = DirectX::XMLoadFloat4(&blendKeysPerAnimation[1][jointIndex].quat);

		/*Normalized blend factor between 0 to 1 is calculated here.*/
		float blendFactor = transitionTime / m_AnimComponentList[m_AnimCompIndex]->m_TransitionDuration;

		DirectX::XMVECTOR lerpBlendTrans = DirectX::XMVectorLerp(transAnim1, transAnim2, blendFactor);
		DirectX::XMVECTOR lerpBlendScale = DirectX::XMVectorLerp(scaleAnim1, scaleAnim2, blendFactor);
		DirectX::XMVECTOR lerpBlendQuat = DirectX::XMQuaternionSlerp(quatAnim1, quatAnim2, blendFactor);

		DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(lerpBlendTrans);
		DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(lerpBlendScale);
		DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(lerpBlendQuat);

		DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

		DirectX::XMStoreFloat4x4(&localTransforms[jointIndex], localTransform);
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

			blendKeysPerAnimation[0].push_back(blendKey);
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

			blendKeysPerAnimation[0].push_back(blendKey);
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

					blendKeysPerAnimation[0].push_back(blendKey);
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

void AnimationHandler::ExtractTargetKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, float targetTime, float globalTime)
{
	int animStateIndex = m_AnimComponentList[m_AnimCompIndex]->target_State->stateIndex;

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animStateIndex)->GetAllJoints();

	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
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

			blendKeysPerAnimation[1].push_back(blendKey);
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

			blendKeysPerAnimation[1].push_back(blendKey);
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

					blendKeysPerAnimation[1].push_back(blendKey);
				}
			}
		}
	}
	/*Incrementing the target animation's local time.*/
	m_AnimComponentList[m_AnimCompIndex]->target_Time += globalTime;
}

void AnimationHandler::SetAnimationComponent(int animationState, float transitionDuration, Blending blendingType, bool isLooping)
{
	/*Sets a current used animation component and updates the information.*/
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
	/*Increment the transition time remaining with the global time.*/
	m_AnimComponentList[m_AnimCompIndex]->m_TransitionTimeLeft += secondsElapsed;

	/*If the transition time have reached the duration of the transition.*/
	if (m_AnimComponentList[m_AnimCompIndex]->m_TransitionTimeLeft >= m_AnimComponentList[m_AnimCompIndex]->m_TransitionDuration)
	{
		m_AnimComponentList[m_AnimCompIndex]->m_TransitionComplete = true;
		m_AnimComponentList[m_AnimCompIndex]->m_TransitionTimeLeft = 0;
	}

	/*Transition is still proceeding. Update both animations and blend between them.*/
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
	/*Sets the current animation component index for easier access of different lists in each iteration.*/
	this->m_AnimCompIndex = animCompIndex;
}

void AnimationHandler::CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localTransforms)
{
	int jointCount = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->jointCount;

	std::vector<DirectX::XMFLOAT4X4> toRootTransform(jointCount);

	/*The root joint does not have a parent, which means it's local transform is not changed with a parent/child relation.*/
	if (m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->joints[0].parentIndex == -1)
	{
		toRootTransform[0] = localTransforms[0];
		//DirectX::XMStoreFloat4x4(&toRootTransform[0], DirectX::XMMatrixIdentity());
	}

	for (int i = 1; i < jointCount; i++)
	{
		/*Calculates the relation between each child and parent when going to a new pose each frame.*/
		int parentIndex = m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->joints[i].parentIndex;

		DirectX::XMMATRIX toParent = DirectX::XMLoadFloat4x4(&localTransforms[i]);

		DirectX::XMMATRIX parentToRoot = DirectX::XMLoadFloat4x4(&toRootTransform[parentIndex]);

		DirectX::XMMATRIX toRoot = DirectX::XMMatrixMultiply(toParent, parentToRoot);
		
		DirectX::XMStoreFloat4x4(&toRootTransform[i], toRoot);
	}

	for (int i = 0; i < jointCount; i++)
	{
		/*Multiplying the child and parent local matrices with inverse bindpose to go to global space.*/
		DirectX::XMMATRIX* inverseBindPose = &static_cast<DirectX::XMMATRIX>(m_AnimComponentList[m_AnimCompIndex]->skeleton->GetSkeletonData()->joints[i].invBindPose);
		DirectX::XMMATRIX toRoot = DirectX::XMLoadFloat4x4(&toRootTransform[i]);

		m_animGraphicsComponents[m_AnimCompIndex]->finalJointTransforms[i] = DirectX::XMMatrixMultiply(*inverseBindPose, toRoot);
	}
}