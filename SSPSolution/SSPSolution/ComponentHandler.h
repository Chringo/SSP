#ifndef SSPAPPLICATION_COMPONENTHANDLER_H
#define SSPAPPLICATION_COMPONENTHANDLER_H

#include "../GraphicsDLL/GraphicsHandler.h"
//#include "ComponentStructs.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../physicsDLL/physicsDLL/PhysicsHandler.h"

class ComponentHandler
{
private:
	GraphicsHandler* m_graphicsHandler;
	PhysicsHandler* m_physicsHandler;
public:
	ComponentHandler();
	~ComponentHandler();

	//Returns 0 if the graphicsHandler or physicshandler is a nullptr
	int Initialize(GraphicsHandler* graphicsHandler, PhysicsHandler* physicsHandler);

	GraphicsComponent* GetGraphicsComponent();
	PhysicsComponent* GetPhysicsComponent();
	void UpdateGraphicsComponents();
	void SetGraphicsComponentListSize(int gCompSize);
};

#endif