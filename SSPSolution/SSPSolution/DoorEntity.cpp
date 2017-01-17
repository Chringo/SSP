#include "DoorEntity.h"

DoorEntity::DoorEntity()
{
}


DoorEntity::~DoorEntity()
{
}

int DoorEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp)
{
	this->InitializeBase(entityID, pComp, gComp);

	return 0;
}

int DoorEntity::Update(float dT, InputHandler * inputHandler)
{
	return 0;
}

int DoorEntity::React(int entityID, EVENT reactEvent)
{
	return 0;
}
