#ifndef SSPAPPLICATION_ENTITIES_DOORENTITY_H
#define SSPAPPLICATION_ENTITIES_DOORENTITY_H

#include "Entity.h"

class DoorEntity :
	public Entity
{
private:
	bool m_isOpened;

public:
	DoorEntity();
	virtual ~DoorEntity();

	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);
};

#endif