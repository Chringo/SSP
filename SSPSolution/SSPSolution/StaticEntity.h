#ifndef SSPAPPLICATION_ENTITIES_STATICENTITY_H
#define SSPAPPLICATION_ENTITIES_STATICENTITY_H
#include "Entity.h"
class StaticEntity :
	public Entity
{
public:
	StaticEntity();
	~StaticEntity();

	int Initialize();

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);


};
#endif
