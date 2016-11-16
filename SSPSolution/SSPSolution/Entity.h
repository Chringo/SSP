#ifndef SSPAPPLICATION_ENTITIES_ENTITY_H
#define SSPAPPLICATION_ENTITIES_ENTITY_H
#include "Observer.h"
#include "Subject.h"
//Subject includes this for the events
#include "ComponentStructs.h"

class Entity :
	public Observer
{
private:
protected:
	int m_entityID;
	Subject* subject;
	PhysicsComponent* pComp;
	GraphicsComponent* gComp;
public:
	Entity();
	virtual ~Entity();


	virtual int Update() = 0;
	virtual int React(int entityID, EVENT event) = 0;
private:
};

#endif