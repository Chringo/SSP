#include "Animation.h"

Animation::Animation()
{
	int animIndex = 0;
	this->elapsedTime = 0.0f;
	this->currentAnimation = IDLE_STATE;
	this->m_graphicsAnimationComponent = new GraphicsAnimationComponent;
	this->m_graphicsAnimationComponent->joints = 19;
	this->m_graphicsAnimationComponent->worldMatrix = DirectX::XMMatrixIdentity();
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
			DirectX::XMFLOAT4 tempRot(animatedJoint.keyframes[startFrame].rotation);



			DirectX::XMVECTOR trans = XMLoadFloat3(&tempTrans);
			DirectX::XMVECTOR scale = XMLoadFloat3(&tempScale);
			DirectX::XMVECTOR rot = XMLoadFloat4(&tempRot);
			
			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
			DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
			DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(trans);

			DirectX::XMMATRIX localTransform = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(scaleMat, rotMat), transMat));

			DirectX::XMStoreFloat4x4(&interpolatedTransforms[jointIndex], localTransform);
		}
		/*The current time is the last keyframe.*/
		else if (currentTime >= animationStack.top().endFrame)
		{
			int endFrame = animationStack.top().endFrame;

			DirectX::XMFLOAT3 tempTrans(animatedJoint.keyframes[endFrame].translation);
			DirectX::XMFLOAT3 tempScale(animatedJoint.keyframes[endFrame].scale);
			DirectX::XMFLOAT3 tempRot(animatedJoint.keyframes[endFrame].rotation);

			DirectX::XMVECTOR trans = XMLoadFloat3(&tempTrans);
			DirectX::XMVECTOR scale = XMLoadFloat3(&tempScale);
			DirectX::XMVECTOR rot = XMLoadFloat3(&tempRot);

			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
			DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
			DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(trans);

			DirectX::XMMATRIX localTransform = (scaleMat * rotMat) * transMat;

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

					DirectX::XMVECTOR trans1 = DirectX::XMLoadFloat3(&tempTrans1);
					DirectX::XMVECTOR scale1 = DirectX::XMLoadFloat3(&tempScale1);
					DirectX::XMVECTOR rot1 = DirectX::XMLoadFloat3(&tempRot1);

					DirectX::XMVECTOR trans2 = DirectX::XMLoadFloat3(&tempTrans2);
					DirectX::XMVECTOR scale2 = DirectX::XMLoadFloat3(&tempScale2);
					DirectX::XMVECTOR rot2 = DirectX::XMLoadFloat3(&tempRot2);

					DirectX::XMVECTOR lerpTrans = DirectX::XMVectorLerp(trans1, trans2, lerpFactor);
					DirectX::XMVECTOR lerpScale = DirectX::XMVectorLerp(scale1, scale2, lerpFactor);
					DirectX::XMVECTOR lerpRot = DirectX::XMVectorLerp(rot1, rot2, lerpFactor);

					DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(lerpScale);
					DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(lerpRot);
					DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslationFromVector(lerpTrans);

					DirectX::XMMATRIX localTransform = (scaleMat * rotMat) * transMat;

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

void Animation::CalculateGlobalInverseBindPose(std::vector<DirectX::XMMATRIX>& globalInverseBindPose)
{
	for (int i = 0; i < jointCount; i++)
	{
		int parentIndex = skeltempVec[i].parentIndex;

		DirectX::XMVECTOR determinant = DirectX::XMMatrixDeterminant(skeltempVec[i].invBindPose);

		DirectX::XMMATRIX childBindPose = DirectX::XMMatrixInverse(&determinant, skeltempVec[i].invBindPose);

		if (parentIndex == -1)
		{
			globalInverseBindPose[i] = DirectX::XMMatrixInverse(nullptr, childBindPose);
		}

		else
		{
			//DirectX::XMMATRIX parentInvBindPose = skeltempVec[parentIndex].invBindPose; //HÄR HAR DAVID JIDDRAT!<---------------------------------------
			DirectX::XMMATRIX parentInvBindPose = globalInverseBindPose[parentIndex];

			DirectX::XMVECTOR determinant = DirectX::XMMatrixDeterminant(parentInvBindPose);
			DirectX::XMMATRIX parentBindPose = DirectX::XMMatrixInverse(&determinant, parentInvBindPose);

			globalInverseBindPose[i] = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixMultiply(childBindPose, parentBindPose));
		}
	}
}

void Animation::CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localMatrices)
{
	std::vector<DirectX::XMMATRIX> globalInverseBindPose(jointCount);

	/*Call this func() just for temporary usage, this MUST be done in the EXPORTER for optimization.*/
	CalculateGlobalInverseBindPose(globalInverseBindPose);

	//m_graphicsAnimationComponent->finalTransforms[0] = DirectX::XMLoadFloat4x4(&localMatrices[0]);
	m_graphicsAnimationComponent->finalTransforms[0] = DirectX::XMMatrixIdentity();

	/*Compute the final matrices for each joint, updating the result to the skel vertex shader.*/
	for (int jointIndex = 0; jointIndex < jointCount; jointIndex++) //HÄR HAR DAVID JIDDRAT!<-------------------------------------------
	{
		//int parentIndex = skeltempVec[jointIndex].parentIndex;

		DirectX::XMMATRIX childTransform = DirectX::XMLoadFloat4x4(&localMatrices[jointIndex]);
		//DirectX::XMMATRIX parentTransform = DirectX::XMLoadFloat4x4(&localMatrices[parentIndex]);

		//DirectX::XMMATRIX parentChildTransform = DirectX::XMMatrixMultiply(parentTransform, childTransform);

		DirectX::XMMATRIX globalInvBindPose = globalInverseBindPose[jointIndex];

		DirectX::XMMATRIX finalTransform = DirectX::XMMatrixMultiply(childTransform, globalInvBindPose);

		m_graphicsAnimationComponent->finalTransforms[jointIndex] = finalTransform;
	}
}

void Animation::Blend(int lastFrame, int prevState, int newState)
{
}

void Animation::Push(int currentAnimation, bool isLooping, int startFrame, int endFrame, int duration)
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

