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
	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, AIComponent* aiComp);
	//void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	//void operator delete(void* p) { _aligned_free(p); };
};
#endif