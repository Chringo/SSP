#include "PlatformEntity.h"
PlatformEntity::PlatformEntity(){}
PlatformEntity::~PlatformEntity(){}

int PlatformEntity::Update(float deltaTime, InputHandler * inputHandler)
{
	this->SyncComponents();
	return 1;
}

int PlatformEntity::React(int entityID, EVENT reactEvent)
{

	return 1;
}

int PlatformEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, AIComponent * aiComp)
{
	this->InitializeBase(entityID, pComp, gComp, nullptr, aiComp);
	return 1;
}
