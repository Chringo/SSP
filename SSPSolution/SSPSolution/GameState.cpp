#include "GameState.h"
#include "GameStateHandler.h"

NetworkModule* GameState::m_networkModule = nullptr;

GameState::GameState()
{


}


GameState::~GameState()
{
}

bool GameState::GetManualRemoval()
{
	return this->m_manualRemoval;
}

int GameState::InitializeBase(GameStateHandler * gsh, ComponentHandler * cHandler, Camera * cameraRef, bool manualRemoval)
{
	int result = 0;
	this->m_manualRemoval = manualRemoval;
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
