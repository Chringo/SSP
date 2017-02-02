#ifndef SSPAPPLICATION_COMPONENTHANDLER_H
#define SSPAPPLICATION_COMPONENTHANDLER_H

//#include "ComponentStructs.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/AnimationHandler.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "../AIDLL/AIHandler.h"
#include "SoundHandler.h"

class ComponentHandler
{
private:
	GraphicsHandler* m_graphicsHandler;
	PhysicsHandler* m_physicsHandler;
	AIHandler* m_aiHandler;
	AnimationHandler* m_aHandler;
	SoundHandler* m_soundHandler;

public:
	ComponentHandler();
	~ComponentHandler();

	//Returns 0 if the graphicsHandler or physicshandler is a nullptr
	int Initialize(GraphicsHandler* graphicsHandler, PhysicsHandler* physicsHandler, AIHandler* aiHandler, AnimationHandler* aHandler, SoundHandler* soundHandler);

	GraphicsComponent* GetGraphicsComponent();
	GraphicsAnimationComponent * GetGraphicsAnimationComponent();
	PhysicsComponent* GetPhysicsComponent();
	UIComponent* GetUIComponent();
	TextComponent* GetTextComponent();
	AIComponent* GetAIComponent();
	AnimationComponent* GetAnimationComponent();
	SoundHandler* GetSoundHandler();

	void UpdateGraphicsComponents();
	void UpdateGraphicsAnimationComponents();
	void UpdateAIComponents();
	void UpdateListnerPos(DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newLookDir, DirectX::XMFLOAT3 newUpVector);
	void SetGraphicsComponentListSize(int gCompSize);
	void SetGraphicsAnimationComponentListSize(int gCompSize);
	PhysicsComponent* GetClosestPhysicsComponent(PhysicsComponent* component, int minDistance);

	//temporary function
	PhysicsHandler* GetPhysicsHandler() const;
};

#endif