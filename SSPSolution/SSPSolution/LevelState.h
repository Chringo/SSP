#ifndef SSPAPPLICATION_GAMESTATES_LEVELSTATE_H
#define SSPAPPLICATION_GAMESTATES_LEVELSTATE_H
#include "GameState.h"
#include "Player.h"
#include "../SSP_Editor/LevelHeaders.h"
#include "../ResourceLib/ResourceHandler.h"
#include "StaticEntity.h"
class LevelState :
	public GameState
{
private:
	Player m_player1;
	std::vector<Entity*> m_entities;
public:
	LevelState();
	virtual ~LevelState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);
	int CreateLevel(LevelData::Level* data);
private:
};

#endif