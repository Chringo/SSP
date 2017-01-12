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
	int active = 0;
	int modelID = -1;
	int skeletonID = 1;
	int animationState = 0;
};

struct SkeletonTemp
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
	bool isActive;

	float startFrame;
	float endFrame;

	float localTime;
};

class AnimationHandler
{

private:

	std::vector<AnimationClip> m_animationStack;

	GraphicsAnimationComponent * m_graphicsAnimationComponent;

	bool m_newAnimation;
	bool m_isComplete;
	
	float m_globalTimeElapsed;
	
	float m_BlendDuration;
	float m_BlendTimeLeft;

	std::vector<SkeletonTemp> m_skeletonContainer;

	std::vector<const Resources::Animation::AnimationJoint*> m_animationContainer;

	std::vector<DirectX::XMFLOAT4X4> m_localTransforms;

public: 

	AnimationHandler();
	~AnimationHandler();

	void Update(float dt);

	void Push(int animationState, bool newAnimation, bool isLooping, float weight);

	void Pop();

	Resources::Model* GetAnimatedModel(int modelId);
	Resources::Skeleton* GetSkeleton(Resources::Model* modelPtr);
	Resources::Animation* GetAnimations(Resources::Skeleton* skeletonPtr);

	float GetStartFrame(int animationState);
	float GetEndFrame(int animationState);

	void Interpolate(AnimationClip clipA, float globalTimeElapsed);

	void CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localMatrices);

	GraphicsAnimationComponent * GetGraphicsAnimationComponentTEMP() { return this->m_graphicsAnimationComponent; };
};


#endif

