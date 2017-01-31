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

struct AnimationComponent
{
	/*System variables.*/
	int active = 0;

	/*Used to check if a state is either new or old.*/
	int previousState = 1;

	/*Used to play one or two animations, for blending.*/
	Resources::Animation::AnimationState* source_State; // Always available
	Resources::Animation::AnimationState* target_State;
	/*The source and target local times.*/
	float source_Time = 0.f;
	float target_Time = 0.f;

	float m_TransitionDuration = 0.f;
	float m_TransitionTimeLeft = 0.f;

	Blending blendFlag = NO_TRANSITION; // Determines if blending should occur or not.

	Resources::Skeleton* skeleton = nullptr;
	std::vector<Resources::Animation*>* animation_States = nullptr;
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

	/*List with animations components*/
	std::vector<AnimationComponent*> m_AnimComponentList;

	bool m_TransitionComplete;

	int * m_nrOfGraphicsAnimationComponents;
	GraphicsAnimationComponent** m_animGraphicsComponents = nullptr;

public: 
	//Functions used outside the class.
	GRAPHICSDLL_API AnimationHandler();
	GRAPHICSDLL_API ~AnimationHandler();
	GRAPHICSDLL_API void ShutDown();
	GRAPHICSDLL_API void Initialize(GraphicsAnimationComponent ** graphicAnimComponents, int * noActiveComponents);
	GRAPHICSDLL_API void Update(float dt);

	GRAPHICSDLL_API AnimationComponent* CreateAnimationComponent();
	GRAPHICSDLL_API AnimationComponent* GetNextAvailableComponent();

	GRAPHICSDLL_API void UpdateAnimationComponents(float dt);
	
private:
	//Functions only used in class.

	//void Push(AnimStateData animState);
	//void Pop();
	void CalculateFinalTransform(std::vector<DirectX::XMMATRIX> localMatrices);
	void InterpolateKeys(Resources::Animation::AnimationState* animState, float globalTimeElapsed);
	void Blend(float secondsElapsed);
	void BlendKeys(std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation, float transitionTime);

	//void ExtractBlendingKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation);
	void ExtractSourceKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, float sourceTime, float globalTime);
	void ExtractTargetKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, float targetTime, float globalTime);
};

#endif

