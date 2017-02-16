#ifndef GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H
#define GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H

#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "AnimationStateEnums.h"
#include "GraphicsComponent.h"
#include "../ResourceLib/ResourceHandler.h"

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
	int previousState = 0;

	/*Used to play one or two animations, for blending.*/
	Resources::Animation::AnimationState* source_State; // Always available
	Resources::Animation::AnimationState* target_State;
	/*The source and target local times.*/
	float source_Time = 0.f;
	float target_Time = 0.f;

	float transitionDuration = 0.f;
	float transitionTimeLeft = 0.f;
	float playingSpeed = 1.f;
	float velocity = 1.f;

	bool m_TransitionComplete = false;
	bool syncWalkSound = false;

	Blending blendFlag = NO_TRANSITION; // Determines if blending should occur or not.

	bool lockAnimation = false;

	Resources::Skeleton* skeleton = nullptr;
	std::vector<Resources::Animation*>* animation_States = nullptr;
};

struct BlendKeyframe
{
	DirectX::XMFLOAT3 trans;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 quat;
};

class AnimationHandler
{
private:
	//Variables used only in class.
	int m_nrOfAnimComps;
	int m_maxAnimComps;

	int m_AnimCompIndex;

	/*List with animations components*/
	std::vector<AnimationComponent*> m_AnimComponentList;

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
	void SetAnimCompIndex(int animCompIndex);
	void CalculateFinalTransform(std::vector<DirectX::XMFLOAT4X4> localMatrices, std::vector<DirectX::XMFLOAT4X4> localScales);
	void InterpolateKeys(Resources::Animation::AnimationState* animState, float globalTimeElapsed);
	void Blend(float secondsElapsed);
	void BlendKeys(std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation, float transitionTime);
	void ExtractSourceKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, float sourceTime, float globalTime);
	void ExtractTargetKeys(std::vector<std::vector<BlendKeyframe>>& blendKeysPerAnimation, float targetTime, float globalTime);
	void SetAnimationComponent(int animationState, float transitionDuration, Blending blendingType, bool isLooping, float playingSpeed);
	void CheckPlayerFootPosSynch(int player, int runningState, int foot, int keyframeIndex);
};

#endif

