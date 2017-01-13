#include "StaticEntity.h"



StaticEntity::StaticEntity()
{
}


StaticEntity::~StaticEntity()
{
}

int StaticEntity::Initialize()
{
	this->InitializeBase();
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
