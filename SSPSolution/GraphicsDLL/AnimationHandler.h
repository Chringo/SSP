#ifndef GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H
#define GRAPHICSDLL_ANIMATIONS_ANIMATIONHANDLER_H

#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include "AnimationStateEnums.h"
#include "GraphicsComponent.h"
#include "../ResourceLib/ResourceHandler.h"

#define JOINT_COUNT 21

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
	int currentState = 0;
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

	Blending blendFlag = Blending::NO_TRANSITION;

	bool m_TransitionComplete = false;
	bool syncWalkSound = false;

	bool lockAnimation = false;

	Resources::Skeleton* skeleton = nullptr;
	std::vector<Resources::Animation*>* animation_States = nullptr;
};

enum BlendingIndex
{
	SOURCE_ANIMATION = 0,
	TARGET_ANIMATION = 1
};

struct BlendKeyframe
{
	DirectX::XMVECTOR trans;
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR quat;
};

struct VectorData
{
	std::vector<DirectX::XMMATRIX> localTransforms;
	std::vector<DirectX::XMMATRIX> localScales;
	std::vector<DirectX::XMMATRIX> relationTransform;
	std::vector<std::vector<BlendKeyframe>> blendKeysPerAnimation;
};

class AnimationHandler
{
private:

	//Variables used only in class.
	std::vector<VectorData> m_transformData;

	int m_nrOfAnimComps;
	int m_maxAnimComps;
	int m_AnimCompIndex;

	bool m_extractOnce;

	/*List with animations components*/
	std::vector<AnimationComponent*> m_AnimComponentList;

	int * m_nrOfGraphicsAnimationComponents;

	GraphicsAnimationComponent** m_animGraphicsComponents = nullptr;

public: 
	//Functions used outside the class.
	GRAPHICSDLL_API AnimationHandler();
	GRAPHICSDLL_API ~AnimationHandler();
	/*Called specifically when the game is shutdown.*/
	GRAPHICSDLL_API void ShutDown();
	GRAPHICSDLL_API void Initialize(GraphicsAnimationComponent ** graphicAnimComponents, int * noActiveComponents);
	GRAPHICSDLL_API void Update(float dt);
	/*Same functionality as ShutDown() but for the function name, deletes and clear all animation components.*/
	GRAPHICSDLL_API void ClearAnimationComponents();
	GRAPHICSDLL_API AnimationComponent* CreateAnimationComponent();
	GRAPHICSDLL_API AnimationComponent* GetNextAvailableComponent();
	
private:
	//Functions only used in class.
	void SetAnimCompIndex(int animCompIndex);
	void CalculateFinalTransform();
	void InterpolateKeys(Resources::Animation::AnimationState* animState, float globalTimeElapsed);
	void Blend(float secondsElapsed);
	void BlendKeys(float transitionTime);
	void ExtractSourceKeys(float sourceTime, float globalTime);
	void ExtractTargetKeys(float targetTime, float globalTime);
	void SetAnimationComponent(int animationState, float transitionDuration, Blending blendingType, bool isLooping, bool lockAnimation, float playingSpeed, float velocity);
};

#endif