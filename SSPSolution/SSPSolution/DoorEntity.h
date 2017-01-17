#ifndef SSPAPPLICATION_ENTITIES_DOORENTITY_H
#define SSPAPPLICATION_ENTITIES_DOORENTITY_H

#include "Entity.h"

class DoorEntity :
	public Entity
{
private:
	bool m_isOpened;
	float m_minRotation;
	float m_maxRotation;
	float m_rotateTime;
	float m_rotatePerSec;

public:
	DoorEntity();
	virtual ~DoorEntity();

	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, float minRotation, float maxRotation, float rotateTime);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	bool SetIsOpened(bool isOpened);
	bool GetIsOpened();
};

#endif