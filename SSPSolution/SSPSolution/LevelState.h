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

class LevelState :
	public GameState
{
private:

	struct PingObject
	{
		GraphicsComponent* m_gComp;
		DirectX::XMFLOAT3 m_pos;
		float m_time = 0;
		float m_maxTime = 5;

		void Update(float dt)
		{
			if (this->m_gComp->active)
			{
				this->m_time += dt;
				if (this->m_time < m_maxTime)
				{
					//If we want to do soemthing while it is shown
				}
				else
				{
					this->m_gComp->active = false;
					this->m_time = 0;
				}
			}

		};
	};

	FSMEnvironment::LevelDirector m_director;
	Player m_player1;
	Player m_player2;
	DirectX::XMVECTOR m_player1_Spawn;
	DirectX::XMVECTOR m_player2_Spawn;
	PingObject m_player1_Ping;
	PingObject m_player2_Ping;

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

	int m_clearedLevel;
public:
	LevelState();
	virtual ~LevelState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);
	int CreateLevel(LevelData::Level* data);
	int UnloadLevel();
	//TEMP
	int LoadNext(InputHandler* inputHandler);

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
private:
};

#endif