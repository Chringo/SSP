#include "LevelSelectState.h"



LevelSelectState::LevelSelectState()
{
}


LevelSelectState::~LevelSelectState()
{
}

int LevelSelectState::ShutDown()
{
	int result = 1;
	return result;
}

int LevelSelectState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler);
	return result;
}

int LevelSelectState::Update(float dt, InputHandler * inputHandler)
{
	return 0;
}
