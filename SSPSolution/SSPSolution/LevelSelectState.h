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

	int Initialize();

private:
};

#endif