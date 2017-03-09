#include "StaticEntity.h"



StaticEntity::StaticEntity()
{
}


StaticEntity::~StaticEntity()
{
}

int StaticEntity::Initialize(unsigned int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, AnimationComponent* aComp, AIComponent * aiComp)
{
	this->InitializeBase(entityID, pComp, gComp, aComp, aiComp);
	return 0;
}


int StaticEntity::Update(float dT, InputHandler * inputHandler)
{
	return 0;
}

int StaticEntity::React(unsigned int entityID, EVENT reactEvent)
{
	return 0;
}
