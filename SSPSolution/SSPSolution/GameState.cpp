#include "GameState.h"
#include "GameStateHandler.h"

GameState::GameState()
{
}


GameState::~GameState()
{
}

int GameState::InitializeBase(GameStateHandler * gsh, ComponentHandler * cHandler, Camera * cameraRef)
{
	int result = 0;
	if (gsh == nullptr || cHandler == nullptr || cameraRef == nullptr)
	{
		this->m_gsh = nullptr;
		this->m_cHandler = nullptr;
		this->m_cameraRef = nullptr;
		result = 0;
	}
	else
	{
		this->m_gsh = gsh;
		this->m_cHandler = cHandler;
		this->m_cameraRef = cameraRef;
		result = 1;
	}
	return result;
}