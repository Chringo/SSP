#ifndef SSPAPPLICATION_COMPONENTHANDLER_H
#define SSPAPPLICATION_COMPONENTHANDLER_H

//#include "ComponentStructs.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "../AIDLL/AIHandler.h"
#include "SoundHandler.h"

class ComponentHandler
{
private:
	GraphicsHandler* m_graphicsHandler;
	PhysicsHandler* m_physicsHandler;
	AIHandler* m_aiHandler;
	SoundHandler* m_soundHandler;

public:
	ComponentHandler();
	~ComponentHandler();

	//Returns 0 if the graphicsHandler or physicshandler is a nullptr
	int Initialize(GraphicsHandler* graphicsHandler, PhysicsHandler* physicsHandler, AIHandler* aiHandler, SoundHandler* soundHandler);

	GraphicsComponent* GetGraphicsComponent();
	PhysicsComponent* GetPhysicsComponent();
	UIComponent* GetUIComponent();
	TextComponent* GetTextComponent();
	AIComponent* GetAIComponent();
	SoundComponent2D* GetSoundComponent2D();

	void UpdateGraphicsComponents();
	void UpdateAIComponents();
	void UpdateSoundHandler();
	void SetGraphicsComponentListSize(int gCompSize);

	PhysicsComponent* GetClosestPhysicsComponent(PhysicsComponent* component, int minDistance);

	//temporary function
	PhysicsHandler* GetPhysicsHandler() const;
};

#endif