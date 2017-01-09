#ifndef SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#define SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#include "InputHandler.h"
#include "ComponentHandler.h"
#include "LevelSelectState.h"
class GameStateHandler;
class GameState
{
private:	//Variables
protected:
	GameStateHandler* m_gsh;
	ComponentHandler* m_cHandler;

	int InitializeBase(GameStateHandler* gsh, ComponentHandler* cHandler);
public:
	GameState();
	virtual ~GameState();

	virtual int ShutDown() = 0;
	//Returns 1 for success and 0 for failure
	virtual int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler) = 0;

	virtual int Update(float dt, InputHandler * inputHandler) = 0;
private:	//Helper functions
};

#endif