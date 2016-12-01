#include "AnimationHandler.h"

AnimationHandler::AnimationHandler()
{
	this->t = 0.f;
	this->currentAnimation = -1;
}

AnimationHandler::~AnimationHandler()
{
}

void AnimationHandler::Update(float dt)
{
	/*Need to check in this function what the time in the animation is.
	If the last frame have been reached, we should check if the time is
	at the end of the animation and if it's a looping animation.*/

	/*Pseucode*/
	//If time is at the last frame and animation is looping.
		//Reset the time T of the animation to zero.

	//If not and still playing the animation 
		//Increment t with the time between two frames 

	//If there is a new animation, and still playing 
		//take the latest frame old animation and blend with the first frame in new.
		//Reset time T of the previous animation to zero. 

	if (!animationStack.empty())
	{
	}
}

void AnimationHandler::Interpolate(int currentFrame)
{
}

void AnimationHandler::Blend(int lastFrame, int prevState, int newState)
{
}

void AnimationHandler::Push(int currentAnimation, bool isLooping, int startFrame, int endFrame, int duration)
{
	StackHolder stack;

	stack.isLooping = isLooping;
	stack.startFrame = startFrame;
	stack.endFrame = endFrame;
	stack.duration = duration;

	animationStack.push(stack);
}

void AnimationHandler::Pop()
{
	if (!animationStack.empty())
	{
		animationStack.pop();
	}
}

