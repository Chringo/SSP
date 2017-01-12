#include "MenuState.h"



MenuState::MenuState()
{
}


MenuState::~MenuState()
{
}

int MenuState::ShutDown()
{
	int result = 1;
	return result;
}

int MenuState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler, cameraRef);
	return result;
}

int MenuState::Update(float dt, InputHandler * inputHandler)
{
	return 0;
}
