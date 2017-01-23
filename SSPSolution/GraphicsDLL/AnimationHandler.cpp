#include "AnimationHandler.h"

int AnimationHandler::IncreaseArraySize()
{
	AnimationComponent** newArray = new AnimationComponent*[this->m_maxAnimationComponents + MAXIMUM_ALLOCATION];

	for (int i = 0; i < this->m_maxAnimationComponents + MAXIMUM_ALLOCATION; i++)
	{
		if (i < this->m_nrOfAnimationComponents)
		{
			newArray[i] = this->m_AnimationComponents[i];
		}
		
		else
		{
			newArray[i] = new AnimationComponent();
		}
	}
	delete[] this->m_AnimationComponents;
	this->m_AnimationComponents = newArray;
	this->m_maxAnimationComponents += MAXIMUM_ALLOCATION;

	return 1;
}

int AnimationHandler::IncreaseArraySize(int increaseTo)
{
	AnimationComponent** newArray = new AnimationComponent*[increaseTo];

	for (int i = 0; i < increaseTo; i++)
	{
		if (i < this->m_nrOfAnimationComponents)
		{
			newArray[i] = this->m_AnimationComponents[i];
		}

		else
		{
			newArray[i] = nullptr;
		}
	}
	delete[] this->m_AnimationComponents;
	this->m_AnimationComponents = newArray;
	this->m_maxAnimationComponents = increaseTo;

	return 1;
}

int AnimationHandler::DecreaseArraySize()
{
	this->m_maxAnimationComponents -= MAXIMUM_ALLOCATION;
	AnimationComponent** newArray = new AnimationComponent*[this->m_maxAnimationComponents];

	for (int i = 0; i < this->m_maxAnimationComponents; i++)
	{
		if (i < this->m_nrOfAnimationComponents)
		{
			newArray[i] = this->m_AnimationComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
	}

	for (int i = this->m_maxAnimationComponents; i < this->m_maxAnimationComponents + MAXIMUM_ALLOCATION; i++)
	{
		if (this->m_AnimationComponents[i])
		{
			delete this->m_AnimationComponents[i];
		}
	}

	delete[] this->m_AnimationComponents;
	this->m_AnimationComponents = newArray;

	return 1;
}

int AnimationHandler::DecreaseArraySize(int decreaseTo)
{
	AnimationComponent** newArray = new AnimationComponent*[decreaseTo];

	for (int i = 0; i < decreaseTo; i++)
	{
		if (i < this->m_nrOfAnimationComponents)
		{
			newArray[i] = this->m_AnimationComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
	}

	for (int i = decreaseTo; i < this->m_maxAnimationComponents; i++)
	{
		if (this->m_AnimationComponents[i])
		{
			delete this->m_AnimationComponents[i];
		}
	}

	delete[] this->m_AnimationComponents;
	this->m_AnimationComponents = newArray;
	this->m_maxAnimationComponents = decreaseTo;

	return 1;
}

AnimationHandler::AnimationHandler()
{
	this->m_BlendState = NO_TRANSITION;
	this->m_TransitionComplete = false;
	this->m_globalTimeElapsed = 0;
	this->m_TransitionDuration = 0;
	this->m_TransitionTimeLeft = 0;

	this->m_animGraphicsComponents = nullptr;
	this->m_nrOfAnimationComponents = 0;
	this->m_maxAnimationComponents = 5;
}

AnimationHandler::AnimationHandler(GraphicsAnimationComponent ** graphicAnimComponents, int * noActiveComponents)
{
	/*Initialize all Graphics Animation Components arrays with joint matrices for each skeleton.*/
	this->m_animGraphicsComponents = graphicAnimComponents;
	this->m_nrOfGraphicsAnimationComponents = noActiveComponents;
	
	//for (int i = 0; i < this->m_nrOfGraphicsAnimationComponents[i]; i++)
	//{

	//}
}

AnimationHandler::~AnimationHandler()
{
	//delete m_graphicsAnimationComponent;
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
			if (!m_AnimationComponentStack.empty())
			{
				m_AnimationComponentStack.front()->localTime += seconds;
			}

			/*If the animation reaches the last frame, either reset animation or switch to idle state.*/
			if (m_AnimationComponentStack.front()->localTime >= m_AnimationComponentStack.front()->endFrame)
			{
				/*Animation is looping. Reset the time of the animation.*/
				if (m_AnimationComponentStack.front()->isLooping == true)
				{
					m_AnimationComponentStack.front()->localTime = 0;
				}

				else
				{
					/*Push the IDLE state to the stack.*/
					//Push(PLAYER_IDLE, true, 0.5);
					m_BlendState = SMOOTH_TRANSITION;
					break;
				}
			}

			/*Interpolate the keyframes of this animation.*/
			InterpolateKeys(m_AnimationComponentStack.front(), m_AnimationComponentStack.front()->localTime);
				
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
}

void AnimationHandler::SetAnimationDataContainer(GraphicsAnimationComponent* graphAnimationComponent, int index)
{
	Resources::Model* model;
	Resources::ResourceHandler::GetInstance()->GetModel(UINT(graphAnimationComponent->modelID), model);

	if (model == nullptr)
		return;

	Resources::Skeleton* skeleton = model->GetSkeleton();

	if (skeleton == nullptr)
		return;

	Resources::Skeleton::Joint* jointList = skeleton->GetSkeletonData()->joints;

	m_AnimationDataContainer[index].skeleton.resize(skeleton->GetSkeletonData()->jointCount);

	for (int jointIndex = 0; jointIndex < skeleton->GetSkeletonData()->jointCount; jointIndex++)
	{
		DirectX::XMMATRIX matrix = DirectX::XMMATRIX(jointList[jointIndex].invBindPose);

		Skeleton skel;
		skel.jointIndex = jointList[jointIndex].jointIndex;
		skel.parentIndex = jointList[jointIndex].parentIndex;
		skel.inverseBindPose = matrix;

		m_AnimationDataContainer[index].skeleton[jointIndex] = skel;
	}

	m_AnimationDataContainer[index].animations.resize(skeleton->GetNumAnimations());

	for (int animIndex = 0; animIndex < skeleton->GetNumAnimations(); animIndex++)
	{
		const Resources::Animation* animation = skeleton->GetAnimation(animIndex);

		if (animation == nullptr)
			return;

		else
			m_AnimationDataContainer[index].animations[animIndex] = animation->GetAllJoints();
	}
}

AnimationDataContainer AnimationHandler::GetAnimationDataFromIndex(int index)
{
	return m_AnimationDataContainer[index];
}

void AnimationHandler::SetAnimationData(AnimationDataContainer animationData)
{
	this->m_AnimationData = animationData;
}

int AnimationHandler::SetComponentArraySize(int newSize)
{
	if (this->m_nrOfAnimationComponents < newSize)
	{
		this->IncreaseArraySize(newSize);
	}

	else if (this->m_maxAnimationComponents > newSize)
	{
		this->DecreaseArraySize(newSize);
	}

	return 0;
}

AnimationComponent * AnimationHandler::GetNextAvailableComponent()
{
	if (this->m_nrOfAnimationComponents < this->m_maxAnimationComponents)
	{
		this->m_nrOfAnimationComponents++;
		return this->m_AnimationComponents[this->m_nrOfAnimationComponents - 1];
	}

	else
	{
		this->IncreaseArraySize();
		return this->GetNextAvailableComponent();
	}

	return nullptr;
}

void AnimationHandler::Shutdown()
{
	for (int i = 0; i < m_nrOfAnimationComponents; i++)
	{
		if (this->m_AnimationComponents[i] != nullptr)
		{
			delete this->m_AnimationComponents[i];
			this->m_AnimationComponents[i] = nullptr;
		}
	}

	delete[]this->m_AnimationComponents;
}

void AnimationHandler::InterpolateKeys(AnimationComponent* animationComponent, float currentTime)
{
	std::vector<DirectX::XMFLOAT4X4> localTransforms;

	int jointSize = m_AnimationDataContainer.size();

	localTransforms.resize(jointSize);

	int animationState = animationComponent->animationState;

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimationData.animations[animationState];

	for (unsigned int jointIndex = 0; jointIndex < jointSize; jointIndex++)
	{
		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animationComponent->startFrame)
		{
			int startFrame = (int)animationComponent->startFrame;

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
		else if (currentTime >= animationComponent->endFrame)
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

void AnimationHandler::ExtractBlendingKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, AnimationComponent* animationComponent, float currentTime, int animIndex)
{
	int animationState = animationComponent->animationState;

	const Resources::Animation::AnimationJoint* animatedJoints = m_AnimationData.animations[animationState];

	for (unsigned int jointIndex = 0; jointIndex < m_AnimationData.skeleton.size(); jointIndex++)
	{
		BlendKeyframe blendKey;

		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animationComponent->startFrame)
		{
			int startFrame = (int)animationComponent->startFrame;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[startFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[startFrame].scale);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[startFrame].quaternion);

			blendKey.trans = DirectX::XMLoadFloat3(&tempTrans);
			blendKey.scale = DirectX::XMLoadFloat3(&tempScale);
			blendKey.quat = DirectX::XMLoadFloat4(&tempQuat);

			blendKeysPerAnimation[animIndex].push_back(blendKey);
		}

		/*The current time is the last keyframe.*/
		else if (currentTime >= animationComponent->endFrame)
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

	int jointSize = m_AnimationData.skeleton.size();

	localTransforms.resize(jointSize);

	for (int jointIndex = 0; jointIndex < jointSize; jointIndex++)
	{
		DirectX::XMVECTOR transAnim1 = blendKeysPerAnimation[0][jointIndex].trans;
		DirectX::XMVECTOR transAnim2 = blendKeysPerAnimation[1][jointIndex].trans;

		DirectX::XMVECTOR scaleAnim1 = blendKeysPerAnimation[0][jointIndex].scale;
		DirectX::XMVECTOR scaleAnim2 = blendKeysPerAnimation[1][jointIndex].scale;

		DirectX::XMVECTOR quatAnim1 = blendKeysPerAnimation[0][jointIndex].quat;
		DirectX::XMVECTOR quatAnim2 = blendKeysPerAnimation[1][jointIndex].quat;

		/*To make the blending work, it only seems that the blend factor of the new animation is required to make this work.*/
		//float weightA = 1.0f - (transitionTime / m_TransitionDuration);
		//float weightB = transitionTime / m_TransitionDuration;
		float blendFactor = transitionTime / m_TransitionDuration;

		std::cout << "Transition Blend Factor: " << blendFactor << std::endl;

		DirectX::XMVECTOR lerpBlendTrans = DirectX::XMVectorLerp(transAnim1, transAnim2, blendFactor);
		DirectX::XMVECTOR lerpBlendScale = DirectX::XMVectorLerp(scaleAnim1, scaleAnim2, blendFactor);
		DirectX::XMVECTOR lerpBlendQuat = DirectX::XMQuaternionSlerp(quatAnim1, quatAnim2, blendFactor
		);

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

		for (int animClipIndex = 0; animClipIndex < m_AnimationComponentStack.size(); animClipIndex++)
		{
			if (m_AnimationComponentStack[animClipIndex]->localTime <= GetStartFrame(animClipIndex))
			{
				m_AnimationComponentStack[animClipIndex]->localTime = GetStartFrame(animClipIndex);
			}

			else if (m_AnimationComponentStack[animClipIndex]->localTime >= GetEndFrame(animClipIndex))
			{
				m_AnimationComponentStack[animClipIndex]->localTime = GetEndFrame(animClipIndex);
			}

			else if (m_AnimationComponentStack[animClipIndex]->localTime > GetStartFrame(animClipIndex) && m_AnimationComponentStack[animClipIndex]->localTime < GetEndFrame(animClipIndex))
			{
				m_AnimationComponentStack[animClipIndex]->localTime += secondsElapsed;
			}

			ExtractBlendingKeys(blendKeysPerAnimation, m_AnimationComponentStack[animClipIndex], m_AnimationComponentStack[animClipIndex]->localTime, animClipIndex);
		}

		/*Calculate the blend time with the delta-time.*/

		BlendKeys(blendKeysPerAnimation, m_TransitionTimeLeft);
	}
}

void AnimationHandler::CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localTransforms)
{
	std::vector<DirectX::XMFLOAT4X4> toRootTransform(m_AnimationData.skeleton.size());

	DirectX::XMMATRIX childLocal = DirectX::XMLoadFloat4x4(&localTransforms[0]);

	DirectX::XMStoreFloat4x4(&toRootTransform[0], childLocal);

	for (int i = 1; i < m_AnimationData.skeleton.size(); i++)
	{
		int parentIndex = m_AnimationData.skeleton[i].parentIndex;

		DirectX::XMMATRIX toParent = DirectX::XMLoadFloat4x4(&localTransforms[i]);

		DirectX::XMMATRIX parentToRoot = DirectX::XMLoadFloat4x4(&toRootTransform[parentIndex]);

		DirectX::XMMATRIX toRoot = DirectX::XMMatrixMultiply(toParent, parentToRoot);

		DirectX::XMStoreFloat4x4(&toRootTransform[i], toRoot);
	}

	for (int i = 0; i <  m_AnimationData.skeleton.size(); i++)
	{
		DirectX::XMMATRIX inverseBindPose = m_AnimationData.skeleton[i].inverseBindPose;
		DirectX::XMMATRIX toRoot = DirectX::XMLoadFloat4x4(&toRootTransform[i]);

		//m_graphicsAnimationComponent->finalJointTransforms[i] = DirectX::XMMatrixMultiply(inverseBindPose, toRoot);
	}
}

void AnimationHandler::Push(AnimationComponent* animationComponent)
{
	m_AnimationComponentStack.push_back(animationComponent);
}

void AnimationHandler::Pop()
{
	/*Function swaps the two elements in the vector and removes the last element in vector.*/
	if (!m_AnimationComponentStack.empty())
	{
		std::swap(m_AnimationComponentStack.front(), m_AnimationComponentStack.back());
		m_AnimationComponentStack.pop_back();
		m_AnimationComponentStack.shrink_to_fit();
	}
}

float AnimationHandler::GetStartFrame(int animationState)
{
	float startFrame = m_AnimationData.animations[animationState]->keyframes->timeValue;
	return startFrame;
}

float AnimationHandler::GetEndFrame(int animationState)
{
	int keyframeCount = m_AnimationData.animations[animationState]->keyframeCount;
	float endFrame = m_AnimationData.animations[animationState]->keyframes[keyframeCount - 1].timeValue;
	return endFrame;
}