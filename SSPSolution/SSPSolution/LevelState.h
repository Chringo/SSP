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
#include "FieldEntity.h"
#include "PlatformEntity.h"
//more than 10 segments can lead to chain segments going through walls. #ShitSebbeSays

#define CHAIN_SEGMENT_MODEL_ID 1680427216
//Per player
#define CHAIN_SEGMENTS 5
#define PLAYER1_ID 1
#define PLAYER2_ID 1
#define BALL1_ID 1
#define BALL2_ID 1
#define CHAIN_ID 1
const int GRAB_COOLDOWN = 1;	//Cooldown in seconds until player can grab something after a throw
const int GRAB_RANGE = 3; //The range of the grab in meters


// For testing
#define DEVELOPMENTFUNCTIONS

class LevelState :
	public GameState
{
private:

	FSMEnvironment::LevelDirector m_director;
	Player m_player1;
	Player m_player2;
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
	std::vector<FieldEntity*> m_fieldEntities;
	std::vector<PlatformEntity*> m_platformEntities;
	std::list<EntityPacket> m_entityPacketList;	//List with all updates for entities from the network
	std::list<StatePacket> m_statePacketList;	//List with all updates for entities from the network
	std::list<StateWheelPacket> m_wheelStatePacketList;	//List with all updates for entities from the network
	std::list<GrabPacket> m_grabPacketList;	//List with all updates for entities from the network
	std::list<AnimationPacket> m_animationPacketList;	//List with all animation updates from the network
	
	Entity* GetClosestBall(float minDist);

	int m_curLevel;
	int m_clearedLevel;
	std::vector<std::string> m_levelPaths;

	UIComponent* m_crosshair;
public:
	LevelState();
	virtual ~LevelState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);
	//Return means. 1:Success ; -1:Failed to load file ; -2:Failed to load level ; -3:Failed to load lights
	int CreateLevel(LevelData::Level* data);
	int UnloadLevel();
	//TEMP
	int LoadNext();

	int GetLevelIndex();
	std::string GetLevelPath();

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
private:
};

#endif