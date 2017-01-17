#ifndef SSPAPPLICATION_ENTITIES_BUTTONENTITY_H
#define SSPAPPLICATION_ENTITIES_BUTTONENTITY_H
#include "Entity.h"
class ButtonEntity :
	public Entity
{
private:
	//Variables
	int isActive;
public:
	ButtonEntity();
	virtual ~ButtonEntity();


	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp);

private:
	//Functions
};

#endif