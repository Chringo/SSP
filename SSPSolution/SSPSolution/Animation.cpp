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

	Resources::Status st = Resources::ResourceHandler::GetInstance()->GetModel(UINT(1337), modelPtr);

	skeletonPtr = modelPtr->GetSkeleton();

	jointList = skeletonPtr->GetSkeletonData()->joints;
	jointCount = skeletonPtr->GetSkeletonData()->jointCount;

	for (unsigned int i = 0; i < jointCount; i++)
	{
		ConvertFloatArrayToXMFloatMatrix(jointList[i].invBindPose, i);
	}
	animationPtr = skeletonPtr->GetAnimation(currentAnimation);

	animatedJointsList = animationPtr->GetAllJoints();

	float startFrame = animatedJointsList->keyframes[0].timeValue;
	float endFrame = animatedJointsList->keyframes[animatedJointsList->keyframeCount - 1].timeValue;

	/*Initialize the stack with a default "IDLE" animation.*/
	Push(currentAnimation, true, (int)startFrame, (int)endFrame, 0);
}

Animation::~Animation()
{
	delete m_graphicsAnimationComponent;

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
		elapsedTime += (toSeconds / 60);

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
		Interpolate(elapsedTime, interpolatedTransforms);
	}

	/*No current animation, the physics are currently controlling the "animatible entity".*/
	if(currentAnimation == PHYSICS_STATE)
	{
		/*Have some kind of function here that pauses the playing of any animation, while physics do the job.*/
	}
}

void Animation::Interpolate(float currentTime, std::vector<XMFLOAT4X4> interpolatedTransforms)
{
	interpolatedTransforms.resize(jointCount);

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		Resources::Animation::AnimationJoint animatedJoint = animatedJointsList[jointIndex];

		/*The current time is the first keyframe.*/
		if (currentTime <= animationStack.top().startFrame)
		{
			int startFrame = (int)animationStack.top().startFrame;

			XMFLOAT3 tempTrans(animatedJoint.keyframes[startFrame].translation);
			XMFLOAT3 tempScale(animatedJoint.keyframes[startFrame].scale);
			XMFLOAT4 tempQuat(animatedJoint.keyframes[startFrame].quaternion);

			XMVECTOR trans = XMLoadFloat3(&tempTrans);
			XMVECTOR scale = XMLoadFloat3(&tempScale);
			XMVECTOR quat = XMLoadFloat4(&tempQuat);

			XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 0.f);

			XMStoreFloat4x4(&interpolatedTransforms[jointIndex], XMMATRIX(scale, zero, quat, trans));
		}
		/*The current time is the last keyframe.*/
		else if (currentTime >= animationStack.top().endFrame)
		{
			int endFrame = (int)animationStack.top().endFrame;

			XMFLOAT3 tempTrans(animatedJoint.keyframes[endFrame].translation);
			XMFLOAT3 tempScale(animatedJoint.keyframes[endFrame].scale);
			XMFLOAT4 tempQuat(animatedJoint.keyframes[endFrame].quaternion);

			XMVECTOR trans = XMLoadFloat3(&tempTrans);
			XMVECTOR scale = XMLoadFloat3(&tempScale);
			XMVECTOR quat = XMLoadFloat4(&tempQuat);

			XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 0.f);

			XMStoreFloat4x4(&interpolatedTransforms[jointIndex], XMMATRIX(scale, zero, quat, trans));
		}
		/*The current time is between two keyframes.*/
		else
		{
			int keyFrameCount = animatedJoint.keyframeCount;

			for (int i = 0; i < keyFrameCount; i++)
			{
				/*Check if the current time is between two keyframes for each joint.*/
				if (currentTime >= animatedJoint.keyframes[i].timeValue && currentTime <= animatedJoint.keyframes[i + 1].timeValue)
				{
					float timeKeyframe1 = animatedJoint.keyframes[i].timeValue;
					float timeKeyframe2 = animatedJoint.keyframes[i + 1].timeValue;

					/*Lerp factor is calculated for a normalized value between 0-1 for interpolation.*/
					float lerpFactor = (currentTime - timeKeyframe1) / (timeKeyframe2 - timeKeyframe1);

					XMFLOAT3 tempTrans1(animatedJoint.keyframes[i].translation);
					XMFLOAT3 tempTrans2(animatedJoint.keyframes[i + 1].translation);
					XMFLOAT3 tempScale1(animatedJoint.keyframes[i].scale);
					XMFLOAT3 tempScale2(animatedJoint.keyframes[i + 1].scale);
					XMFLOAT4 tempQuat1(animatedJoint.keyframes[i].quaternion);
					XMFLOAT4 tempQuat2(animatedJoint.keyframes[i + 1].quaternion);

					XMVECTOR trans1 = XMLoadFloat3(&tempTrans1);
					XMVECTOR scale1 = XMLoadFloat3(&tempScale1);
					XMVECTOR quat1 = XMLoadFloat4(&tempQuat1);

					XMVECTOR trans2 = XMLoadFloat3(&tempTrans2);
					XMVECTOR scale2 = XMLoadFloat3(&tempScale2);
					XMVECTOR quat2 = XMLoadFloat4(&tempQuat2);

					XMVECTOR lerpTrans = XMVectorLerp(trans1, trans2, lerpFactor);
					XMVECTOR lerpScale = XMVectorLerp(scale1, scale2, lerpFactor);
					XMVECTOR lerpQuat = XMQuaternionSlerp(quat1, quat2, lerpFactor);

					/*Zero vector for the affine transformation matrix.*/
					XMVECTOR zero = XMVectorSet(0, 0, 0, 0);

					/*Update the transform for each joint in the skeleton.*/
					XMStoreFloat4x4(&interpolatedTransforms[jointIndex],
						XMMATRIX(lerpScale, zero, lerpQuat, lerpTrans));
				}
			}
		}
	}

	/*Calculate the final matrices for each joint in the skeleton hierarchy.*/
	CalculateFinalTransform(interpolatedTransforms);
}

void Animation::ConvertFloatArrayToXMFloatMatrix(float floatArray[16], int jointIndex)
{
	XMFLOAT4X4 matrix = XMFLOAT4X4(floatArray);

	SkelTemp temp;
	temp.jointIndex = jointList[jointIndex].jointIndex;
	temp.parentIndex = jointList[jointIndex].parentIndex;
	temp.invBindPose = matrix;


	skeltempVec.push_back(temp);
}

void Animation::CalculateFinalTransform(std::vector<XMFLOAT4X4> childTransform)
{

	for (unsigned int jointIndex = 0; jointIndex < jointCount; jointIndex++)
	{
		XMMATRIX childTransformation = XMLoadFloat4x4(&childTransform[jointIndex]);
		XMMATRIX invBindPose = XMLoadFloat4x4(&skeltempVec[jointIndex].invBindPose);

		int parentIndex = skeltempVec[jointIndex].parentIndex;

		XMMATRIX tempWorldMatrix = XMMatrixIdentity();
		XMMATRIX worldMatrix;

		if (parentIndex == -1) {
			worldMatrix = childTransformation;
		}

		else {
			worldMatrix = XMMatrixMultiply(tempWorldMatrix, childTransformation);
		}

		XMMATRIX finalTransform;

		finalTransform = XMMatrixMultiply(worldMatrix, invBindPose);

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
	clip.startFrame = (float)startFrame;
	clip.endFrame = (float)endFrame;

	animationStack.push(clip);
}

void Animation::Pop()
{
	if (!animationStack.empty())
	{
		animationStack.pop();
	}
}

