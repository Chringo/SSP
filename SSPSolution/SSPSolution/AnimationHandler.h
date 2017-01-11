#ifndef SSPAPPLICATION_ANIMATION_ANIMATION_H
#define SSPAPPLICATION_ANIMATION_ANIMATION_H

#include <stack>
#include <queue>
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")

struct GraphicsAnimationComponent
{
	int active = 0;
	int modelID = -1;
	int jointCount = 0;

	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX finalJointTransforms[32];
};

struct AnimationComponent
{
	Resources::Model* model;
	Resources::Skeleton* skeleton;
	Resources::Animation* animations;
};

struct SkelTemp
{
	int parentIndex;
	int jointIndex;
	DirectX::XMMATRIX invBindPose;
};

struct AnimationClip
{
	int animationState;
	int previousState;
	bool isLooping;
	float startFrame;
	float endFrame;
};

class AnimationHandler
{

private:
	std::stack<AnimationClip> animationStack;

	GraphicsAnimationComponent * m_graphicsAnimationComponent;

	Resources::Model* modelPtr;

	Resources::Skeleton* skeletonPtr;
	Resources::Skeleton::Joint* jointList;
	unsigned int jointCount;

	std::vector<SkelTemp> skeltempVec;

	const Resources::Animation* animationPtr;

	const Resources::Animation::AnimationJoint* animatedJointsList;

	std::vector<DirectX::XMFLOAT4X4> interpolatedTransforms;

	float elapsedTime;
	bool newAnimation;

public: 
	AnimationHandler();
	~AnimationHandler();

	/*Update each frame in the current animation.*/
	void Update(float dt);

	/*Adds a new animation to the stack. Input arguments should come from somewhere else containing frame data.*/
	void Push(int animationState, bool newAnimation);

	/*Removes the animation that is on the top of the stack.*/
	void Pop();

	void GetAnimationState(int animationState, AnimationClip& clip);

	void Interpolate(float currentTime);

	void ConvertFloatArrayToXMFloatMatrix(float floatArray[16], int jointIndex);

	void CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localMatrices);

	GraphicsAnimationComponent * GetGraphicsAnimationComponentTEMP() { return this->m_graphicsAnimationComponent; };
};


#endif

