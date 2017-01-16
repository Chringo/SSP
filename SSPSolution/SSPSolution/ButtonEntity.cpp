#include "ButtonEntity.h"



ButtonEntity::ButtonEntity()
{
}


ButtonEntity::~ButtonEntity()
{
}

int ButtonEntity::Update(float dT, InputHandler * inputHandler)
{
	int result = 0;
	return result;
}

int ButtonEntity::React(int entityID, EVENT reactEvent)
{
	int result = 0;
	return result;
}

int ButtonEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp)
{
	int result = 0;
	this->InitializeBase(entityID, pComp, gComp);
	return result;
}
