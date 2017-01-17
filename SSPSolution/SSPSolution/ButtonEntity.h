#ifndef SSPAPPLICATION_ENTITIES_BUTTONENTITY_H
#define SSPAPPLICATION_ENTITIES_BUTTONENTITY_H
#include "Entity.h"
class ButtonEntity :
	public Entity
{
private:
	//Variables
	int isActive;
	float range;
public:
	ButtonEntity();
	virtual ~ButtonEntity();

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp);

	//For now we check only if the player is close enough
	int CheckPressed(DirectX::XMFLOAT3 playerPos);

private:
	//Functions
};

#endif