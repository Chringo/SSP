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

public:
	ComponentHandler();
	~ComponentHandler();

	//Returns 0 if the graphicsHandler or physicshandler is a nullptr
	int Initialize(GraphicsHandler* graphicsHandler, PhysicsHandler* physicsHandler, AIHandler* aiHandler, AnimationHandler* aHandler);

	GraphicsComponent* GetStaticGraphicsComponent();
	GraphicsComponent* GetDynamicGraphicsComponent();
	GraphicsComponent* GetPersistentGraphicsComponent();
	GraphicsAnimationComponent * GetGraphicsAnimationComponent();
	PhysicsComponent* GetPhysicsComponent();
	UIComponent* GetUIComponent();
	TextComponent* GetTextComponent();
	AIComponent* GetAIComponent();
	AnimationComponent* GetAnimationComponent();

	void UpdateGraphicsComponents();
	void UpdateGraphicsAnimationComponents();
	void UpdateAIComponents();
	void UpdateSoundHandler();
	void SetGraphicsComponentListSize(int gCompSize);
	void SetGraphicsAnimationComponentListSize(int gCompSize);

	int ResizeGraphicsStatic(size_t newCap);
	int ResizeGraphicsDynamic(size_t newCap);
	int ResizeGraphicsPersistent(size_t newCap);
	int ClearAminationComponents();
	int ClearAIComponents();
	int RemoveUIComponentFromPtr(UIComponent* ptr);
	int RemoveLastUIComponent();

	void WaypointTime();

	//temporary function
	PhysicsHandler* GetPhysicsHandler() const;
	GraphicsHandler* GetGraphicsHandler() const;
};

#endif