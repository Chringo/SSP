#ifndef SSPAPPLICATION_GAMESTATES_LEVELSELECTSTATE_H
#define SSPAPPLICATION_GAMESTATES_LEVELSELECTSTATE_H
#include "GameState.h"
class LevelSelectState :
	public GameState
{
private:
public:
	LevelSelectState();
	~LevelSelectState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* camera);
	int Update(float dt, InputHandler * inputHandler);

private:
};

#endif