#include "GameStateHandler.h"



GameStateHandler::GameStateHandler()
{
}


GameStateHandler::~GameStateHandler()
{
}

int GameStateHandler::ShutDown()
{
	//Delete the states
	while (this->m_statesToRemove.size())
	{
		GameState* temp;
		temp = this->m_stateStack.back();
		delete temp;
		temp = nullptr;
		this->m_stateStack.pop_back();
	}
	//Delete the states that should have been popped
	while (this->m_statesToRemove.size())
	{
		GameState* temp;
		temp = this->m_statesToRemove.back();
		delete temp;
		temp = nullptr;
		this->m_statesToRemove.pop_back();
	}
	return 1;
}


int GameStateHandler::Initialize(ComponentHandler * cHandler)
{
	int result = 0;
	
	//Create, Initialize and push a LevelState
	LevelState* tempState = new LevelState();
	result = tempState->Initialize(this, cHandler);
	//If the initialization was successful
	if (result > 0)
	{
		//Push it to the gamestate stack/vector
		this->m_stateStack.push_back(tempState);
	}
	return result;
}

int GameStateHandler::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;

	//Update the active state
	if (this->m_stateStack.size())
	{
		this->m_stateStack.back()->Update(dt, inputHandler);
	}
	//Delete the states
	while (this->m_statesToRemove.size())
	{
		GameState* temp;
		temp = this->m_statesToRemove.back();
		delete temp;
		temp = nullptr;
		this->m_statesToRemove.pop_back();
	}
	return result;
}
