#ifndef SSPAPPLICATION_GAMESTATES_GAMESTATEHANDLER_H
#define SSPAPPLICATION_GAMESTATES_GAMESTATEHANDLER_H
#include "GameState.h"
#include "StartState.h"
#include "LevelSelectState.h"
#include <vector>

#define START_WITHOUT_MENU

class GameStateHandler
{
private:
	std::vector<GameState*> m_stateStack;
	std::vector<GameState*> m_statesToRemove;
public:
	GameStateHandler();
	~GameStateHandler();

	int ShutDown();

	int Initialize(ComponentHandler* cHandler, Camera* cameraRef, std::string levelPath = NULL);

	int Update(float dt, InputHandler* inputHandler);

	//Push a state to the stack
	int PushStateToStack(GameState* state);
private:
};

#endif