#include "Animation.h"

Animation::Animation()
{
	int animIndex = 0;
	this->elapsedTime = 0.0f;
	this->currentAnimation = IDLE_STATE;
	this->m_graphicsAnimationComponent = new GraphicsAnimationComponent;
	this->m_graphicsAnimationComponent->joints = 19;

	this->m_graphicsAnimationComponent->worldMatrix = DirectX::XMMatrixRotationY(270);
	//this->m_graphicsAnimationComponent->worldMatrix = DirectX::XMMatrixIdentity();
	for (int i = 0; i < 32; i++)
	{
		m_graphicsAnimationComponent->finalTransforms[i] = DirectX::XMMatrixIdentity();
	}

	Resources::ResourceHandler::GetInstance()->GetModel(UINT(1337), modelPtr);

	skeletonPtr = modelPtr->GetSkeleton();

	jointList = skeletonPtr->GetSkeletonData()->joints;
	jointCount = skeletonPtr->GetSkeletonData()->jointCount;

	interpolatedTransforms.resize(jointCount);

	for (int i = 0; i < jointCount; i++)
	{
		ConvertFloatArrayToXMFloatMatrix(jointList[i].invBindPose, i);
	}
	animationPtr = skeletonPtr->GetAnimation(currentAnimation);

	animatedJointsList = animationPtr->GetAllJoints();

	float startFrame = animatedJointsList->keyframes[0].timeValue;
	float endFrame = animatedJointsList->keyframes[animatedJointsList->keyframeCount - 1].timeValue;

	/*Initialize the stack with a default "IDLE" animation.*/
	Push(currentAnimation, true, startFrame, endFrame, 0);
}

Animation::~Animation()
{
	delete m_graphicsAnimationComponent;
	//delete modelPtr;
}

void Animation::Update(float dt)
{
	/*Need to check in this function what the time in the animation is.
	If the last frame have been reached, we should check if the time is
	at the end of the animation and if it's a looping animation.*/

	bool newAnimation = false;
	
	/*Check if there is a current animation in the stack.*/
	if (!animationStack.empty() || currentAnimation == IDLE_STATE)
	{
		/*Increment elapsedTime with the delta-time. Consider to multiply framerate here?*/
		float toSeconds = dt / 1000000;
		elapsedTime += toSeconds;

		/*If the animation reaches the last frame and the animation is looping, reset elapsedTime to ZERO.*/
		if (elapsedTime >= animationStack.top().endFrame && animationStack.top().isLooping == true)
		{
			elapsedTime = 0.0f;
		}

		/*If the animation reaches the last frame and the animation is NOT looping, */
		else if (elapsedTime >= animationStack.top().endFrame && animationStack.top().isLooping == false)
		{
			/*Send the elapsed time from previous animation clip, it's index and the index to the next animation.*/
			/*Blend between animations.*/
			//Blend(elapsedTime, currentAnimation, 0); //<---- Somehow get the new animation state as input argument here. 

			elapsedTime = 0.0f;

			/*Remove the previous played animation clip from stack.*/
			Pop();

			/*The animation is new and not the default state.*/
			if (newAnimation != IDLE_STATE)
			{
				//GetAnimationData(currentAnimation, isLooping, startFrame, endFrame, duration);

				/*Push the new animation to the stack.*/
				Push(0, false, 0, 0, 0);
			}

			/*The animation is the default state.*/
			else
			{
				Push(IDLE_STATE, true, 0, 0, 0);
			}
		}

		//else if (elapsedTime > animationStack.top().startFrame || elapsedTime < animationStack.top().endFrame && newAnimation == true)
		//{
		//	Push(0, false, 0, 0, 0);

		//	//Blend(elapsedTime, currentAnimation, 0);
		//}

		/*Call this function and check if the elapsedTime is at the start, between frames or at the end.*/
		Interpolate(elapsedTime);
	}

	/*No current animation, the physics are currently controlling the "animatible entity".*/
	if(currentAnimation == PHYSICS_STATE)
	{
		/*Have some kind of function here that pauses the playing of any animation, while physics do the job.*/
	}
}

void Animation::Interpolate(float currentTime)
{
	for (int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		Resources::Animation::AnimationJoint animatedJoint = animatedJointsList[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animationStack.top().startFrame)
		{
			int startFrame = animationStack.top().startFrame;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[startFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[startFrame].scale);
			DirectX::XMFLOAT3 tempRot(animatedJoint.keyframes[startFrame].rotation);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[startFrame].quaternion);

			//tempTrans.x *= -1.0;
			//tempTrans.y *= -1.0;
			tempTrans.z *= -1.0;

			//tempQuat.x *= -1.0;
			//tempQuat.y *= -1.0;
			//tempQuat.z *= -1.0;
			//tempQuat.w *= -1.0;

			//tempScale.x *= -1.0;
			//tempScale.y *= -1.0;
			//tempScale.z *= -1.0;

			DirectX::XMVECTOR trans = XMLoadFloat3(&tempTrans);
			DirectX::XMVECTOR scale = XMLoadFloat3(&tempScale);
			DirectX::XMVECTOR rot = XMLoadFloat3(&tempRot);
			DirectX::XMVECTOR quat = XMLoadFloat4(&tempQuat);

			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
			DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
			DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(quat);
			DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(trans);

			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, rotMat), scaleMat);
			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(scaleMat, quatMat), transMat);
			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

			DirectX::XMStoreFloat4x4(&interpolatedTransforms[jointIndex], localTransform);
		}
		/*The current time is the last keyframe.*/
		else if (currentTime >= animationStack.top().endFrame)
		{
			int endFrame = animationStack.top().endFrame;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[endFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[endFrame].scale);
			DirectX::XMFLOAT3 tempRot(animatedJoint.keyframes[endFrame].rotation);
			DirectX::XMFLOAT4 tempQuat(animatedJoint.keyframes[endFrame].quaternion);

			//tempTrans.x *= -1.0;
			//tempTrans.y *= -1.0;
			tempTrans.z *= -1.0;

			//tempQuat.x *= -1.0;
			//tempQuat.y *= -1.0;
			//tempQuat.z *= -1.0;
			//tempQuat.w *= -1.0;
			
			//tempScale.x *= -1.0;
			//tempScale.y *= -1.0;
			//tempScale.z *= -1.0;

			DirectX::XMVECTOR trans = XMLoadFloat3(&tempTrans);
			DirectX::XMVECTOR scale = XMLoadFloat3(&tempScale);
			DirectX::XMVECTOR rot = XMLoadFloat3(&tempRot);
			DirectX::XMVECTOR quat = XMLoadFloat4(&tempQuat);

			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
			DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
			DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(quat);
			DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(trans);

			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(scaleMat, quatMat), transMat);
			DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, rotMat), scaleMat);
			//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

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

					//tempTrans1.x *= -1.0;
					//tempTrans1.y *= -1.0;
					tempTrans1.z *= -1.0;

					//tempTrans2.x *= -1.0;
					//tempTrans2.y *= -1.0;
					tempTrans2.z *= -1.0;

					//tempQuat1.x *= -1.0;
					//tempQuat1.y *= -1.0;
					//tempQuat1.z *= -1.0;
					//tempQuat1.w *= -1.0;

					//tempQuat2.x *= -1.0;
					//tempQuat2.y *= -1.0;
					//tempQuat2.z *= -1.0;
					//tempQuat2.w *= -1.0;

					//tempScale1.x *= -1.0;
					//tempScale1.y *= -1.0;
					//tempScale1.z *= -1.0;

					//tempScale2.x *= -1.0;
					//tempScale2.y *= -1.0;
					//tempScale2.z *= -1.0;

					DirectX::XMVECTOR trans1 = DirectX::XMLoadFloat3(&tempTrans1);
					DirectX::XMVECTOR scale1 = DirectX::XMLoadFloat3(&tempScale1);
					DirectX::XMVECTOR rot1 = DirectX::XMLoadFloat3(&tempRot1);
					DirectX::XMVECTOR quat1 = XMLoadFloat4(&tempQuat1);
					DirectX::XMVECTOR quat2 = XMLoadFloat4(&tempQuat2);

					DirectX::XMVECTOR trans2 = DirectX::XMLoadFloat3(&tempTrans2);
					DirectX::XMVECTOR scale2 = DirectX::XMLoadFloat3(&tempScale2);
					DirectX::XMVECTOR rot2 = DirectX::XMLoadFloat3(&tempRot2);

					DirectX::XMVECTOR lerpTrans = DirectX::XMVectorLerp(trans1, trans2, lerpFactor);
					DirectX::XMVECTOR lerpScale = DirectX::XMVectorLerp(scale1, scale2, lerpFactor);
					DirectX::XMVECTOR lerpRot = DirectX::XMVectorLerp(rot1, rot2, lerpFactor);
					DirectX::XMVECTOR lerpQuat = DirectX::XMQuaternionSlerp(quat1, quat2, lerpFactor);

					DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(lerpScale);
					DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(lerpRot);
					DirectX::XMMATRIX quatMat = DirectX::XMMatrixRotationQuaternion(lerpQuat);
					DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(lerpTrans);

					DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, quatMat), scaleMat);
					//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(transMat, rotMat), scaleMat);
					//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(scaleMat, rotMat), transMat);
					//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(scaleMat, quatMat), transMat);
					//DirectX::XMMATRIX localTransform = DirectX::XMMatrixMultiply(transMat, quatMat);

					/*Update the transform for each joint in the skeleton.*/
					DirectX::XMStoreFloat4x4(&interpolatedTransforms[jointIndex], localTransform);
				}
			}
		}
	}

	/*Calculate the final matrices for each joint in the skeleton hierarchy.*/
	CalculateFinalTransform(interpolatedTransforms);
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

void Animation::Blend(int lastFrame, int prevState, int newState)
{
}

void Animation::Push(int currentAnimation, bool isLooping, float startFrame, float endFrame, float duration)
{
	this->currentAnimation = currentAnimation;

	AnimationClip clip;
	clip.isLooping = isLooping;
	clip.startFrame = startFrame;
	clip.endFrame = endFrame;

	animationStack.push(clip);
}

void Animation::Pop()
{
	if (!animationStack.empty())
	{
		animationStack.pop();
	}
}

