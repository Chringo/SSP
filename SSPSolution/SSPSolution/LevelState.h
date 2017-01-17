#ifndef SSPAPPLICATION_GAMESTATES_LEVELSTATE_H
#define SSPAPPLICATION_GAMESTATES_LEVELSTATE_H
#include "GameState.h"
#include "Player.h"
#include "../SSP_Editor/LevelHeaders.h"
#include "../ResourceLib/ResourceHandler.h"
#include "StaticEntity.h"
#include "DynamicEntity.h"
#include "LevelDirector.h"

class LevelState :
	public GameState
{
private:
	FSMEnvironment::LevelDirector m_director;
	Player m_player1;
	std::vector<DynamicEntity*> m_dynamicEntitys;
	std::vector<StaticEntity*>	m_staticEntitys;
	std::vector<Entity*> m_puzzleElements;
	int buttonCnt;
public:
	LevelState();
	virtual ~LevelState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);
	int CreateLevel(LevelData::Level* data);

	void LockCameraToPlayer();
private:
};

#endif