#include "LevelState.h"



LevelState::LevelState()
{
}


LevelState::~LevelState()
{
}

int LevelState::ShutDown()
{
	int result = 1;
	return result;
}

int LevelState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler);

	//Read from file
	//Get Components
	//Set Component values
	//Give Components to entities
	this->m_player1.Initialize();

	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	int result = 0;

	this->m_player1.Update(dt, inputHandler);

	return result;
}
