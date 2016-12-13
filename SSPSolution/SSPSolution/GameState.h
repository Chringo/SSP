#ifndef SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#define SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#include "InputHandler.h"
#include "ComponentHandler.h"
class GameStateHandler;
class GameState
{
private:	//Variables
protected:
	GameStateHandler* m_gsh;
	ComponentHandler* m_cHandler;
public:
	GameState();
	virtual ~GameState();
	//Returns 1 for success and 0 for failure
	virtual int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler) = 0;
	int InitializeBase(GameStateHandler* gsh, ComponentHandler* cHandler);

	virtual int Update(float dt, InputHandler * inputHandler) = 0;
private:	//Helper functions
};

#endif