#include "DynamicEntity.h"

DynamicEntity::DynamicEntity()
{
}

DynamicEntity::~DynamicEntity()
{
}


int DynamicEntity::Initialize(unsigned int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, AnimationComponent* aComp, AIComponent* aiComp)
{
	this->InitializeBase(entityID, pComp, gComp, aComp, aiComp);
	return 0;
}

int DynamicEntity::Update(float dT, InputHandler * inputHandler)
{
	int result = 1;

	//Copy the world matrix into the graphics component for rendering purposes

	this->SyncComponents();

	return result;
}

int DynamicEntity::React(unsigned int entityID, EVENT reactEvent)
{
	return 0;
}
