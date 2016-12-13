#include "StartState.h"



StartState::StartState()
{
}


StartState::~StartState()
{
}

int StartState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler);
	return result;
}

int StartState::Update(float dt, InputHandler * inputHandler)
{
	return 0;
}
