#ifndef GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H
#define GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H

#include <stack>
#include <queue>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <iostream>
#include "GraphicsComponent.h"
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

//struct GraphicsAnimationComponent
//{
//	int active = 0;
//	int modelID = -1;
//	int jointCount = 0;
//
//	DirectX::XMMATRIX worldMatrix;
//	DirectX::XMMATRIX finalJointTransforms[32];
//};

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

enum AnimationStates
{
	IDLE_STATE	=	0, 
	WALK_STATE	=	1,
	RUN_STATE	=	2, 
	THROW_STATE =	3
};

enum BlendingStates
{
	NO_TRANSITION		=	0,
	SMOOTH_TRANSITION	=	1,
	FROZEN_TRANSITION	=	2
};

struct BlendKeyframe
{
	DirectX::XMVECTOR trans;
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR quat;
};

class AnimationHandler
{

private:
	//Variables
	std::vector<AnimationClip> m_animationStack;

	GraphicsAnimationComponent * m_graphicsAnimationComponent;

	BlendingStates m_BlendState;
	bool m_TransitionComplete;
	
	float m_globalTimeElapsed;
	
	float m_TransitionDuration;
	float m_TransitionTimeLeft;

	std::vector<SkeletonTemp> m_skeletonContainer;

	std::vector<const Resources::Animation::AnimationJoint*> m_animationContainer;

	//std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation;

	//std::vector<DirectX::XMFLOAT4X4> m_localTransforms;

public: 

	AnimationHandler();
	~AnimationHandler();

	void AddAnimation(int animationState, bool isLooping, float transitionTime);

	GRAPHICSDLL_API void Update(float dt);

	void Push(int animationState, bool isLooping, float transitionTime);

	void Pop();

	Resources::Model* GetAnimatedModel(int modelId);
	Resources::Skeleton* GetSkeleton(Resources::Model* modelPtr);
	Resources::Animation* GetAnimations(Resources::Skeleton* skeletonPtr);

	float GetStartFrame(int animationState);
	float GetEndFrame(int animationState);

	void InterpolateKeys(AnimationClip animationClip, float globalTimeElapsed);
	void ExtractBlendingKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, AnimationClip animationClip, float globalTimeElapsed, int animIndex);

	void BlendKeys(std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation, float transitionTime);

	void Blend(float secondsElapsed);

	void CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localMatrices);

	GraphicsAnimationComponent * GetGraphicsAnimationComponentTEMP() { return this->m_graphicsAnimationComponent; };
private:
	//Functions
};


#endif

