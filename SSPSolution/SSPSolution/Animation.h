#ifndef SSPAPPLICATION_CORE_ANIMATION_H
#define SSPAPPLICATION_CORE_ANIMATION_H

#include <stack>
#include <DirectXMath.h>
#include <vector>
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")
using namespace DirectX;

/*This class has the responsibility to play animations in a generic way.
By generic it is meant for any "animatible entity" to be updated by
this class. The result of playing each frame will update the entity of 
the skeleton and it's hierarchy of joints.*/

enum AnimationStates
{
	PHYSICS_STATE = -1, 
	IDLE_STATE	 =	0,
	WALK_STATE   =	1,
	RUN_STATE    =	2,
	JUMP_STATE   =	3,
	THROW_STATE  =	4
};

struct AnimationClip
{
	bool isLooping;
	int startFrame;
	int endFrame; 
};

struct SkelTemp
{
	int parentIndex;
	int jointIndex;
	XMFLOAT4X4 invBindPose;
};

class Animation
{

private:
	std::stack<AnimationClip> animationStack;

	std::vector<XMFLOAT4X4> interpolatedTransforms;

	Resources::Model* modelPtr;

	Resources::Skeleton* skeletonPtr;
	Resources::Skeleton::Joint* jointList;
	unsigned int jointCount;

	std::vector<SkelTemp> skeltempVec;

	const Resources::Animation* animationPtr;

	const Resources::Animation::AnimationJoint* animatedJointsList;

	

	int currentAnimation;
	float elapsedTime;

public: 
	Animation();
	~Animation();

	/*Update each frame in the current animation.*/
	void Update(float dt);

	/*Adds a new animation to the stack. Input arguments should come from somewhere else containing frame data.*/
	void Push(int newAnimation, bool isLooping, int startFrame, int endFrame, int duration);

	/*Removes the animation that is on the top of the stack.*/
	void Pop();

	/*Don't know if this is suppose to be here? Maybe this is a function
	we call from Update(), having another class holding interpolation func?*/
	void Interpolate(float currentTime, std::vector<XMFLOAT4X4> updatedTransforms);

	void ConvertFloatArrayToXMFloatMatrix(float floatArray[16], int jointIndex);

	void CalculateFinalTransform();

	/*Maybe this is more suitable for being in another class? I might consider
	calling this in the Update() from another class pointer.*/
	void Blend(int lastFrame, int prevState, int newState);
};


#endif

