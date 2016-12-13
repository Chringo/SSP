#ifndef SSPAPPLICATION_GAMESTATES_MENUSTATE_H
#define SSPAPPLICATION_GAMESTATES_MENUSTATE_H
#include "GameState.h"
class MenuState :
	public GameState
{
private:
public:
	MenuState();
	~MenuState();

	int Initialize(GameStateHandler* gsh);
	int Update(float dt, InputHandler * inputHandler);
private:
};

#endif