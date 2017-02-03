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
	LevelState* currentLevel;
public:
	LevelSelectState();
	~LevelSelectState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);
	int LoadLevel(std::string path);

private:
};

#endif