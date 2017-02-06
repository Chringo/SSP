#include "PlatformEntity.h"
PlatformEntity::PlatformEntity()
{
}
PlatformEntity::~PlatformEntity()
{
}

int PlatformEntity::Update(float deltaTime, InputHandler * inputHandler)
{

	return 1;
}

int PlatformEntity::React(int entityID, EVENT reactEvent)
{

	return 1;
}
