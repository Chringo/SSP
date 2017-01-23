#ifndef SSPAPPLICATION_GAMESTATES_LEVELSTATE_H
#define SSPAPPLICATION_GAMESTATES_LEVELSTATE_H
#include "GameState.h"
#include "Player.h"
#include "../SSP_Editor/LevelHeaders.h"
#include "../ResourceLib/ResourceHandler.h"
#include "StaticEntity.h"
#include "DynamicEntity.h"
#include "LevelDirector.h"
#include "DoorEntity.h"
#include "ButtonEntity.h"
#include "WheelEntity.h"

class LevelState :
	public GameState
{
private:
	FSMEnvironment::LevelDirector m_director;
	Player m_player1;
	std::vector<DynamicEntity*> m_dynamicEntitys;
	//Entities where no data needs to be moved between the components
	std::vector<StaticEntity*>	m_staticEntitys;
	//Puzzle entities
	std::vector<DoorEntity*> m_doorEntities;
	std::vector<ButtonEntity*> m_buttonEntities;
	std::vector<WheelEntity*> m_wheelEntities;
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