#ifndef GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H
#define GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H

#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "AnimationStateEnums.h"
#include "GraphicsComponent.h"
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

#define ANIMSTATE_MAX_COUNT 8


struct AnimStateData
{
	/*Animation state information.*/
	int animationState = 0;
	float startFrame = 0.f;
	float endFrame = 0.f;
	float localTime = 0.f;
	bool isLooping = false;
};
struct AnimationComponent
{
	/*System variables.*/
	int active = 0;

	/*Animation array data.*/
	AnimStateData Anim_StateData[ANIMSTATE_MAX_COUNT];
};

struct Skeleton
{
	DirectX::XMMATRIX inverseBindPose;
	int parentIndex;
	int jointIndex;
};

struct AnimationDataContainer
{
	std::vector<Skeleton> skeleton;
	std::vector<const Resources::Animation::AnimationJoint*> animations;
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
	//Variables used only in class.

	int m_nrOfAnimComps;
	int m_maxAnimComps;

	/*2D-array, which have a index for each Graphics Animation Component with their separate updating Animation Component lists.*/
	std::vector<AnimationComponent*> m_AnimComponentList;

	BlendingStates m_BlendState;

	bool m_TransitionComplete;
	
	float m_globalTimeElapsed;
	float m_TransitionDuration;
	float m_TransitionTimeLeft;

	int * m_nrOfGraphicsAnimationComponents;
	GraphicsAnimationComponent** m_animGraphicsComponents = nullptr;

	/*This is the data that is stored per model that have a skeleton and keyframe animations.
	Examples include: both the two players and animatible props like levers and doors.*/
	std::vector<AnimationDataContainer> m_AnimationData;

	SkelAnimModels m_skelAnimModel;

	/*Used to set the current Graphics Animation Component separate animation stack.*/
	std::vector<AnimStateData> m_AnimationComponentStack;

public: 
	//Functions used outside the class.
	GRAPHICSDLL_API AnimationHandler();
	GRAPHICSDLL_API AnimationHandler(GraphicsAnimationComponent** graphicAnimComponents, int* noActiveComponents);
	GRAPHICSDLL_API ~AnimationHandler();

	GRAPHICSDLL_API void Update(float dt);

	GRAPHICSDLL_API void SetAnimationDataContainer(GraphicsAnimationComponent* graphAnimationComponent, int index);

	GRAPHICSDLL_API AnimationComponent* CreateAnimationComponent();
	GRAPHICSDLL_API AnimationComponent* GetNextAvailableComponent();

	GRAPHICSDLL_API void ShutDown();

private:
	//Functions only used in class.

	void Push(AnimStateData animState);

	void Pop();

	float GetStartFrame(int animationState);

	float GetEndFrame(int animationState);

	void CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localMatrices);

	void InterpolateKeys(AnimStateData animState, float globalTimeElapsed);

	void Blend(float secondsElapsed);

	void ExtractBlendingKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, AnimStateData animStateData, float globalTimeElapsed, int animIndex);

	void BlendKeys(std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation, float transitionTime);
};

#endif

