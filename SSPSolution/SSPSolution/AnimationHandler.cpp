#include "AnimationHandler.h"

AnimationHandler::AnimationHandler()
{
	/*No good practice but will work for now. Need to get the model ID from GraphicsAnimationComponent or AnimationComponent.*/
	Resources::Model* model = GetAnimatedModel(1337);

	/*Skeleton that is contained in the specified model.*/
	Resources::Skeleton* skeleton = GetSkeleton(model);

	/*Gets all animations and saves them in a container to use for playing and blending animations.*/
	GetAnimations(skeleton);

	this->m_elapsedTime = 0.0f;
	this->m_graphicsAnimationComponent = new GraphicsAnimationComponent;
	this->m_graphicsAnimationComponent->jointCount = 19;

	//this->m_graphicsAnimationComponent->worldMatrix = DirectX::XMMatrixIdentity();
	this->m_graphicsAnimationComponent->worldMatrix = DirectX::XMMatrixTranslation(0, 4, 10);
	for (int i = 0; i < 32; i++)
	{
		m_graphicsAnimationComponent->finalJointTransforms[i] = DirectX::XMMatrixIdentity();
	}

	m_isComplete = false;
	m_newAnimation = false;

	m_BlendTimeLeft = 0;
	m_BlendDuration = 1;
	
	m_elapsedTime = 0;
	
	/*Initialize the stack with a default "IDLE" animation.*/
	Push(0, m_newAnimation, true, 1);
}

AnimationHandler::~AnimationHandler()
{
	delete m_graphicsAnimationComponent;
}

void AnimationHandler::Update(float dt)
{
	/*Check if there is a current animation in the stack.*/
	if (!m_animationStack.empty())
	{
		/*Increment elapsedTime with the delta-time. Consider to multiply framerate here?*/
		float toSeconds = dt / 1000000;
		m_elapsedTime += toSeconds;

		/*If the animation reaches the last frame and the animation is looping, reset elapsedTime to ZERO.*/
		if (m_elapsedTime >= m_animationStack.front().endFrame && m_animationStack.front().isLooping == true)
		{
			m_elapsedTime = 0.0f;
		}

		if (m_newAnimation == true)
		{
			m_elapsedTime = 0.0f;
			//m_newAnimation = false;
			Blend(m_elapsedTime, m_newAnimation);
		}

		Interpolate(m_elapsedTime);
	}

	/*There is no current animation playing.*/
	else
	{
		return;
	}
}

void AnimationHandler::Interpolate(float currentTime)
{
	int jointCount = m_skeletonContainer.size();

	m_localTransforms.resize(jointCount);

	int currentAnimation = m_animationStack.front().animationState;

	const Resources::Animation::AnimationJoint* animatedJoints = m_animationContainer[currentAnimation];

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		const Resources::Animation::AnimationJoint animatedJoint = animatedJoints[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= m_animationStack.front().startFrame)
		{
			int startFrame = (int)m_animationStack.front().startFrame;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[startFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[startFrame].scale);
			DirectX::XMFLOAT3 tempRot(animatedJoint.keyframes[startFrame].rotation);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[startFrame].quaternion);

			DirectX::XMVECTOR trans = XMLoadFloat3(&tempTrans);
			DirectX::XMVECTOR scale = XMLoadFloat3(&tempScale);
			DirectX::XMVECTOR rot = XMLoadFloat3(&tempRot);
			DirectX::XMVECTOR quat = XMLoadFloat4(&tempQuat);

			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
			DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
			DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(quat);
			DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(trans);

			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

			DirectX::XMStoreFloat4x4(&m_localTransforms[jointIndex], localTransform);
		}
		/*The current time is the last keyframe.*/
		else if (currentTime >= m_animationStack.front().endFrame)
		{
			int endFrame = (int)m_animationStack.front().endFrame;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[endFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[endFrame].scale);
			DirectX::XMFLOAT3 tempRot(animatedJoint.keyframes[endFrame].rotation);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[endFrame].quaternion);

			DirectX::XMVECTOR trans = XMLoadFloat3(&tempTrans);
			DirectX::XMVECTOR scale = XMLoadFloat3(&tempScale);
			DirectX::XMVECTOR rot = XMLoadFloat3(&tempRot);
			DirectX::XMVECTOR quat = XMLoadFloat4(&tempQuat);

			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
			DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
			DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(quat);
			DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(trans);

			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

			DirectX::XMStoreFloat4x4(&m_localTransforms[jointIndex], localTransform);
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
					DirectX::XMFLOAT3 tempRot1(animatedJoint.keyframes[i].rotation);
					DirectX::XMFLOAT3 tempRot2(animatedJoint.keyframes[i + 1].rotation);
					DirectX::XMFLOAT4 tempQuat1(animatedJoint.keyframes[i].quaternion);
					DirectX::XMFLOAT4 tempQuat2(animatedJoint.keyframes[i + 1].quaternion);

					DirectX::XMVECTOR trans1 = DirectX::XMLoadFloat3(&tempTrans1);
					DirectX::XMVECTOR scale1 = DirectX::XMLoadFloat3(&tempScale1);
					DirectX::XMVECTOR rot1 = DirectX::XMLoadFloat3(&tempRot1);
					DirectX::XMVECTOR quat1 = XMLoadFloat4(&tempQuat1);

					DirectX::XMVECTOR trans2 = DirectX::XMLoadFloat3(&tempTrans2);
					DirectX::XMVECTOR scale2 = DirectX::XMLoadFloat3(&tempScale2);
					DirectX::XMVECTOR rot2 = DirectX::XMLoadFloat3(&tempRot2);
					DirectX::XMVECTOR quat2 = XMLoadFloat4(&tempQuat2);

					DirectX::XMVECTOR lerpTrans = DirectX::XMVectorLerp(trans1, trans2, lerpFactor);
					DirectX::XMVECTOR lerpScale = DirectX::XMVectorLerp(scale1, scale2, lerpFactor);
					DirectX::XMVECTOR lerpRot = DirectX::XMVectorLerp(rot1, rot2, lerpFactor);
					DirectX::XMVECTOR lerpQuat = DirectX::XMQuaternionSlerp(quat1, quat2, lerpFactor);

					DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(lerpScale);
					DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(lerpRot);
					DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(lerpQuat);
					DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(lerpTrans);

					DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);

					/*Update the transform for each joint in the skeleton.*/
					DirectX::XMStoreFloat4x4(&m_localTransforms[jointIndex], localTransform);
				}
			}
		}
	}

	/*Calculate the final matrices for each joint in the skeleton hierarchy.*/
	CalculateFinalTransform(m_localTransforms);
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

void AnimationHandler::Push(int animationState, bool newAnimation, bool isLooping, float weight)
{
	AnimationClip clip;

	clip.animationState = animationState;

	/*First time pushing to the stack, the stack should be empty, thus previous state is none.*/
	if (m_animationStack.empty())
		clip.previousState = -1;

	else
	{
		clip.previousState = m_animationStack.front().animationState;
		
		Pop(); // Comment out when writing blend code. 
	}

	if (clip.previousState == -1 || clip.previousState == clip.animationState)
		m_newAnimation = false;

	else
		m_newAnimation = true;
		
	clip.startFrame = GetStartFrame(animationState);
	clip.endFrame = GetEndFrame(animationState);
	clip.isLooping = isLooping;
	clip.weight = weight;

	m_animationStack.push_back(clip);
}

void AnimationHandler::Pop()
{
	if (!m_animationStack.empty())
	{
		m_animationStack.pop_back();
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

void AnimationHandler::Blend(float currentTime, bool newAnimation)
{
	/*If the time left to blend from one animation to the other, we keep doing this until reaching value of m_blendDuration.*/
	if (m_BlendTimeLeft > 0)
	{
		
	}
}

