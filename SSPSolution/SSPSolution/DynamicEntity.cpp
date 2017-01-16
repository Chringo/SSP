#include "DynamicEntity.h"

DynamicEntity::DynamicEntity()
{
}

DynamicEntity::~DynamicEntity()
{
}


int DynamicEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp)
{
	this->InitializeBase(entityID, pComp, gComp);
	return 0;
}

int DynamicEntity::Update(float dT, InputHandler * inputHandler)
{
	return 0;
}

int DynamicEntity::React(int entityID, EVENT reactEvent)
{
	return 0;
}
