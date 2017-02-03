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
						/*Push the IDLE state to the stack.*/
						//Push(PLAYER_IDLE, true, 0.5);
						m_AnimComponentList[m_AnimCompIndex]->blendFlag = SMOOTH_TRANSITION;
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
	for (size_t compIndex = 0; compIndex < this->m_AnimComponentList.size(); compIndex++)
	{
		/*If the current iterating component is active, update component and data.*/
		if (this->m_AnimComponentList[compIndex]->active >= 1)  // if active == true
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

	std::vector<DirectX::XMMATRIX> localTransforms(jointCount);

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimComponentList[m_AnimCompIndex]->animation_States->at(animState->stateIndex)->GetAllJoints();

	for ( int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animState->startTime)
		{
			int startFrame = (int)animState->startTime;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[startFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[startFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[startFrame].quaternion);

			DirectX::XMVECTOR trans = XMLoadFloat3(&tempTrans);
			DirectX::XMVECTOR scale = XMLoadFloat3(&tempScale);
			DirectX::XMVECTOR quat = XMLoadFloat4(&tempQuat);

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

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[endFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[endFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[endFrame].quaternion);

			DirectX::XMVECTOR trans = XMLoadFloat3(&tempTrans);
			DirectX::XMVECTOR scale = XMLoadFloat3(&tempScale);
			DirectX::XMVECTOR quat = XMLoadFloat4(&tempQuat);

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

					DirectX::XMFLOAT3 tempTrans1(animatedJoint.keyframes[i].translation);
					DirectX::XMFLOAT3 tempTrans2(animatedJoint.keyframes[i + 1].translation);
					DirectX::XMFLOAT3 tempScale1(animatedJoint.keyframes[i].scale);
					DirectX::XMFLOAT3 tempScale2(animatedJoint.keyframes[i + 1].scale);
					DirectX::XMFLOAT4 tempQuat1(animatedJoint.keyframes[i].quaternion);
					DirectX::XMFLOAT4 tempQuat2(animatedJoint.keyframes[i + 1].quaternion);

					DirectX::XMVECTOR trans1 = DirectX::XMLoadFloat3(&tempTrans1);
					DirectX::XMVECTOR scale1 = DirectX::XMLoadFloat3(&tempScale1);
					DirectX::XMVECTOR quat1 = XMLoadFloat4(&tempQuat1);

					DirectX::XMVECTOR trans2 = DirectX::XMLoadFloat3(&tempTrans2);
					DirectX::XMVECTOR scale2 = DirectX::XMLoadFloat3(&tempScale2);
					DirectX::XMVECTOR quat2 = XMLoadFloat4(&tempQuat2);

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

	for ( int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		BlendKeyframe blendKey;

		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (sourceTime <= m_AnimComponentList[m_AnimCompIndex]->source_State->startTime)
		{
			int startFrame = (int)m_AnimComponentList[m_AnimCompIndex]->source_State->startTime;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[startFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[startFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[startFrame].quaternion);

			blendKey.trans = DirectX::XMLoadFloat3(&tempTrans);
			blendKey.scale = DirectX::XMLoadFloat3(&tempScale);
			blendKey.quat = DirectX::XMLoadFloat4(&tempQuat);

			blendKeysPerAnimation[0].push_back(blendKey);
		}

		/*The current time is the last keyframe.*/
		else if (sourceTime >= m_AnimComponentList[m_AnimCompIndex]->source_State->endTime)
		{
			int endFrame = animatedJoint.keyframeCount - 1;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[endFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[endFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[endFrame].quaternion);

			blendKey.trans = DirectX::XMLoadFloat3(&tempTrans);
			blendKey.scale = DirectX::XMLoadFloat3(&tempScale);
			blendKey.quat = DirectX::XMLoadFloat4(&tempQuat);

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

					DirectX::XMFLOAT3 tempTrans1(animatedJoint.keyframes[i].translation);
					DirectX::XMFLOAT3 tempTrans2(animatedJoint.keyframes[i + 1].translation);
					DirectX::XMFLOAT3 tempScale1(animatedJoint.keyframes[i].scale);
					DirectX::XMFLOAT3 tempScale2(animatedJoint.keyframes[i + 1].scale);
					DirectX::XMFLOAT4 tempQuat1(animatedJoint.keyframes[i].quaternion);
					DirectX::XMFLOAT4 tempQuat2(animatedJoint.keyframes[i + 1].quaternion);

					DirectX::XMVECTOR trans1 = DirectX::XMLoadFloat3(&tempTrans1);
					DirectX::XMVECTOR scale1 = DirectX::XMLoadFloat3(&tempScale1);
					DirectX::XMVECTOR quat1 = DirectX::XMLoadFloat4(&tempQuat1);

					DirectX::XMVECTOR trans2 = DirectX::XMLoadFloat3(&tempTrans2);
					DirectX::XMVECTOR scale2 = DirectX::XMLoadFloat3(&tempScale2);
					DirectX::XMVECTOR quat2 = DirectX::XMLoadFloat4(&tempQuat2);

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

	for (unsigned int jointIndex = 0; (int)jointIndex < jointCount; jointIndex++)
	{
		BlendKeyframe blendKey;

		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (targetTime <= m_AnimComponentList[m_AnimCompIndex]->target_State->startTime)
		{
			int startFrame = (int)m_AnimComponentList[m_AnimCompIndex]->source_State->startTime;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[startFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[startFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[startFrame].quaternion);

			blendKey.trans = DirectX::XMLoadFloat3(&tempTrans);
			blendKey.scale = DirectX::XMLoadFloat3(&tempScale);
			blendKey.quat = DirectX::XMLoadFloat4(&tempQuat);

			blendKeysPerAnimation[1].push_back(blendKey);
		}

		/*The current time is the last keyframe.*/
		else if (targetTime >= m_AnimComponentList[m_AnimCompIndex]->target_State->endTime)
		{
			int endFrame = animatedJoint.keyframeCount - 1;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[endFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[endFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[endFrame].quaternion);

			blendKey.trans = DirectX::XMLoadFloat3(&tempTrans);
			blendKey.scale = DirectX::XMLoadFloat3(&tempScale);
			blendKey.quat = DirectX::XMLoadFloat4(&tempQuat);

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

					DirectX::XMFLOAT3 tempTrans1(animatedJoint.keyframes[i].translation);
					DirectX::XMFLOAT3 tempTrans2(animatedJoint.keyframes[i + 1].translation);
					DirectX::XMFLOAT3 tempScale1(animatedJoint.keyframes[i].scale);
					DirectX::XMFLOAT3 tempScale2(animatedJoint.keyframes[i + 1].scale);
					DirectX::XMFLOAT4 tempQuat1(animatedJoint.keyframes[i].quaternion);
					DirectX::XMFLOAT4 tempQuat2(animatedJoint.keyframes[i + 1].quaternion);

					DirectX::XMVECTOR trans1 = DirectX::XMLoadFloat3(&tempTrans1);
					DirectX::XMVECTOR scale1 = DirectX::XMLoadFloat3(&tempScale1);
					DirectX::XMVECTOR quat1 = DirectX::XMLoadFloat4(&tempQuat1);

					DirectX::XMVECTOR trans2 = DirectX::XMLoadFloat3(&tempTrans2);
					DirectX::XMVECTOR scale2 = DirectX::XMLoadFloat3(&tempScale2);
					DirectX::XMVECTOR quat2 = DirectX::XMLoadFloat4(&tempQuat2);

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