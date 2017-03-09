#ifndef SSPAPPLICATION_ENTITIES_PLATFORMENTITY_H
#define SSPAPPLICATION_ENTITIES_PLATFORMENTITY_H
#include "Entity.h"
class PlatformEntity :
	public Entity
{
private:
	irrklang::ISound* m_ActiveSound;

public:
	PlatformEntity();
	virtual ~PlatformEntity();

	int Initialize(unsigned int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, AIComponent* aiComp);
	int Shutdown();
	int Update(float deltaTime, InputHandler* inputHandler);
	int React(unsigned int entityID, EVENT reactEvent);
	//void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	//void operator delete(void* p) { _aligned_free(p); };
};
#endif