#ifndef SSPAPPLICATION_GAMESTATES_LEVELSELECTSTATE_H
#define SSPAPPLICATION_GAMESTATES_LEVELSELECTSTATE_H
#include "GameState.h"
#include "LevelState.h"
class LevelSelectState :
	public GameState
{
private:
	LevelState* currentLevel = nullptr;
public:
	LevelSelectState();
	~LevelSelectState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler);
	int Update(float dt, InputHandler * inputHandler);

private:
};

#endif