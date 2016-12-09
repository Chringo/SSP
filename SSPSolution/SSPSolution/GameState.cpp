#include "GameState.h"
#include "GameStateHandler.h"


GameState::GameState()
{
}


GameState::~GameState()
{
}

int GameState::InitializeBase(GameStateHandler * gsh)
{
	int result = 0;
	if(gsh != nullptr)
		this->m_gsh = gsh;
	else
	{
		this->m_gsh = nullptr;
		result = 0;
	}
	return result;
}