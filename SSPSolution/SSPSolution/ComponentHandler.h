#ifndef SSPAPPLICATION_COMPONENTHANDLER_H
#define SSPAPPLICATION_COMPONENTHANDLER_H

//#include "ComponentStructs.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/AnimationHandler.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "../AIDLL/AIHandler.h"

class ComponentHandler
{
private:
	GraphicsHandler* m_graphicsHandler;
	AnimationHandler* m_AnimationHandler;
	PhysicsHandler* m_physicsHandler;
	AIHandler* m_aiHandler;
public:
	ComponentHandler();
	~ComponentHandler();

	//Returns 0 if the graphicsHandler or physicshandler is a nullptr
	int Initialize(GraphicsHandler* graphicsHandler, AnimationHandler* animationHandler, PhysicsHandler* physicsHandler, AIHandler* aiHandler);

	GraphicsComponent* GetGraphicsComponent();
	AnimationComponent* GetAnimationComponent();
	PhysicsComponent* GetPhysicsComponent();
	AIComponent* GetAIComponent();
	void UpdateGraphicsComponents();
	void UpdateAIComponents();
	void SetGraphicsComponentListSize(int gCompSize);

	//temporary function
	PhysicsHandler* GetPhysicsHandler() const;
};

#endif