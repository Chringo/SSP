#ifndef GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H
#define GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H

#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "GraphicsComponent.h"
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

struct AnimationComponent
{
	int active = 0;
	int animationState = 0;
	int previousState = 0;
	float startFrame = 0.f;
	float endFrame = 0.f;
	float localTime = 0.f;
	bool isLooping = false;
	
	/*Consider to use them here for each animation?*/
	//float transitionDuration;
	//float tranisitionTimeLeft;
	//BlendingStates blendingStates;
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

enum AnimationStates
{
	PLAYER_IDLE		=	0, 
	PLAYER_WALK		=	1,
	PLAYER_RUN		=	2, 
	PLAYER_THROW	=	3
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

	/*2D-array, which have a index for each Graphics Animation Component with their separate updating Animation Component lists.*/
	std::vector<std::vector<AnimationComponent*>> m_AnimationComponentList; 

	/*Used to set the current Graphics Animation Component separate animation stack.*/
	std::vector<AnimationComponent*> m_AnimationComponentStack;

	/*This is a temporary place for this I think?*/
	GraphicsAnimationComponent * m_graphicsAnimationComponent;

	BlendingStates m_BlendState;

	bool m_TransitionComplete;
	
	float m_globalTimeElapsed;
	float m_TransitionDuration;
	float m_TransitionTimeLeft;

	std::vector<AnimationDataContainer> m_AnimationDataContainer;
	AnimationDataContainer m_AnimationData;

public: 
	//Functions used outside the class.

	GRAPHICSDLL_API AnimationHandler();
	GRAPHICSDLL_API ~AnimationHandler();

	GRAPHICSDLL_API void Update(float dt);

	GRAPHICSDLL_API void SetAnimationDataContainer(GraphicsAnimationComponent* graphAnimationComponent, int index);
	GRAPHICSDLL_API AnimationDataContainer GetAnimationDataFromIndex(int index);
	GRAPHICSDLL_API void SetAnimationData(AnimationDataContainer animationData);

	GRAPHICSDLL_API int GetAnimationComponentCount(int graphicsAnimationIndex);
	GRAPHICSDLL_API std::vector<AnimationComponent*> GetAnimationComponentsFromIndex(int graphicsAnimationIndex);
	GRAPHICSDLL_API void SetAnimationComponents(std::vector<AnimationComponent*> animationComponents);

	//GraphicsAnimationComponent * GetGraphicsAnimationComponentTEMP() { return this->m_graphicsAnimationComponent; };

private:
	//Functions only used in class.

	void Push(int animationState, bool isLooping, float transitionTime);

	void Pop();

	float GetStartFrame(int animationState);

	float GetEndFrame(int animationState);

	void CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localMatrices);

	void InterpolateKeys(AnimationComponent* animationComponent, float globalTimeElapsed);

	void Blend(float secondsElapsed);

	void ExtractBlendingKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, AnimationComponent* animationComponent, float globalTimeElapsed, int animIndex);

	void BlendKeys(std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation, float transitionTime);
};

#endif

