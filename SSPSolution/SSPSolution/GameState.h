#ifndef SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#define SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#include "InputHandler.h"
class GameStateHandler;
class GameState
{
private:	//Variables
protected:
	GameStateHandler* m_gsh;
public:
	GameState();
	virtual ~GameState();
	//Returns 1 for success and 0 for failure
	virtual int Initialize(GameStateHandler* gsh) = 0;
	int InitializeBase(GameStateHandler* gsh);

	virtual int Update(float dt, InputHandler * inputHandler) = 0;
private:	//Helper functions
};

#endif