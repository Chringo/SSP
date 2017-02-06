#ifndef SSPAPPLICATION_ENTITIES_PLATFORMENTITY_H
#define SSPAPPLICATION_ENTITIES_PLATFORMENTITY_H
#include "Entity.h"
class PlatformEntity :
	public Entity
{
private:

public:
	PlatformEntity();
	virtual ~PlatformEntity();

	int Update(float deltaTime, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

};
#endif