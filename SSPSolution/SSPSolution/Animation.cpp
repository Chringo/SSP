#include "Animation.h"

Animation::Animation()
{
	this->elapsedTime = 0.0f;
	this->m_graphicsAnimationComponent = new GraphicsAnimationComponent;
	this->m_graphicsAnimationComponent->joints = 19;

	//this->m_graphicsAnimationComponent->worldMatrix = DirectX::XMMatrixIdentity();
	this->m_graphicsAnimationComponent->worldMatrix = DirectX::XMMatrixTranslation(0, 4, 10);
	for (int i = 0; i < 32; i++)
	{
		m_graphicsAnimationComponent->finalTransforms[i] = DirectX::XMMatrixIdentity();
	}
	
	/*Initialize the stack with a default "IDLE" animation.*/
	Push(0, false);

	for (unsigned int i = 0; i < jointCount; i++)
	{
		ConvertFloatArrayToXMFloatMatrix(jointList[i].invBindPose, i);
	}

	transitionDuration = 1.0f;
	transitionTime = 0.0f;
}

Animation::~Animation()
{
	delete m_graphicsAnimationComponent;

}

void Animation::Update(float dt)
{
	/*Check if there is a current animation in the stack.*/
	if (!animationStack.empty())
	{
		/*Increment elapsedTime with the delta-time. Consider to multiply framerate here?*/
		float toSeconds = dt / 1000000;
		elapsedTime += toSeconds;

		/*If the animation reaches the last frame and the animation is looping, reset elapsedTime to ZERO.*/
		if (elapsedTime >= animationStack.top().endFrame && animationStack.top().isLooping == true)
		{
			elapsedTime = 0.0f;
		}

		/*if (newAnimation == true)
		{
			Blend(animationStack.top().previousState, animationStack.top().animationState, elapsedTime);
		}

		else
		{
			Interpolate(elapsedTime);
		}*/

		if (newAnimation == true)
		{
			elapsedTime = 0;
			newAnimation = false;
		}

		Interpolate(elapsedTime);
	}

	/*There is no current animation playing.*/
	else
	{
		return;
	}
}

void Animation::Interpolate(float currentTime)
{
	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		Resources::Animation::AnimationJoint animatedJoint = animatedJointsList[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animationStack.top().startFrame)
		{
			int startFrame = (int)animationStack.top().startFrame;

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

			DirectX::XMStoreFloat4x4(&interpolatedTransforms[jointIndex], localTransform);
		}
		/*The current time is the last keyframe.*/
		else if (currentTime >= animationStack.top().endFrame)
		{
			int endFrame = (int)animationStack.top().endFrame;

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

			DirectX::XMStoreFloat4x4(&interpolatedTransforms[jointIndex], localTransform);
		}
		/*The current time is between two keyframes.*/
		else
		{
			int keyFrameCount = animatedJoint.keyframeCount;

			for (int i = 0; i < keyFrameCount; i++)
			{
				/*Check if the current time is between two keyframes for each joint.*/
				if (currentTime > animatedJoint.keyframes[i].timeValue && currentTime < animatedJoint.keyframes[i + 1].timeValue)
				{
					float timeKeyframe1 = animatedJoint.keyframes[i].timeValue;
					float timeKeyframe2 = animatedJoint.keyframes[i + 1].timeValue;

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
					DirectX::XMStoreFloat4x4(&interpolatedTransforms[jointIndex], localTransform);
				}
			}
		}
	}

	/*Calculate the final matrices for each joint in the skeleton hierarchy.*/
	CalculateFinalTransform(interpolatedTransforms);
}

void Animation::Blend(int oldState, int newState, float currentTime)
{
	transitionTime += currentTime;

	float duration = animationStack.top().endFrame;

	float t = (transitionTime) / (animationStack.top().endFrame - animationStack.top().startFrame);

	Interpolate(t);

	if (t > 1)
	{
		transitionTime = 0;
		std::cout << "Transition completed!" << std::endl;
	}
}

void Animation::ConvertFloatArrayToXMFloatMatrix(float floatArray[16], int jointIndex)
{
	DirectX::XMMATRIX matrix = DirectX::XMMATRIX(floatArray);

	SkelTemp temp;
	temp.jointIndex = jointList[jointIndex].jointIndex;
	temp.parentIndex = jointList[jointIndex].parentIndex;
	temp.invBindPose = matrix;

	skeltempVec.push_back(temp);
}

void Animation::CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localMatrices)
{
	DirectX::XMMATRIX childLocal = DirectX::XMLoadFloat4x4(&localMatrices[0]);

	std::vector<DirectX::XMFLOAT4X4> toRootTransform(jointCount);

	DirectX::XMStoreFloat4x4(&toRootTransform[0], childLocal);

	for (int i = 1; i < jointCount; i++)
	{
		int parentIndex = skeltempVec[i].parentIndex;

		DirectX::XMMATRIX toParent = DirectX::XMLoadFloat4x4(&localMatrices[i]);

		DirectX::XMMATRIX parentToRoot = DirectX::XMLoadFloat4x4(&toRootTransform[parentIndex]);

		DirectX::XMMATRIX toRoot = DirectX::XMMatrixMultiply(toParent, parentToRoot);

		DirectX::XMStoreFloat4x4(&toRootTransform[i], toRoot);
	}

	for (int i = 0; i < jointCount; i++)
	{
		DirectX::XMMATRIX invBindPose = skeltempVec[i].invBindPose;
		DirectX::XMMATRIX toRoot = DirectX::XMLoadFloat4x4(&toRootTransform[i]);

		m_graphicsAnimationComponent->finalTransforms[i] = DirectX::XMMatrixMultiply(invBindPose, toRoot);
	}
}

void Animation::Push(int animationState, bool newAnimation)
{
	AnimationClip clip;

	GetAnimationState(animationState, clip);

	this->newAnimation = newAnimation;

	animationStack.push(clip);
}

void Animation::Pop()
{
	if (!animationStack.empty())
	{
		animationStack.pop();
	}
}

void Animation::GetAnimationState(int animationState, AnimationClip & clip)
{
	/*Temp place for both Model and skeleton.*/

	Resources::ResourceHandler::GetInstance()->GetModel(UINT(1337), modelPtr);

	skeletonPtr = modelPtr->GetSkeleton();

	jointList = skeletonPtr->GetSkeletonData()->joints;
	jointCount = skeletonPtr->GetSkeletonData()->jointCount;

	interpolatedTransforms.resize(jointCount);

	animationPtr = skeletonPtr->GetAnimation(animationState);

	animatedJointsList = animationPtr->GetAllJoints();

	clip.animationState = animationState;

	if (animationStack.empty())
	{
		clip.previousState = -1;
	}

	else
	{
		clip.previousState = animationStack.top().animationState;
		Pop();
	}

	clip.startFrame = animatedJointsList->keyframes[0].timeValue;
	clip.endFrame = animatedJointsList->keyframes[animatedJointsList->keyframeCount - 1].timeValue;
	clip.isLooping = true;

}

