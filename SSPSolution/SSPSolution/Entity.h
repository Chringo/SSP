#ifndef SSPAPPLICATION_ENTITIES_ENTITY_H
#define SSPAPPLICATION_ENTITIES_ENTITY_H
#include "Observer.h"
#include "Subject.h"
#include "InputHandler.h"
//Subject includes this for the events
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "../AIDLL/AIHandler.h"
#include "ComponentHandler.h"
class Entity :
	public Observer
{
private:
protected:
	int m_entityID;
	Subject m_subject;
	PhysicsComponent* m_pComp;
	GraphicsComponent* m_gComp;
	AIComponent* m_aiComp;
	int InitializeBase();

public:
	Entity();
	virtual ~Entity();

	virtual int Initialize() = 0;
	//dT should be in seconds
	virtual int Update(float dT, InputHandler* inputHandler) = 0;
	virtual int React(int entityID, EVENT reactEvent) = 0;

	PhysicsComponent* SetPhysicsComponent(PhysicsComponent* pComp);
	GraphicsComponent* SetGraphicsComponent(GraphicsComponent* gComp);
	AIComponent* SetAIComponent(AIComponent* aiComp);

	PhysicsComponent* GetPhysicsComponent();
	GraphicsComponent* GetGraphicComponent();
	AIComponent* GetAIComponent();


private:
};

#endif