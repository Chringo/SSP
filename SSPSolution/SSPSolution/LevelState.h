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
const int GRAB_RANGE = 3;		//The range of the grab in meters
const int PING_DISTANCE = 100;	//Distance for the ping ray


// For testing
#define DEVELOPMENTFUNCTIONS

class LevelState :
	public GameState
{
private:

	struct PingObject
	{
		GraphicsComponent* m_gComp;
		DirectX::XMFLOAT3 m_pos;
		float m_time = 0.f;
		float m_maxTime = 5.f;
		float m_animHeight = 0.f;
		bool m_dir = true;	//True = up, False = down

		void SetPos(DirectX::XMVECTOR newPos)
		{
			DirectX::XMStoreFloat3(&this->m_pos, newPos);
			this->m_gComp->worldMatrix = DirectX::XMMatrixTranslationFromVector(newPos);
		}

		void Update(float dt)
		{
			if (this->m_gComp->active)
			{
				this->m_time += dt;
				if (this->m_time < m_maxTime)
				{
					//printf("Showing the ping");
					//If we want to do soemthing while it is shown
					
					if (this->m_dir == true)	//Going up
					{
						this->m_animHeight += dt;

						if (this->m_animHeight >= 0.5f)
						{
							this->m_dir = false;
						}
					}
					else //Going down
					{
						this->m_animHeight -= dt;

						if (this->m_animHeight <= 0.f)
						{
							this->m_dir = true;
						}
					}

					//Update the position
					DirectX::XMVECTOR  newPos = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(this->m_pos.x, this->m_pos.y + this->m_animHeight, this->m_pos.z));
					this->m_gComp->worldMatrix = DirectX::XMMatrixTranslationFromVector(newPos);
				}
				else
				{
					this->m_gComp->active = false;
					this->m_time = 0;
					this->m_animHeight = 0;
					this->m_dir = true;
				}
			}

		};
	};
	struct GraphicalLink
	{
		GraphicsComponent* m_gComp;
		DirectX::XMFLOAT3 m_pos;
		DirectX::XMMATRIX m_rotMat;
		float xRot;
		
		void SetPos(DirectX::XMVECTOR newPos)
		{
			DirectX::XMStoreFloat3(&this->m_pos, newPos);
			this->m_gComp->worldMatrix = DirectX::XMMatrixTranslationFromVector(newPos);

			this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_rotMat, this->m_gComp->worldMatrix);
		}
		
		void SetRot(float yRot, float zRot)
		{
			
			this->m_rotMat = DirectX::XMMatrixRotationRollPitchYaw(this->xRot, yRot, zRot);

//			this->m_gComp->worldMatrix = DirectX::XMMatrixTranslationFromVector(newPos);

			this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_rotMat, this->m_gComp->worldMatrix);
		}
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
	std::list<PingPacket> m_pingPacketList;	//List with all Ping updates from the network

	std::vector<GraphicalLink> m_grapichalLinkListPlayer1;
	std::vector<GraphicalLink> m_grapichalLinkListPlayer2;
	std::vector<PhysicsComponent*> m_Player1ChainPhysicsComp;
	std::vector<PhysicsComponent*> m_Player2ChainPhysicsComp;

	Entity* GetClosestBall(float minDist);

	int m_curLevel;
	int m_clearedLevel;
	struct hardcodedLevelData {
		std::string levelPath;
		float farPlane;
	};
	std::vector<hardcodedLevelData> m_levelPaths;

	void SendSyncForJoin();

	UIComponent* m_controlsOverlay;

	UIComponent* m_crosshair;
	float delta_t;
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

	void SetCurrentLevelID(int currentLevelID);

	int EnterState();
	int LeaveState();

	void UpdateGraphicalLinks();

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
private:
	DirectX::XMVECTOR GetInterpolatedSplinePoint(float t, std::vector<PhysicsComponent*>*list);
	DirectX::XMVECTOR Equal(float t, DirectX::XMVECTOR p1, DirectX::XMVECTOR p2, DirectX::XMVECTOR p3, DirectX::XMVECTOR p4);
};

#endif