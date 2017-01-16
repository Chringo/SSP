#include "AnimationHandler.h"

AnimationHandler::AnimationHandler()
{
	/*No good practice but will work for now. Need to get the model ID from GraphicsAnimationComponent or AnimationComponent.*/
	Resources::Model* model = GetAnimatedModel(1337);

	/*Skeleton that is contained in the specified model.*/
	Resources::Skeleton* skeleton = GetSkeleton(model);

	/*Gets all animations and saves them in a container to use for playing and blending animations.*/
	GetAnimations(skeleton);

	this->m_globalTimeElapsed = 0.0f;
	this->m_graphicsAnimationComponent = new GraphicsAnimationComponent;
	this->m_graphicsAnimationComponent->jointCount = 19;

	this->m_graphicsAnimationComponent->worldMatrix = DirectX::XMMatrixTranslation(0, 4, 10);
	for (int i = 0; i < 32; i++)
	{
		m_graphicsAnimationComponent->finalJointTransforms[i] = DirectX::XMMatrixIdentity();
	}

	m_TransitionTimeLeft = 0;
	m_TransitionDuration = 0;
	
	/*Initialize the stack with a default "IDLE" animation.*/
	Push(0, true, 0);
}

AnimationHandler::~AnimationHandler()
{
	delete m_graphicsAnimationComponent;
}

void AnimationHandler::AddAnimation(int animationState, bool isLooping, float transitionTime)
{
	if (m_animationStack.front().animationState == animationState)
	{
		/*Don't do anything. Animation already exists and plays.*/
	}

	else
	{
		/*Animation is new. Add it to the stack.*/
		Push(animationState, isLooping, transitionTime);
	}

}

void AnimationHandler::Update(float dt)
{
	/*Convert the delta-time to be in seconds unit format.*/
	float seconds = dt / 1000000;

	m_globalTimeElapsed += seconds;

	switch (m_BlendState)
	{
		/*If only one animation is playing, there should be no transition.*/
		case (BlendingStates::NO_TRANSITION):
		{
			if (!m_animationStack.empty())
			{
				m_animationStack.front().localTime += seconds;
			}

			/*If the animation reaches the last frame, either reset animation or switch to idle state.*/
			if (m_animationStack.front().localTime >= m_animationStack.front().endFrame)
			{
				/*Animation is looping. Reset the time of the animation.*/
				if (m_animationStack.front().isLooping == true)
				{
					m_animationStack.front().localTime = 0;
				}

				else
				{
					/*Push the IDLE state to the stack.*/
					Push(IDLE_STATE, true, 1);
					m_BlendState = SMOOTH_TRANSITION;
					break;
				}
			}

			/*Interpolate the keyframes of this animation.*/
			InterpolateKeys(m_animationStack.front(), m_animationStack.front().localTime);
				
			break;
		}

		/*If two animations are playing, there should be a smooth transition.*/
		case (BlendingStates::SMOOTH_TRANSITION):
		{
			/*Transition is complete. Swap the animations and remove the old animation.*/
			if (m_TransitionComplete == true)
			{
				m_BlendState = NO_TRANSITION;
				m_TransitionComplete = false;
				
				Pop();
			}

			/*Blending is not complete. Proceed the transition process.*/
			else
			{
				Blend(seconds);
			}

			break;
		}
	}

	///*Check if there is a current animation in the stack.*/
	//if (!m_animationStack.empty())
	//{
	//	/*Increment elapsedTime with the delta-time. Consider to multiply framerate here?*/
	//	float toSeconds = dt / 1000000;
	//	m_globalTimeElapsed += toSeconds;

	//	/*If the animation reaches the last frame and the animation is looping, reset elapsedTime to ZERO.*/
	//	if (m_globalTimeElapsed >= m_animationStack.front().endFrame && m_animationStack.front().isLooping == true)
	//	{
	//		m_globalTimeElapsed = 0.0f;
	//	}

	//	InterpolateKeys(m_animationStack.front(), m_globalTimeElapsed);
	//}
}

void AnimationHandler::InterpolateKeys(AnimationClip animationClip, float currentTime)
{
	std::vector<DirectX::XMFLOAT4X4> localTransforms;

	int jointSize = m_skeletonContainer.size();

	localTransforms.resize(jointSize);

	int animationState = animationClip.animationState;

	const Resources::Animation::AnimationJoint* animatedJoints = m_animationContainer[animationState];

	for (unsigned int jointIndex = 0; jointIndex < jointSize; jointIndex++)
	{
		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animationClip.startFrame)
		{
			int startFrame = (int)animationClip.startFrame;

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

			DirectX::XMStoreFloat4x4(&localTransforms[jointIndex], localTransform);
		}
		/*The current time is the last keyframe.*/
		else if (currentTime >= animationClip.endFrame)
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

			DirectX::XMStoreFloat4x4(&localTransforms[jointIndex], localTransform);
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
					DirectX::XMStoreFloat4x4(&localTransforms[jointIndex], localTransform);
				}
			}
		}
	}

	/*Calculate the final matrices for each joint in the skeleton hierarchy.*/
	CalculateFinalTransform(localTransforms);
}

void AnimationHandler::ExtractBlendingKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, AnimationClip animationClip, float currentTime, int animIndex)
{
	int jointCount = m_skeletonContainer.size();

	int animationState = animationClip.animationState;

	const Resources::Animation::AnimationJoint* animatedJoints = m_animationContainer[animationState];

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		BlendKeyframe blendKey;

		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animationClip.startFrame)
		{
			int startFrame = (int)animationClip.startFrame;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[startFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[startFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[startFrame].quaternion);

			blendKey.trans = DirectX::XMLoadFloat3(&tempTrans);
			blendKey.scale = DirectX::XMLoadFloat3(&tempScale);
			blendKey.quat = DirectX::XMLoadFloat4(&tempQuat);

			blendKeysPerAnimation[animIndex].push_back(blendKey);
		}

		/*The current time is the last keyframe.*/
		else if (currentTime >= animationClip.endFrame)
		{
			int endFrame = animatedJoint.keyframeCount - 1;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[endFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[endFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[endFrame].quaternion);

			blendKey.trans = DirectX::XMLoadFloat3(&tempTrans);
			blendKey.scale = DirectX::XMLoadFloat3(&tempScale);
			blendKey.quat = DirectX::XMLoadFloat4(&tempQuat);

			blendKeysPerAnimation[animIndex].push_back(blendKey);
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
					DirectX::XMVECTOR quat1 = DirectX::XMLoadFloat4(&tempQuat1);

					DirectX::XMVECTOR trans2 = DirectX::XMLoadFloat3(&tempTrans2);
					DirectX::XMVECTOR scale2 = DirectX::XMLoadFloat3(&tempScale2);
					DirectX::XMVECTOR quat2 = DirectX::XMLoadFloat4(&tempQuat2);

					blendKey.trans = DirectX::XMVectorLerp(trans1, trans2, lerpFactor);
					blendKey.scale = DirectX::XMVectorLerp(scale1, scale2, lerpFactor);
					blendKey.quat = DirectX::XMQuaternionSlerp(quat1, quat2, lerpFactor);

					blendKeysPerAnimation[animIndex].push_back(blendKey);
				}
			}
		}
	}
}

void AnimationHandler::BlendKeys(std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation, float transitionTime)
{
	std::vector<DirectX::XMFLOAT4X4> localTransforms;

	int jointSize = m_skeletonContainer.size();

	localTransforms.resize(jointSize);

	for (int jointIndex = 0; jointIndex < jointSize; jointIndex++)
	{
		DirectX::XMVECTOR transAnim1 = blendKeysPerAnimation[0][jointIndex].trans;
		DirectX::XMVECTOR transAnim2 = blendKeysPerAnimation[1][jointIndex].trans;

		DirectX::XMVECTOR scaleAnim1 = blendKeysPerAnimation[0][jointIndex].scale;
		DirectX::XMVECTOR scaleAnim2 = blendKeysPerAnimation[1][jointIndex].scale;

		DirectX::XMVECTOR quatAnim1 = blendKeysPerAnimation[0][jointIndex].quat;
		DirectX::XMVECTOR quatAnim2 = blendKeysPerAnimation[1][jointIndex].quat;

		float weightA = 1.0f - (transitionTime / m_TransitionDuration);
		float weightB = transitionTime / m_TransitionDuration;

		DirectX::XMVECTOR lerpBlendTrans = DirectX::XMVectorLerp(transAnim1, transAnim2, weightA + weightA);
		DirectX::XMVECTOR lerpBlendScale = DirectX::XMVectorLerp(scaleAnim1, scaleAnim2, weightA + weightA);
		DirectX::XMVECTOR lerpBlendQuat = DirectX::XMQuaternionSlerp(quatAnim1, quatAnim2, weightA + weightA);

		DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(lerpBlendTrans);
		DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(lerpBlendScale);
		DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(lerpBlendQuat);

		DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

		/*Update the transform for each joint in the skeleton.*/
		DirectX::XMStoreFloat4x4(&localTransforms[jointIndex], localTransform);
	}

	CalculateFinalTransform(localTransforms);
}

void AnimationHandler::Blend(float secondsElapsed)
{
	m_TransitionTimeLeft += secondsElapsed;

	/*If the transition time have reached the duration of the transition.*/
	if (m_TransitionTimeLeft >= m_TransitionDuration)
	{
		m_TransitionComplete = true;
		m_TransitionTimeLeft = 0;
	}

	/*Transition is still proceeding. Update both animations and blend them.*/
	else
	{
		std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation;

		blendKeysPerAnimation.resize(2);

		for (int animClipIndex = 0; animClipIndex < m_animationStack.size(); animClipIndex++)
		{
			if (m_animationStack[animClipIndex].localTime <= GetStartFrame(animClipIndex))
			{
				m_animationStack[animClipIndex].localTime = GetStartFrame(animClipIndex);
			}

			else if (m_animationStack[animClipIndex].localTime >= GetEndFrame(animClipIndex))
			{
				m_animationStack[animClipIndex].localTime = GetEndFrame(animClipIndex);
			}

			else if (m_animationStack[animClipIndex].localTime > GetStartFrame(animClipIndex) && m_animationStack[animClipIndex].localTime < GetEndFrame(animClipIndex))
			{
				m_animationStack[animClipIndex].localTime += secondsElapsed;
			}

			ExtractBlendingKeys(blendKeysPerAnimation, m_animationStack[animClipIndex], m_animationStack[animClipIndex].localTime, animClipIndex);
		}

		/*Calculate the blend time with the delta-time.*/

		BlendKeys(blendKeysPerAnimation, m_TransitionTimeLeft);
	}
}

void AnimationHandler::CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localTransforms)
{
	int jointCount = m_skeletonContainer.size();

	std::vector<DirectX::XMFLOAT4X4> toRootTransform(jointCount);

	DirectX::XMMATRIX childLocal = DirectX::XMLoadFloat4x4(&localTransforms[0]);

	DirectX::XMStoreFloat4x4(&toRootTransform[0], childLocal);

	for (int i = 1; i < jointCount; i++)
	{
		int parentIndex = m_skeletonContainer[i].parentIndex;

		DirectX::XMMATRIX toParent = DirectX::XMLoadFloat4x4(&localTransforms[i]);

		DirectX::XMMATRIX parentToRoot = DirectX::XMLoadFloat4x4(&toRootTransform[parentIndex]);

		DirectX::XMMATRIX toRoot = DirectX::XMMatrixMultiply(toParent, parentToRoot);

		DirectX::XMStoreFloat4x4(&toRootTransform[i], toRoot);
	}

	for (int i = 0; i < jointCount; i++)
	{
		DirectX::XMMATRIX invBindPose = m_skeletonContainer[i].invBindPose;
		DirectX::XMMATRIX toRoot = DirectX::XMLoadFloat4x4(&toRootTransform[i]);

		m_graphicsAnimationComponent->finalJointTransforms[i] = DirectX::XMMatrixMultiply(invBindPose, toRoot);
	}
}

void AnimationHandler::Push(int animationState, bool isLooping, float transitionTime)
{
	AnimationClip clip;

	clip.animationState = animationState;

	/*First time pushing to the stack, the stack should be empty, thus previous state is none.*/
	if (m_animationStack.empty())
		clip.previousState = -1;

	else
	{
		clip.previousState = m_animationStack.front().animationState;
		
		//Pop(); // Comment out when writing blend code. 
	}

	/*The animations are the same, no transition will be made.*/
	if (clip.previousState == -1 || clip.previousState == clip.animationState)
	{
		m_BlendState = BlendingStates::NO_TRANSITION;
	}


	/*The animations are different, smooth transition will be made.*/
	else
	{
		m_TransitionDuration = transitionTime;
		m_BlendState = BlendingStates::SMOOTH_TRANSITION;
	}
		
	clip.startFrame = GetStartFrame(animationState);
	clip.endFrame = GetEndFrame(animationState);
	clip.isLooping = isLooping;
	clip.localTime = 0;

	m_animationStack.push_back(clip);
}

void AnimationHandler::Pop()
{
	/*Function swaps the two elements in the vector and removes the last element in vector.*/
	if (!m_animationStack.empty())
	{
		std::swap(m_animationStack.front(), m_animationStack.back());
		m_animationStack.pop_back();
		m_animationStack.shrink_to_fit();
	}
}

Resources::Model * AnimationHandler::GetAnimatedModel(int modelId)
{
	Resources::Model* model;
	Resources::ResourceHandler::GetInstance()->GetModel(UINT(modelId), model);

	if (model == nullptr)
		return nullptr;

	else
		return model;
}

Resources::Skeleton * AnimationHandler::GetSkeleton(Resources::Model * model)
{
	if (model == nullptr)
		return nullptr;

	else
	{
		Resources::Skeleton* skeleton = model->GetSkeleton();

		int jointCount = skeleton->GetSkeletonData()->jointCount;

		Resources::Skeleton::Joint* jointList = skeleton->GetSkeletonData()->joints;

		for (int jointIndex = 0; jointIndex < jointCount; jointIndex++)
		{
			DirectX::XMMATRIX matrix = DirectX::XMMATRIX(jointList[jointIndex].invBindPose);

			SkeletonTemp skelTemp;
			skelTemp.jointIndex = jointList[jointIndex].jointIndex;
			skelTemp.parentIndex = jointList[jointIndex].parentIndex;
			skelTemp.invBindPose = matrix;

			m_skeletonContainer.push_back(skelTemp);
		}

		return skeleton;
	}
}

Resources::Animation * AnimationHandler::GetAnimations(Resources::Skeleton * skeleton)
{
	if(skeleton == nullptr)
		return nullptr;

	else
	{
		int numAnimations = skeleton->GetNumAnimations();

		m_animationContainer.resize(numAnimations);

		for (int animIndex = 0; animIndex < numAnimations; animIndex++)
		{
			const Resources::Animation* animation = skeleton->GetAnimation(animIndex);

			if (animation == nullptr)
				return nullptr;

			else
			{
				const Resources::Animation::AnimationJoint* animationJoint = animation->GetAllJoints();
				m_animationContainer[animIndex] = animationJoint;
			}
		}
	}
}

float AnimationHandler::GetStartFrame(int animationState)
{
	float startFrame = m_animationContainer[animationState]->keyframes->timeValue;
	return startFrame;
}

float AnimationHandler::GetEndFrame(int animationState)
{
	int keyframeCount = m_animationContainer[animationState]->keyframeCount;
	float endFrame = m_animationContainer[animationState]->keyframes[keyframeCount - 1].timeValue;
	return endFrame;
}