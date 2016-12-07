#include "Animation.h"

Animation::Animation()
{
	int animIndex = 0;
	this->elapsedTime = 0.0f;
	this->currentAnimation = IDLE_STATE;

	/*Initialize the stack with a default "IDLE" animation.*/
	Push(currentAnimation, true, 0, 0, 0);
}

Animation::~Animation()
{
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
		elapsedTime += dt;

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

		else if (elapsedTime > animationStack.top().startFrame || elapsedTime < animationStack.top().endFrame && newAnimation == true)
		{
			Push(0, false, 0, 0, 0);

			//Blend(elapsedTime, currentAnimation, 0);
		}

		int jointCount = 0;
		std::vector<XMFLOAT4X4> interpolatedTransforms(jointCount);

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
	int jointListSize = 0;

	for (int jointIndex = 0; jointIndex < jointListSize; jointIndex++)
	{
		XMVECTOR trans1, trans2;
		XMVECTOR scale1, scale2;
		XMVECTOR quat1, quat2;

		trans1 = XMVectorSet(0, 0, 0, 0); trans2 = XMVectorSet(0, 0, 0, 0);
		scale1 = XMVectorSet(0, 0, 0, 0); scale2 = XMVectorSet(0, 0, 0, 0);
		quat1 = XMVectorSet(0, 0, 0, 0); quat2 = XMVectorSet(0, 0, 0, 0);

		/*Interpolate translations for the two keyframes.*/
		XMVECTOR lerpedTrans = XMVectorLerp(trans1, trans2, 0);

		/*Interpolate scale for the two keyframes.*/
		XMVECTOR lerpedScale = XMVectorLerp(scale1, scale2, 0);

		/*Spherical interpolate quaternions for the two keyframes.*/
		XMVECTOR lerpedQuat = XMQuaternionSlerp(quat1, quat2, 0);

		/*Zero vector for the affine transformation matrix.*/
		XMVECTOR zero = XMVectorSet(0, 0, 0, 0);

		/*Update the transform for each joint in the skeleton.*/
		XMStoreFloat4x4(&interpolatedTransforms[jointIndex],
			XMMatrixAffineTransformation(lerpedScale, zero, lerpedQuat, lerpedTrans));
	}
}

void Animation::CalculateFinalTransform(std::vector<XMFLOAT4X4> interpolatedTransforms)
{

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

