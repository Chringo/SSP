#ifndef SSPAPPLICATION_GAMESTATES_LEVELSELECTSTATE_H
#define SSPAPPLICATION_GAMESTATES_LEVELSELECTSTATE_H
//
#include "LevelState.h"
#include "../ResourceLib/FileLoader.h"
#include "../GraphicsDLL/LightHandler.h"
class LevelSelectState :
	public GameState
{
private:
	LevelState* m_currentLevel;
public:
	LevelSelectState();
	~LevelSelectState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);
	int LoadLevel(int levelID = 0);

	int EnterState();
	int LeaveState();

private:
};

#endif