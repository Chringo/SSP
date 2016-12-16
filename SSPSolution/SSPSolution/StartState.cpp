#include "StartState.h"



StartState::StartState()
{
}


StartState::~StartState()
{
}

int StartState::ShutDown()
{
	int result = 1;
	return result;
}

int StartState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* camera)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler, camera);
	return result;
}

int StartState::Update(float dt, InputHandler * inputHandler)
{
	return 0;
}
