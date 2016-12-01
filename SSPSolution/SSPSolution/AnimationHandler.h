#ifndef SSPAPPLICATION_CORE_ANIMATIONHANDLER_H
#define SSPAPPLICATION_CORE_ANIMATIONHANDLER_H

#include <stack>

/*This class has the responsibility to play animations in a generic way.
By generic it is meant for any "animatible entity" to be updated by
this class. The result of playing each frame will update the entity of 
the skeleton and it's hierarchy of joints.*/

struct StackHolder
{
	bool isLooping;
	int startFrame;
	int endFrame;
	int duration;
};

class AnimationHandler
{

private:

	std::stack<StackHolder> animationStack;

	//Pointer to the class that holds the jointList and keyframe data.
	/*pointer->func/data*/
	int currentAnimation;
	float t;


public: 
	AnimationHandler();
	~AnimationHandler();

	/*Update each frame in the current animation.*/
	void Update(float dt);

	/*Adds a new animation to the stack.*/
	void Push(int currentAnimation, bool isLooping, int startFrame, int endFrame, int duration);

	/*Removes the animation that is on the top of the stack.*/
	void Pop();

	/*Don't know if this is suppose to be here? Maybe this is a function
	we call from Update(), having another class holding interpolation func?*/
	void Interpolate(int currentFrame);

	/*Maybe this is more suitable for being in another class? I might consider
	calling this in the Update() from another class pointer.*/
	void Blend(int lastFrame, int prevState, int newState);
};


#endif

