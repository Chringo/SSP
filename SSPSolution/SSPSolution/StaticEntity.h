#ifndef SSPAPPLICATION_ENTITIES_STATICENTITY_H
#define SSPAPPLICATION_ENTITIES_STATICENTITY_H
#include "Entity.h"
class StaticEntity :
	public Entity
{
private:
	//Variables
public:
	StaticEntity();
	~StaticEntity();

	int Initialize(unsigned int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, AnimationComponent* aComp, AIComponent* aiComp = nullptr);

	int Update(float dT, InputHandler* inputHandler);
	int React(unsigned int entityID, EVENT reactEvent);
private:
	//Functions
};
#endif
