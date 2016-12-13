#ifndef SSPAPPLICATION_GAMESTATES_STARTSTATE_H
#define SSPAPPLICATION_GAMESTATES_STARTSTATE_H
#include "GameState.h"
class StartState :
	public GameState
{
private:
public:
	StartState();
	virtual ~StartState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler);
	int Update(float dt, InputHandler * inputHandler);
private:
};

#endif