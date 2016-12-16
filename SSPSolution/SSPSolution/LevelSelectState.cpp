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

int LevelSelectState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* camera)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler, camera);
	return result;
}

int LevelSelectState::Update(float dt, InputHandler * inputHandler)
{
	return 0;
}
