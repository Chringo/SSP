#ifndef SSPAPPLICATION_ENTITIES_DYNAMICENTITY_H
#define SSPAPPLICATION_ENTITIES_DYNAMICENTITY_H
#include "Entity.h"
class DynamicEntity :
	public Entity
{
public:
	DynamicEntity();
	~DynamicEntity();

	int Initialize(unsigned int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, AnimationComponent* aComp = nullptr, AIComponent* aiComp = nullptr);

	int Update(float dT, InputHandler* inputHandler);
	int React(unsigned int entityID, EVENT reactEvent);


};
#endif