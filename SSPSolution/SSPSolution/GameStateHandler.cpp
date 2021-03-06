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
	while (this->m_stateStack.size())
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

	//Shutdown the NetworkModule that is shared with all GameStates
	if (GameState::m_networkModule)	//If it is active
	{
		GameState::m_networkModule->Shutdown();
		delete GameState::m_networkModule;
		GameState::m_networkModule = nullptr;
	}


	return 1;
}


int GameStateHandler::Initialize(ComponentHandler * cHandler, Camera * cameraRef, std::string levelPath)
{
	int result = 0;
	
#ifndef START_WITHOUT_MENU
	StartState* startState = new StartState();
	result = startState->Initialize(this, cHandler, cameraRef);

	if (result > 0)
	{
		//Push it to the gamestate stack/vector
		this->m_stateStack.push_back(startState);
	}
	else
	{
		//Delete it
		delete startState;
		startState = nullptr;
	}
#else
	//Create, Initialize and push a LevelSelectState
	LevelSelectState* levelSelect = new LevelSelectState();
	result = levelSelect->Initialize(this, cHandler, cameraRef);

	//If the initialization was successful
	if (result > 0)
	{
		//Push it to the gamestate stack/vector
		//this->PushStateToStack(levelSelect);

		if (levelPath.length() < 2)
		{
			//levelSelect->LoadLevel(std::string("../ResourceLib/AssetFiles/L1P1.level"));
			levelSelect->LoadLevel(std::string("../Assets/L0E1.level"));

		}	
		else
			levelSelect->LoadLevel(levelPath);
		//Delete it. If it was successful it would have pushed a LevelState to the stack
		delete levelSelect;
		levelSelect = nullptr;
	}
	else
	{
		delete levelSelect;
		levelSelect = nullptr;
	}
#endif
	
	return result;

	////Create, Initialize and push a LevelSelectState
	//LevelSelectState* levelSelect = new LevelSelectState();
	//result = levelSelect->Initialize(this, cHandler);

	////If the initialization was successful
	//if (result > 0)
	//{
	//	//Push it to the gamestate stack/vector
	//	this->m_stateStack.push_back(levelSelect);
	//	
	//	levelSelect->LoadLevel(std::string("../ResourceLib/AssetFiles/TestingLevel.level"));
	//}
	//else
	//{
	//	//Delete it
	//	delete levelSelect;
	//	levelSelect = nullptr;
	//}
	//return result;


	////Create, Initialize and push a LevelState
	//LevelState* tempState = new LevelState();
	//result = tempState->Initialize(this, cHandler);
	////If the initialization was successful
	//if (result > 0)
	//{
	//	//Push it to the gamestate stack/vector
	//	this->m_stateStack.push_back(tempState);
	//}
	//else
	//{
	//	//Delete it
	//	delete tempState;
	//	tempState = nullptr;
	//}
	//return result;
}

int GameStateHandler::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;

	//Update the active state
	if (this->m_stateStack.size())
	{
		result = this->m_stateStack.back()->Update(dt, inputHandler);
	}
	//Delete the states
	while (this->m_statesToRemove.size())
	{
		GameState* temp;
		temp = this->m_statesToRemove.back();
		temp->ShutDown();
		delete temp;
		temp = nullptr;
		this->m_statesToRemove.pop_back();
	}
	return result;
}

int GameStateHandler::PushStateToStack(GameState * state)
{
	int result = 1;

	if (this->m_stateStack.size() > 0)
	{
		this->m_stateStack.back()->LeaveState();
	}
	this->m_stateStack.push_back(state);
	this->m_stateStack.back()->EnterState();

	return 1;
}

GameState * GameStateHandler::PopStateFromStack()
{
	GameState* result = nullptr;

	this->m_stateStack.back()->LeaveState();
	result = this->m_stateStack.back();

	this->m_stateStack.pop_back();

	//Check if it wants to be manually managed after popping
	if (!result->GetManualRemoval())
	{
		this->m_statesToRemove.push_back(result);
	}
	if (this->m_stateStack.size() > 0)
	{
		this->m_stateStack.back()->EnterState();
	}

	return result;
}
