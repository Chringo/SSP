#include "GameStateHandler.h"



GameStateHandler::GameStateHandler()
{
}


GameStateHandler::~GameStateHandler()
{
}

int GameStateHandler::Initialize()
{
	return 1;
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
