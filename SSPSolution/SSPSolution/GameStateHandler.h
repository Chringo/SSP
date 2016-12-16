#ifndef SSPAPPLICATION_GAMESTATES_GAMESTATEHANDLER_H
#define SSPAPPLICATION_GAMESTATES_GAMESTATEHANDLER_H
#include "GameState.h"
#include "LevelState.h"
#include <vector>
class GameStateHandler
{
private:
	std::vector<GameState*> m_stateStack;
	std::vector<GameState*> m_statesToRemove;
public:
	GameStateHandler();
	~GameStateHandler();

	int ShutDown();

	int Initialize(ComponentHandler* cHandler);

	int Update(float dt, InputHandler* inputHandler);
private:
};

#endif