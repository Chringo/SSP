#ifndef SSPAPPLICATION_GAMESTATES_LEVELSTATE_H
#define SSPAPPLICATION_GAMESTATES_LEVELSTATE_H
#include "GameState.h"
#include "Player.h"
class LevelState :
	public GameState
{
private:
	Player m_player1;
public:
	LevelState();
	virtual ~LevelState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* camera);
	int Update(float dt, InputHandler * inputHandler);
private:
};

#endif