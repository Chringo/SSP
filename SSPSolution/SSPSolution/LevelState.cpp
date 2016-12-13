#include "LevelState.h"



LevelState::LevelState()
{
}


LevelState::~LevelState()
{
}

int LevelState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler);
	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	return 0;
}
