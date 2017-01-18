#include "StaticEntity.h"



StaticEntity::StaticEntity()
{
}


StaticEntity::~StaticEntity()
{
}

int StaticEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp)
{
	this->InitializeBase(entityID, pComp, gComp);
	return 0;
}


int StaticEntity::Update(float dT, InputHandler * inputHandler)
{
	return 0;
}

int StaticEntity::React(int entityID, EVENT reactEvent)
{
	return 0;
}
