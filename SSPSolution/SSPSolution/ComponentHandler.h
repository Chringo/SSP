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

public:
	ComponentHandler();
	~ComponentHandler();

	int Initialize(GraphicsHandler* graphicsHandler);

	GraphicsComponent* GetGraphicsComponent();
	PhysicsComponent* GetPhysicsComponent();
	void UpdateGraphicsComponents();
	void SetGraphicsComponentListSize(int gCompSize);
};

#endif