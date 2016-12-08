#ifndef SSPAPPLICATION_ENTITIES_ENTITY_H
#define SSPAPPLICATION_ENTITIES_ENTITY_H
#include "Observer.h"
#include "Subject.h"
#include "InputHandler.h"
//Subject includes this for the events
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../physicsDLL/physicsDLL/PhysicsHandler.h"

class Entity :
	public Observer
{
private:
protected:
	int m_entityID;
	Subject* m_subject;
	PhysicsComponent* m_pComp;
	GraphicsComponent* m_gComp;
	//PhysicsComponent* m_pComp;
	//GraphicsComponent* m_gComp;
public:
	Entity();
	virtual ~Entity();

	virtual int Initialize() = 0;
	//dT should be in seconds
	virtual int Update(float dT, InputHandler* inputHandler) = 0;
	virtual int React(int entityID, EVENT reactEvent) = 0;
private:
};

#endif