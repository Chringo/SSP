#ifndef SSPAPPLICATION_GAMESTATES_LEVELSELECTSTATE_H
#define SSPAPPLICATION_GAMESTATES_LEVELSELECTSTATE_H
#include "GameState.h"
#include "LevelState.h"
#include "../ResourceLib/FileLoader.h"
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
	int LoadLevel(std::string path);

private:
};

#endif