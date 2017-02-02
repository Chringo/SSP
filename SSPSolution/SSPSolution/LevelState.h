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
#include "LeverEntity.h"

class LevelState :
	public GameState
{
private:
	struct Checkpoint
	{
		unsigned int index;
		DirectX::XMVECTOR pos;
		OBB obb;
	};
	FSMEnvironment::LevelDirector m_director;
	Field* directorTestField;
	Player m_player1;
	Player m_player2;
	int counter = 0;
	DirectX::XMVECTOR m_player1_Spawn;
	DirectX::XMVECTOR m_player2_Spawn;

	std::vector<DynamicEntity*> m_dynamicEntitys;
	//Entities where no data needs to be moved between the components
	std::vector<StaticEntity*>	m_staticEntitys;
	//Puzzle entities
	std::vector<DoorEntity*> m_doorEntities;
	std::vector<ButtonEntity*> m_buttonEntities;
	std::vector<WheelEntity*> m_wheelEntities;
	std::vector<LeverEntity*> m_leverEntities;
	std::vector<Checkpoint*> m_checkpoints;
	std::list<EntityPacket> m_entityPacketList;	//List with all updates for entities from the network
	std::list<StatePacket> m_statePacketList;	//List with all updates for entities from the network
	std::list<StateWheelPacket> m_wheelStatePacketList;	//List with all updates for entities from the network
	std::list<GrabPacket> m_grabPacketList;	//List with all updates for entities from the network
	
public:
	LevelState();
	virtual ~LevelState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);
	int CreateLevel(LevelData::Level* data);

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
private:
};

#endif