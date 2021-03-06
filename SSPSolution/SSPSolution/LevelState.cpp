#include "LevelState.h"
#include "CreditState.h"
#include "GameStateHandler.h"

inline OBB m_ConvertOBB(BoundingBoxHeader & boundingBox) //Convert from BBheader to OBB struct										
{
	OBB obj;

	obj.ext[0] = boundingBox.extension[0];
	obj.ext[1] = boundingBox.extension[1];
	obj.ext[2] = boundingBox.extension[2];


	DirectX::XMMATRIX extensionMatrix;
	extensionMatrix = DirectX::XMMatrixSet(
		boundingBox.extensionDir[0].x, boundingBox.extensionDir[0].y, boundingBox.extensionDir[0].z, 0.0f,
		boundingBox.extensionDir[1].x, boundingBox.extensionDir[1].y, boundingBox.extensionDir[1].z, 0.0f,
		boundingBox.extensionDir[2].x, boundingBox.extensionDir[2].y, boundingBox.extensionDir[2].z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	obj.ort = extensionMatrix;


	return obj;
}

Entity* LevelState::GetClosestBall(float minDist)
{
	Entity* closest = nullptr;
	float closestDistance = FLT_MAX;
	PhysicsComponent* pc = this->m_player1.GetPhysicsComponent();
	//Calc the distance for play1 ball;
	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(pc->PC_pos, this->m_player2.GetBall()->GetPhysicsComponent()->PC_pos)));
	float distance1 = FLT_MAX;
	if (!this->m_player2.GetBall()->IsGrabbed())
	{
		//If distance is less than hald the pickup distance, pickup player2's ball every time
		if (distance < minDist / 1.8f)
		{
			closest = this->m_player2.GetBall();
		}
	}
	if (closest == nullptr)
	{
		//Calc the distance for play1 ball;
		DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(pc->PC_pos, this->m_player1.GetBall()->GetPhysicsComponent()->PC_pos);
		float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vec));

		if (this->m_player1.GetBall()->IsGrabbed() == false && distance <= minDist)	//Is not grabbed and close enoughe
		{
			closest = this->m_player1.GetBall();
			closestDistance = distance;

			vec = DirectX::XMVectorSubtract(pc->PC_pos, this->m_player2.GetBall()->GetPhysicsComponent()->PC_pos);
			distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vec));

			if (this->m_player2.GetBall()->IsGrabbed() == false && distance < closestDistance)	//Is not grabbed and Closer
			{
				closest = this->m_player2.GetBall();
			}
		}
		else //If ball1 is already grabbed
		{
			vec = DirectX::XMVectorSubtract(pc->PC_pos, this->m_player2.GetBall()->GetPhysicsComponent()->PC_pos);
			distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vec));

			if (this->m_player2.GetBall()->IsGrabbed() == false && distance <= minDist)	//Is not grabbed and close enoughe
			{
				closest = this->m_player2.GetBall();
			}
		}
		//Some crazy Kim stuff. probably doesn't work
		//if ((1.0f / distance) * this->m_player2.GetBall()->IsGrabbed() < (1.0f / DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(pc->PC_pos, this->m_player1.GetBall()->GetPhysicsComponent()->PC_pos)))) * this->m_player1.GetBall()->IsGrabbed())
		//{
		//	//Player1's ball is closest 
		//}
		//else
		//{
		//	//Player2's ball is closest
		//}
	}


	return closest;
}

void LevelState::SendSyncForJoin()
{
	unsigned int	startIndex = 0;
	unsigned int	nrOfDynamics = 0;
	bool			isHost = false;
	unsigned int	levelID = this->m_curLevel;	//CHANGE LEVEL HERE NOW
	unsigned int	checkpointID = 0;

	this->m_networkModule->SendPhysicSyncPacket(startIndex, nrOfDynamics, isHost, levelID, checkpointID);

	//Update puzzle elements
	//Check for state changes that should be sent over the network
	for (LeverEntity* e : this->m_leverEntities)
	{
		this->m_networkModule->SendStateLeverPacket(e->GetEntityID(), e->GetIsActive());
	}

	for (ButtonEntity* e : this->m_buttonEntities)
	{
		this->m_networkModule->SendStateButtonPacket(e->GetEntityID(), e->GetIsActive());
	}

	WheelSyncState* wheelSync = nullptr;
	for (WheelEntity* e : this->m_wheelEntities)
	{
		wheelSync = e->GetUnconditionalState();
		if (wheelSync != nullptr)
		{
			this->m_networkModule->SendStateWheelPacket(wheelSync->entityID, wheelSync->rotationState, wheelSync->rotationAmount);
			delete wheelSync;
		}
	}

	PhysicsComponent* pc = nullptr;
	for (PlatformEntity* p : this->m_platformEntities)
	{
		pc = p->GetPhysicsComponent();
		DirectX::XMFLOAT4X4 newrot;
		DirectX::XMStoreFloat4x4(&newrot, pc->PC_OBB.ort);
		this->m_networkModule->SendEntityUpdatePacket(pc->PC_entityID, pc->PC_pos, pc->PC_velocity, newrot);
	}

	//Drop anything we are holding
	this->m_networkModule->SendGrabPacket(this->m_player2.GetEntityID(), -1);

	if (this->m_player1.GetBall()->IsGrabbed())
	{
		this->m_networkModule->SendGrabPacket(this->m_player1.GetBall()->GetISGrabbedBy()->GetEntityID(), this->m_player1.GetBall()->GetEntityID());
}
	else if (this->m_player2.GetBall()->IsGrabbed())
	{
		this->m_networkModule->SendGrabPacket(this->m_player2.GetBall()->GetISGrabbedBy()->GetEntityID(), this->m_player2.GetBall()->GetEntityID());
	}

	AnimationComponent* ap = this->m_player1.GetAnimationComponent();
	if (this->m_player1.GetRagdoll()->state == RagdollState::RAGDOLL || this->m_player1.GetRagdoll()->state == RagdollState::KEYFRAMEBLEND)
	{
		GraphicsAnimationComponent* gp = (GraphicsAnimationComponent*)this->m_player1.GetGraphicComponent();

		for (int i = 0; i < gp->jointCount; i++)	//Iterate all joints
		{
			//Send a packet for E V E R Y joint
			this->m_networkModule->SendAnimationPacket(this->m_player1.GetEntityID(), RAGDOLL_STATE, 0.f, Blending::NO_TRANSITION, false, false, 0.f, 1.0, i, gp->finalJointTransforms[i]);
		}
	}
	else
	{
		bool loop = false;
		if (ap->currentState == PLAYER_IDLE || ap->currentState == PLAYER_BALL_IDLE)
		{
			loop = true;
		}

		this->m_networkModule->SendAnimationPacket(this->m_player1.GetEntityID(), ap->currentState, ap->transitionDuration, ap->blendFlag, loop, ap->lockAnimation, ap->playingSpeed, ap->velocity, 0, DirectX::XMMATRIX());
	}
}

LevelState::LevelState()
{
}

LevelState::~LevelState()
{
}

int LevelState::ShutDown()
{
	int result = 1;
	this->UnloadLevel();
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 1.4f, 0.0f, 0.0f);
	//this->m_dynamicEntitys
	//Get the Camera Pivot and delete it before supplimenting our own

	this->m_cameraRef->SetCameraPivot(nullptr, targetOffset, 1.3f);
	// Clear the dynamic entities
	for (size_t i = 0; i < this->m_dynamicEntitys.size(); i++)
	{
		delete this->m_dynamicEntitys[i];
		this->m_dynamicEntitys[i] = nullptr;
	}

	// Clear the static entities
	for (size_t i = 0; i < this->m_staticEntitys.size(); i++)
	{
		delete this->m_staticEntitys[i];
		this->m_staticEntitys[i] = nullptr;
	}

	//Clear the puzzle entities
	for (size_t i = 0; i < this->m_doorEntities.size(); i++)
	{
		delete this->m_doorEntities[i];
		this->m_doorEntities[i] = nullptr;
	}
	this->m_doorEntities.clear();
	for (size_t i = 0; i < this->m_buttonEntities.size(); i++)
	{
		delete this->m_buttonEntities[i];
		this->m_buttonEntities[i] = nullptr;
	}
	this->m_buttonEntities.clear();
	for (size_t i = 0; i < this->m_leverEntities.size(); i++)
	{
		delete this->m_leverEntities[i];
		this->m_leverEntities[i] = nullptr;
	}
	this->m_leverEntities.clear();
	for (size_t i = 0; i < this->m_wheelEntities.size(); i++)
	{
		delete this->m_wheelEntities[i];
		this->m_wheelEntities[i] = nullptr;
	}
	this->m_wheelEntities.clear();
	for (size_t i = 0; i < this->m_fieldEntities.size(); i++)
	{
		delete this->m_fieldEntities[i];
		this->m_fieldEntities[i] = nullptr;
	}
	this->m_fieldEntities.clear();

	for (size_t i = 0; i < this->m_platformEntities.size(); i++)
	{
		delete this->m_platformEntities[i];
		this->m_platformEntities[i] = nullptr;
	}
	this->m_platformEntities.clear();
	// Clear level director
	this->m_director.Shutdown();

	this->m_cHandler->ResizeGraphicsDynamic(0);
	this->m_cHandler->ResizeGraphicsStatic(0);
	this->m_cHandler->ResizeGraphicsPersistent(0);
	this->m_cHandler->ClearAminationComponents();

	//We need to add a function which empties the physics and bullet.
	this->m_cHandler->GetPhysicsHandler()->ShutDown();
	this->m_cHandler->GetPhysicsHandler()->Initialize();

	//this->m_cHandler->RemoveUIComponentFromPtr(this->m_controlsOverlay);
	//this->m_cHandler->RemoveUIComponentFromPtr(this->m_crosshair);

	this->m_Player1ChainPhysicsComp.clear();
	this->m_Player2ChainPhysicsComp.clear();
	this->m_grapichalLinkListPlayer1.clear();
	this->m_grapichalLinkListPlayer2.clear();

	return result;
}

int LevelState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 1;
	result = GameState::InitializeBase(gsh, cHandler, cameraRef, false);

	this->m_clearedLevel = 0;
	this->m_curLevel = 0;

	//this->m_levelPaths.push_back({ "../ResourceLib/AssetFiles/TutorialLevel.level", 68.0f });
	//this->m_levelPaths.push_back({ "../ResourceLib/AssetFiles/L1P1.level", 46.0f });
	//this->m_levelPaths.push_back({ "../ResourceLib/AssetFiles/L1P2.level", 46.0f });
	////this->m_levelPaths.push_back({ "../ResourceLib/AssetFiles/L1P2.level", 46.0f });
	////this->m_levelPaths.push_back({ "../ResourceLib/AssetFiles/L1P2.level", 46.0f });
	//this->m_levelPaths.push_back({ "../ResourceLib/AssetFiles/L2P1.level", 41.0f });
	//this->m_levelPaths.push_back({ "../ResourceLib/AssetFiles/L3P1.level", 41.0f });
	//this->m_levelPaths.push_back({"../ResourceLib/AssetFiles/L4P1.level", 41.0f });
	//this->m_levelPaths.push_back({"../ResourceLib/AssetFiles/L5P1.level", 40.0f });

	//For installer
	this->m_levelPaths.push_back({ "../Assets/L0E1.level", 68.0f });
	this->m_levelPaths.push_back({ "../Assets/L1E1.level", 46.0f });
	this->m_levelPaths.push_back({ "../Assets/L2E1.level", 46.0f });
	this->m_levelPaths.push_back({ "../Assets/L3E1.level", 41.0f });
	this->m_levelPaths.push_back({ "../Assets/L4E1.level", 41.0f });
	this->m_levelPaths.push_back({ "../Assets/L5E1.level", 41.0f });
	//this->m_levelPaths.push_back({ "../Assets/L6E1.level", 40.0f });



	//this->m_levelPaths.push_back({"../ResourceLib/AssetFiles/L4P1.level, 46.0f}");
	//this->m_levelPaths.push_back({"../ResourceLib/AssetFiles/L5P1.level, 46.0f}");
	//this->m_levelPaths.push_back({"../ResourceLib/AssetFiles/L6P1.level, 46.0f}");
	//this->m_levelPaths.push_back({"../ResourceLib/AssetFiles/L1P2.level, 46.0f}");

	if (this->m_curLevel > this->m_levelPaths.size())
	{
		this->m_curLevel = 0;
	}
	int nrCPs = 22;//(estLen / );

	Resources::ResourceHandler* resHandler = Resources::ResourceHandler::GetInstance();
	this->m_cHandler->GetGraphicsHandler()->ResizeDynamicComponents(2);
	float nrOfSegmentsPerPlayer = 5; //more than 10 segments can lead to chain segments going through walls

	this->m_cHandler->ResizeGraphicsPersistent(2 + 2 + (nrCPs * 2));	//"2 balls + 2 PingObjects + number of graphicallinks for both chains

	//Creating the players

	//Player 1
#pragma region
	this->m_player1 = Player();
	GraphicsComponent* playerG = m_cHandler->GetGraphicsAnimationComponent();
	
	if (this->m_networkModule->IsHost())
	{
		playerG->modelID = 885141774;
	}
	else
	{
		playerG->modelID = 1117267500;
	}
	
	playerG->active = true;
	resHandler->GetModel(playerG->modelID, playerG->modelPtr);
	PhysicsComponent* playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = DEFINED_IDS::PLAYER_1;				//Set Entity ID
	playerP->PC_pos = DirectX::XMVectorSet(0, 2, 0, 0);			//Set Position
	playerP->PC_rotation = DirectX::XMVectorSet(0, 0.0, 0, 0);	//Set Rotation
	playerP->PC_is_Static = false;								//Set IsStatic
	playerP->PC_mass = 10;
	playerP->PC_BVtype = BV_OBB;
	playerP->PC_OBB.ext[0] = playerG->modelPtr->GetOBBData().extension[0];
	playerP->PC_OBB.ext[1] = playerG->modelPtr->GetOBBData().extension[1];
	playerP->PC_OBB.ext[2] = playerG->modelPtr->GetOBBData().extension[2];
	playerP->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	playerP->PC_friction = 0.95f;
	playerG->worldMatrix = DirectX::XMMatrixIdentity();		//FIX THIS


#pragma region
	AnimationComponent* playerAnim1 = nullptr;

	if (playerG->modelPtr->GetSkeleton() != nullptr)
	{
		playerAnim1 = m_cHandler->GetAnimationComponent();

		playerAnim1->skeleton = playerG->modelPtr->GetSkeleton();

		((GraphicsAnimationComponent*)playerG)->jointCount = playerG->modelPtr->GetSkeleton()->GetSkeletonData()->jointCount;

		playerAnim1->active = 1;

		for (int i = 0; i < ((GraphicsAnimationComponent*)playerG)->jointCount; i++)
		{
			((GraphicsAnimationComponent*)playerG)->finalJointTransforms[i] = DirectX::XMMatrixIdentity();
		}

		if (playerG->modelPtr->GetSkeleton()->GetNumAnimations() > 0)
		{
			int numAnimations = playerG->modelPtr->GetSkeleton()->GetNumAnimations();

			playerAnim1->animation_States = playerG->modelPtr->GetSkeleton()->GetAllAnimations();

			playerAnim1->source_State = playerAnim1->animation_States->at(0)->GetAnimationStateData();
			playerAnim1->source_State->isLooping = true;
			playerAnim1->blendFlag = Blending::NO_TRANSITION;
			playerAnim1->playingSpeed = 2.0f;
		}
	}
#pragma endregion Animation_Player1

	this->m_player1.Initialize(playerP->PC_entityID, playerP, playerG, playerAnim1);
	this->m_player1.SetMaxSpeed(30.0f);
	this->m_player1.SetAcceleration(5.0f);
	this->m_player1.SetRagdoll(this->m_cHandler->GetPhysicsHandler()->GetPlayer1Ragdoll());

#pragma endregion Player1

	//Player 2
#pragma region
	this->m_player2 = Player();
	playerG = m_cHandler->GetGraphicsAnimationComponent();
	
	if (this->m_networkModule->IsHost())
	{
		playerG->modelID = 1117267500;
	}
	else
	{
		playerG->modelID = 885141774;
	}
	
	playerG->active = true;
	resHandler->GetModel(playerG->modelID, playerG->modelPtr);
	playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = DEFINED_IDS::PLAYER_2;			//Set Entity ID
	playerP->PC_pos = { 0 };								//Set Position
	playerP->PC_is_Static = false;							//Set IsStatic
	playerP->PC_active = true;								//Set Active
	playerP->PC_mass = 10;
	playerP->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	playerP->PC_BVtype = BV_OBB;
	playerP->PC_OBB.ext[0] = playerG->modelPtr->GetOBBData().extension[0];
	playerP->PC_OBB.ext[1] = playerG->modelPtr->GetOBBData().extension[1];
	playerP->PC_OBB.ext[2] = playerG->modelPtr->GetOBBData().extension[2];
	playerP->PC_friction = 0.95f;
	playerG->worldMatrix = DirectX::XMMatrixIdentity();		//FIX THIS

#pragma region
	AnimationComponent* playerAnim2 = nullptr;

	if (playerG->modelPtr->GetSkeleton() != nullptr)
	{
		playerAnim2 = m_cHandler->GetAnimationComponent();

		playerAnim2->skeleton = playerG->modelPtr->GetSkeleton();

		((GraphicsAnimationComponent*)playerG)->jointCount = playerG->modelPtr->GetSkeleton()->GetSkeletonData()->jointCount;

		playerAnim2->active = 1;

		for (int i = 0; i < ((GraphicsAnimationComponent*)playerG)->jointCount; i++)
		{
			((GraphicsAnimationComponent*)playerG)->finalJointTransforms[i] = DirectX::XMMatrixIdentity();
		}

		if (playerG->modelPtr->GetSkeleton()->GetNumAnimations() > 0)
		{
			int numAnimations = playerG->modelPtr->GetSkeleton()->GetNumAnimations();

			playerAnim2->animation_States = playerG->modelPtr->GetSkeleton()->GetAllAnimations();

			playerAnim2->source_State = playerAnim2->animation_States->at(0)->GetAnimationStateData();
			playerAnim2->source_State->isLooping = true;
			playerAnim2->blendFlag = Blending::NO_TRANSITION;
			playerAnim2->playingSpeed = 2.0f;
		}
	}
#pragma endregion Animation_Player2

	this->m_player2.Initialize(playerP->PC_entityID, playerP, playerG, playerAnim2);
	this->m_player2.SetMaxSpeed(30.0f);
	this->m_player2.SetAcceleration(5.0f);
	this->m_player2.SetRagdoll(this->m_cHandler->GetPhysicsHandler()->GetPlayer2Ragdoll());

#pragma endregion Player2

#pragma region
	////Ball1
	DynamicEntity* ball = new DynamicEntity();
	GraphicsComponent* ballG = m_cHandler->GetPersistentGraphicsComponent();
	if (this->m_networkModule->IsHost())
	{
	ballG->modelID = 1256673809;
	}
	else
	{
		ballG->modelID = 1321651915;
	}
	ballG->active = true;
	resHandler->GetModel(ballG->modelID, ballG->modelPtr);
	PhysicsComponent* ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = DEFINED_IDS::BALL_1;				//Set Entity ID
	ballP->PC_pos = { 0 };									//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	ballP->PC_rotationVelocity = DirectX::XMVectorSet(0, 0, 0, 0);
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_Sphere;
	ballP->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	ballP->PC_OBB.ext[0] = 0.5f;
	ballP->PC_OBB.ext[1] = 0.5f;
	ballP->PC_OBB.ext[2] = 0.5f;
	ballP->PC_Sphere.radius = 0.25;
	ballP->PC_friction = 0.5f;
	//ballP->PC_Sphere.radius = 1;


	ballP->PC_mass = 25;
	ballG->worldMatrix = DirectX::XMMatrixIdentity();
	ball->Initialize(3, ballP, ballG);
	this->m_dynamicEntitys.push_back(ball);
	m_player1.SetBall(ball);
#pragma endregion Ball1

#pragma region
	////Ball2
	DynamicEntity* ball2 = new DynamicEntity();
	ballG = m_cHandler->GetPersistentGraphicsComponent();
	if (this->m_networkModule->IsHost())
	{
	ballG->modelID = 1321651915;
	}
	else
	{
		ballG->modelID = 1256673809;
	}
	ballG->active = true;
	resHandler->GetModel(ballG->modelID, ballG->modelPtr);
	ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = DEFINED_IDS::BALL_2;				//Set Entity ID
	ballP->PC_pos = { 0 };									//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_Sphere;

	ballP->PC_Sphere.radius = 0.25;
	ballP->PC_friction = 0.5f;

	ballP->PC_mass = 25;
	ballG->worldMatrix = DirectX::XMMatrixIdentity();
	ball2->Initialize(4, ballP, ballG);
	this->m_dynamicEntitys.push_back(ball2);
	m_player2.SetBall(ball2);
#pragma endregion Ball2

#pragma region
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 1.4f, 0.0f, 0.0f);
	//this->m_dynamicEntitys
	//Get the Camera Pivot and delete it before supplimenting our own
	
	this->m_cameraRef->SetCameraPivot(
		&this->m_cHandler->GetPhysicsHandler()->GetComponentAt(0)->PC_pos,
		targetOffset,
		1.3f
	);
#pragma endregion Set_Camera

	for (size_t i = 0; i < nrCPs; i++) // player 1 
	{
		GraphicsComponent * cp = cHandler->GetPersistentGraphicsComponent();
		cp->modelID = CHAIN_SEGMENT_MODEL_ID;
		cp->active = true;
		cp->worldMatrix = DirectX::XMMatrixIdentity();
		resHandler->GetModel(cp->modelID, cp->modelPtr);
		GraphicalLink link;
		link.m_gComp = cp;
		link.m_pos = { 0,0,0 };
		link.xRot = 45.f * i;
		link.m_rotMat = DirectX::XMMatrixIdentity();
		link.SetRot(0.f, 0.f);
		this->m_grapichalLinkListPlayer1.push_back(link);

	}
	for (size_t i = 0; i < nrCPs; i++) // Player 2 
	{
		GraphicsComponent * cp = cHandler->GetPersistentGraphicsComponent();
		cp->modelID = CHAIN_SEGMENT_MODEL_ID;
		cp->active = true;
		cp->worldMatrix = DirectX::XMMatrixIdentity();
		resHandler->GetModel(cp->modelID, cp->modelPtr);
		GraphicalLink link;
		link.m_gComp = cp;
		link.m_pos = { 0,0,0 };
		link.xRot = 45.f * i;
		link.m_rotMat = DirectX::XMMatrixIdentity();
		link.SetRot(0.f, 0.f);
		this->m_grapichalLinkListPlayer2.push_back(link);

	}

#pragma region
	this->m_player1_Ping.m_gComp = cHandler->GetPersistentGraphicsComponent();
	this->m_player1_Ping.m_gComp->modelID = 2539810394;
	this->m_player1_Ping.m_gComp->active = true;
	this->m_player1_Ping.m_gComp->worldMatrix = DirectX::XMMatrixIdentity();
	resHandler->GetModel(this->m_player1_Ping.m_gComp->modelID, this->m_player1_Ping.m_gComp->modelPtr);
	this->m_player1_Ping.m_pos = { 0, 0, 0 };

	this->m_player2_Ping.m_gComp = cHandler->GetPersistentGraphicsComponent();
	this->m_player2_Ping.m_gComp->modelID = 2539810394;
	this->m_player2_Ping.m_gComp->active = true;
	this->m_player1_Ping.m_gComp->worldMatrix = DirectX::XMMatrixIdentity();
	resHandler->GetModel(this->m_player2_Ping.m_gComp->modelID, this->m_player2_Ping.m_gComp->modelPtr);
	this->m_player2_Ping.m_pos = { 0, 0, 0 };

	//Set them to inactive in the begining
	this->m_player1_Ping.m_gComp->active = false;
	this->m_player2_Ping.m_gComp->active = false;


#pragma endregion PingModels

	this->m_director.Initialize();

	//Controls overlay
	this->m_controlsOverlay = cHandler->GetUIComponent();
	this->m_controlsOverlay->active = 0;
	this->m_controlsOverlay->position = DirectX::XMFLOAT2(0.f, 0.f);
	this->m_controlsOverlay->spriteID = Textures::Keymaps;
	this->m_controlsOverlay->scale = .6f;

	//Crosshair overlay
	this->m_crosshair = cHandler->GetUIComponent();
	this->m_crosshair->active = false;
	this->m_crosshair->position = DirectX::XMFLOAT2(608.f, 328.f);
	this->m_crosshair->spriteID = Textures::CrosshairAim;
	this->m_crosshair->scale = 0.8f;

	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{

	int result = 1;
	dt = dt / 1000000;

	if (this->m_clearedLevel == 1)
	{
		this->LoadNext();
	}
	this->UpdateGraphicalLinks();

	int prevConnects = (int)this->m_networkModule->GetNrOfConnectedClients();
	this->m_networkModule->Update();

	//If someone has connected
	if (prevConnects < this->m_networkModule->GetNrOfConnectedClients())
	{
		this->SendSyncForJoin();
	}

	this->m_cameraRef->UpdateDeltaTime(dt);

#pragma region 
	if (this->m_networkModule->GetNrOfConnectedClients() != 0)	//Check so we are connected to a client
	{
		//Check for updates for enteties
		this->m_entityPacketList = this->m_networkModule->PacketBuffer_GetEntityPackets();	//This removes the entity packets from the list in NetworkModule

		if (this->m_entityPacketList.size() > 0)
		{

			// Apply each packet to the right entity
			std::list<EntityPacket>::iterator itr;
			PhysicsComponent* pp = nullptr;
			for (itr = this->m_entityPacketList.begin(); itr != this->m_entityPacketList.end(); itr++)
			{
				/*
				Every packet that we recived with the entityID 1 will be sent to player2 object since
				we know that on the other computer will also play on his/her local player1 object.
				This way we know that all packets with ID 1 is sent for the "self" player object (m_player2) and Id 2
				for out local "self" player object (m_player1).
				To compensate for this we will have to switch places for m_player1 and m_player2 position on the
				connecting player so they still have the same start position relative to eachother.
				*/

				if ((int)itr->entityID == DEFINED_IDS::PLAYER_1)	//Packets for player2
				{
					pp = this->m_player2.GetPhysicsComponent();

					// Update the component
					pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
					//this->m_player2.GetRagdoll()->upperBody.center->PC_pos = pp->PC_pos;
					pp->PC_OBB.ort = DirectX::XMLoadFloat4x4(&itr->newRotation);
					pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);

				}
				else if ((int)itr->entityID == DEFINED_IDS::PLAYER_2)	//Packets for player1
				{
					pp = this->m_player1.GetPhysicsComponent();

					// Update the component
					pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
					//this->m_player1.GetRagdoll()->upperBody.center->PC_pos = pp->PC_pos;
					pp->PC_OBB.ort = DirectX::XMLoadFloat4x4(&itr->newRotation);
					pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);
				}
				else if ((int)itr->entityID == DEFINED_IDS::BALL_1)	//Packets for ball1
				{
					pp = this->m_player2.GetBall()->GetPhysicsComponent();

					// Update the component
					pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
					pp->PC_OBB.ort = DirectX::XMLoadFloat4x4(&itr->newRotation);
					pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);
				}
				else if ((int)itr->entityID == DEFINED_IDS::BALL_2)	//Packets for ball2
				{
					pp = this->m_player1.GetBall()->GetPhysicsComponent();

					//if the opposite player is holding the current players ball, the syncing will not be done
					// Update the component
					pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
					pp->PC_OBB.ort = DirectX::XMLoadFloat4x4(&itr->newRotation);
					pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);

				}
				else //For every other entity
				{
					// Find the entity
					std::vector<DynamicEntity*>::iterator Ditr;
					for (Ditr = this->m_dynamicEntitys.begin(); Ditr != this->m_dynamicEntitys.end(); Ditr++)
					{

						if (itr->entityID == (*Ditr._Ptr)->GetEntityID())
						{
							DynamicEntity* ent = (*Ditr._Ptr);	// The entity identified by the ID sent from the other client
							pp = ent->GetPhysicsComponent();

							// Update the component
							pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
							pp->PC_OBB.ort = DirectX::XMLoadFloat4x4(&itr->newRotation);
							pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);
							break;
						}

					}

					//If we still havent found an entity check for platforms
					std::vector<PlatformEntity*>::iterator Pitr;
					for (Pitr = this->m_platformEntities.begin(); Pitr != this->m_platformEntities.end(); Pitr++)
					{

						if (itr->entityID == (*Pitr._Ptr)->GetEntityID())
						{
							PlatformEntity* plat = (*Pitr._Ptr);	// The entity identified by the ID sent from the other client
							pp = plat->GetPhysicsComponent();

							// Update the component
							pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
							pp->PC_OBB.ort = DirectX::XMLoadFloat4x4(&itr->newRotation);
							pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);

							plat->GetAIComponent()->AC_position = pp->PC_pos;
							break;
						}

					}

				}
			}
		}
		this->m_entityPacketList.clear();	//Clear the list
	}
#pragma endregion Network_update_entities

#pragma region

	if (this->m_networkModule->GetNrOfConnectedClients() != 0)
	{

		// LEVERS AND BUTTONS //
		this->m_statePacketList = this->m_networkModule->PacketBuffer_GetStatePackets();	//This removes the entity packets from the list in NetworkModule

		if (this->m_statePacketList.size() > 0)
		{
			// Apply each packet to the right entity
			std::list<StatePacket>::iterator itr;

			for (itr = this->m_statePacketList.begin(); itr != this->m_statePacketList.end(); itr++)
			{

				if (itr->packet_type == UPDATE_BUTTON_STATE)
				{
					for (size_t i = 0; i < this->m_buttonEntities.size(); i++)
					{
						ButtonEntity* bP = this->m_buttonEntities.at(i);
						if (bP->GetEntityID() == itr->entityID)
						{
							ButtonSyncState newState;
							newState.entityID = itr->entityID;
							newState.isActive = itr->isActive;

							bP->SetSyncState(&newState);

							break;
						}

					}

				}
				else if (itr->packet_type == UPDATE_LEVER_STATE)
				{
					for (size_t i = 0; i < this->m_leverEntities.size(); i++)
					{
						LeverEntity* lP = this->m_leverEntities.at(i);
						if (lP->GetEntityID() == itr->entityID)
						{
							LeverSyncState newState;
							newState.entityID = itr->entityID;
							newState.isActive = itr->isActive;

							lP->SetSyncState(&newState);

							break;
						}
					}
				}
			}
		}
		this->m_statePacketList.clear();
		// LEVERS AND BUTTONS END//

		// WHEELS //
		this->m_wheelStatePacketList = this->m_networkModule->PacketBuffer_GetWheelStatePackets();	//This removes the entity packets from the list in NetworkModule

		if (this->m_wheelStatePacketList.size() > 0)
		{
			// Apply each packet to the right entity
			std::list<StateWheelPacket>::iterator itr;

			for (itr = this->m_wheelStatePacketList.begin(); itr != this->m_wheelStatePacketList.end(); itr++)
			{

				for (size_t i = 0; i < this->m_wheelEntities.size(); i++)
				{
					WheelEntity* wP = this->m_wheelEntities.at(i);

					if (wP->GetEntityID() == itr->entityID)
					{
						WheelSyncState newState;
						newState.entityID = itr->entityID;
						newState.rotationState = itr->rotationState;
						newState.rotationAmount = itr->rotationAmount;

						wP->SetSyncState(&newState);

						break;
					}
				}
			}
		}
		this->m_wheelStatePacketList.clear();
		// WHEELS END //
	}

#pragma endregion Network_update_States

#pragma region
	if (this->m_networkModule->GetNrOfConnectedClients() != 0)
	{
		this->m_animationPacketList = this->m_networkModule->PacketBuffer_GetAnimationPackets();

		if (this->m_animationPacketList.size() > 0)
		{
			std::list<AnimationPacket>::iterator itr;
			for (itr = this->m_animationPacketList.begin(); itr != this->m_animationPacketList.end(); itr++)
			{
				/* We know that all packets will be sent to player2
				since only player2 will send animation packets */

				if (itr->newstate == AnimationStates::RAGDOLL_STATE)	//If the packet is for a ragdoll state
				{
					this->m_player2.GetRagdoll()->state = RagdollState::RAGDOLL;
					GraphicsAnimationComponent* gp = (GraphicsAnimationComponent*)this->m_player2.GetGraphicComponent();
					gp->finalJointTransforms[itr->jointIndex] = DirectX::XMLoadFloat4x4(&itr->finalJointTransform);
					this->m_player2.GetAnimationComponent()->previousState = this->m_player2.GetAnimationComponent()->currentState;
					this->m_player2.GetAnimationComponent()->currentState = itr->newstate;
					this->m_player2.SetAnimationComponent(AnimationStates::RAGDOLL_STATE, 0.f, Blending::NO_TRANSITION, false, false, 0.f, 1.0);
					this->m_player2.GetAnimationComponent()->source_State->stateIndex = AnimationStates::RAGDOLL_STATE;

				}
				else
				{
					if (this->m_player2.GetRagdoll()->state == RagdollState::RAGDOLL)
					{

						this->m_player2.GetAnimationComponent()->previousState = this->m_player2.GetAnimationComponent()->currentState;
						this->m_player2.SetAnimationComponent(AnimationStates::PLAYER_RISE_UP, 0, Blending::NO_TRANSITION, false, true, 2.0f, 1.0f);
						this->m_player2.GetAnimationComponent()->currentState = AnimationStates::PLAYER_RISE_UP;
						this->m_player2.GetAnimationComponent()->source_State = this->m_player2.GetAnimationComponent()->animation_States->at(AnimationStates::PLAYER_RISE_UP)->GetAnimationStateData();
						this->m_player2.GetAnimationComponent()->source_State->stateIndex = AnimationStates::PLAYER_RISE_UP;
					}

					this->m_player2.GetRagdoll()->state = RagdollState::ANIMATED;
					this->m_player2.GetAnimationComponent()->previousState = this->m_player2.GetAnimationComponent()->currentState;
					this->m_player2.SetAnimationComponent(itr->newstate, itr->transitionDuritation, (Blending)itr->blendingType, itr->isLooping, itr->lockAnimation, itr->playingSpeed, itr->velocity);
					this->m_player2.GetAnimationComponent()->currentState = itr->newstate;
				}
			}

		}
		this->m_animationPacketList.clear();

	}
#pragma endregion Update_Animations

#pragma region
	float yaw = inputHandler->GetMouseDelta().x;
	float pitch = inputHandler->GetMouseDelta().y;
	float mouseSens = 0.1f;

	if (inputHandler->GetMouseDelta().y || inputHandler->GetMouseDelta().x)
		this->m_cameraRef->RotateCameraPivot(inputHandler->GetMouseDelta().y * mouseSens, inputHandler->GetMouseDelta().x * mouseSens);

	//update player for throw functionallity
	DirectX::XMVECTOR playerLookDir = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->m_cameraRef->GetLookAt()), DirectX::XMLoadFloat3(&this->m_cameraRef->GetCameraPos())));
	DirectX::XMFLOAT3 temp;

	this->m_cameraRef->GetCameraUp(temp);

	DirectX::XMVECTOR upDir = DirectX::XMLoadFloat3(&temp);
	DirectX::XMVECTOR rightDir = m_cameraRef->GetRight(); //DirectX::XMVector3Cross(upDir, playerLookDir);

	//Camera
	this->m_player1.SetRightDir(rightDir);
	this->m_player1.SetUpDir(upDir);
	this->m_player1.SetLookDir(playerLookDir);

#pragma endregion Camera_Update

#pragma region

	Entity* wasGrabbed = this->m_player1.GetGrabbed();
	this->m_player1.Update(dt, inputHandler);

	//Check if we released a grabbed object in player Update
	if (wasGrabbed != this->m_player1.GetGrabbed() && wasGrabbed != nullptr)
	{
		wasGrabbed->SyncComponents();	//Update the component
		this->m_networkModule->SendGrabPacket(this->m_player1.GetEntityID(), -1);	//Send a release packet
		DirectX::XMFLOAT4X4 newrot;
		DirectX::XMStoreFloat4x4(&newrot, wasGrabbed->GetPhysicsComponent()->PC_OBB.ort);
		this->m_networkModule->SendEntityUpdatePacket(wasGrabbed->GetPhysicsComponent()->PC_entityID, wasGrabbed->GetPhysicsComponent()->PC_pos, wasGrabbed->GetPhysicsComponent()->PC_velocity, newrot);	//Send the update data
	}


	//update all dynamic (moving) entities
	Entity* ent = nullptr;
	for (size_t i = 0; i < this->m_dynamicEntitys.size(); i++)
	{
		ent = this->m_dynamicEntitys.at(i);
		if (ent == this->m_player2.GetGrabbed())		//Check if the entity is  grabbed by player2, if it is there will be an update packet for it
		{
			ent->SyncComponents();	//Just sync the component and wait for the update package
		}
		else
		{
			ent->Update(dt, inputHandler);	//Update the entity normaly
		}
	}
	//Sync other half of the components
	this->m_player2.SyncComponents();
#pragma endregion Update/Syncing Components

#pragma region
	if (inputHandler->IsMouseKeyPressed(SDL_BUTTON_LEFT)
		&& this->m_player1.GetGrabbed() == nullptr
		&& this->m_player1.TimeSinceThrow() >= GRAB_COOLDOWN)
	{
		Entity* closestBall = this->GetClosestBall(GRAB_RANGE);

		if (closestBall != nullptr)	//If a ball was found
		{
			if (this->m_player1.GetBall()->IsGrabbed()						  //if our ball is grabbed
				&& this->m_player1.GetGrabbed() != this->m_player1.GetBall()) //AND if the ball is not grabbed by us
			{
				//now we know that player 2 is holding our ball

				//drop the ball
				this->m_player2.SetGrabbed(nullptr);

				//send update packet to client 2
				this->m_networkModule->SendGrabPacket(this->m_player2.GetEntityID(), -1);
			}
			this->m_player1.SetGrabbed(closestBall);
			this->m_networkModule->SendGrabPacket(this->m_player1.GetEntityID(), closestBall->GetEntityID());

			//Play the animation for player picking up the ball.
			this->m_player1.GetAnimationComponent()->previousState = this->m_player1.GetAnimationComponent()->currentState;
			this->m_player1.SetAnimationComponent(AnimationStates::PLAYER_PICKUP, 0.45f, Blending::FROZEN_TRANSITION, false, true, 1.75f, 1.0f);
			this->m_player1.GetAnimationComponent()->currentState = AnimationStates::PLAYER_PICKUP;
		}

	}
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_Q))
	{
		this->m_player1.SetGrabbed(nullptr);
		this->m_networkModule->SendGrabPacket(this->m_player1.GetEntityID(), -1);

		/*Set the component to play the animation for IDLE if the player is standing still when PCs velocity is under 1.*/
		if (DirectX::XMVectorGetX(DirectX::XMVector3Length(this->m_player1.GetPhysicsComponent()->PC_velocity)) < 1.0f )
		{
			this->m_player1.GetAnimationComponent()->previousState = this->m_player1.GetAnimationComponent()->currentState;
			this->m_player1.SetAnimationComponent(AnimationStates::PLAYER_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true, false, 1.0f, 1.0f);
			this->m_player1.GetAnimationComponent()->currentState = AnimationStates::PLAYER_IDLE;
		}
	}
	if (this->m_player1.GetRagdoll()->state == RagdollState::RAGDOLL)
	{
		if (this->m_player1.GetGrabbed() != nullptr)
		{
			this->m_player1.SetGrabbed(nullptr);
			this->m_networkModule->SendGrabPacket(this->m_player1.GetEntityID(), -1);
		}
	}
#pragma endregion Grab/Release

#pragma region
	this->m_grabPacketList = this->m_networkModule->PacketBuffer_GetGrabPacket();	//This removes the entity packets from the list in NetworkModule
	/*
	We know that all packets are from the other player (m_player2)

	*/
	if (this->m_grabPacketList.size() > 0)
	{
		std::list<GrabPacket>::iterator itr;
		PhysicsComponent* pp = nullptr;
		Entity* ep = nullptr;

		for (itr = this->m_grabPacketList.begin(); itr != this->m_grabPacketList.end(); itr++)
		{
			//Check if we want to grab or drop			
			if (itr->grabbedID >= 0)	//Grab
			{
				if (itr->grabbedID == 3)
				{
					this->m_player2.SetGrabbed(this->m_player2.GetBall());
				}
				else if (itr->grabbedID == 4)
				{
					this->m_player2.SetGrabbed(this->m_player1.GetBall());
				}

			}
			else //Drop
			{
				if (itr->entityID == DEFINED_IDS::PLAYER_1)
				{
					this->m_player2.SetGrabbed(nullptr);
				}
				else if (itr->entityID == DEFINED_IDS::PLAYER_2)
				{
					this->m_player1.SetGrabbed(nullptr);
				}
			}

		}
	}
	this->m_grabPacketList.clear();
#pragma endregion Grab_Requests

#pragma region
	//Aming for player1 (SHOULD BE FOR THE CONTROLED PLAYER)
	if (inputHandler->IsMouseKeyDown(SDL_BUTTON_RIGHT))
	{
		this->m_player1.SetAiming(true);
		//Crosshair overlay
		this->m_crosshair->active = true;

		DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(.3f, 1.4f, 0.0f, 0.0f);
		targetOffset = DirectX::XMVectorScale(this->m_player1.GetRightDir(), 0.3f);
		targetOffset = DirectX::XMVectorAdd(targetOffset, { 0.0f, 1.25f, 0.0f, 0.0f });

		m_cameraRef->SetCameraPivotOffset(
			targetOffset,
			.5f
		);
	}

	if (inputHandler->IsMouseKeyReleased(SDL_BUTTON_RIGHT) && this->m_player1.GetIsAming())
	{
		this->m_player1.SetAiming(false);
		//Crosshair overlay
		this->m_crosshair->active = false;

		DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.f, 1.4f, 0.0f, 0.0f);
		m_cameraRef->SetCameraPivotOffset(
			targetOffset,
			1.3f
		);
	}

	if (this->m_player1.GetIsAming())
	{
		this->m_player1.SetLookDir(this->m_cameraRef->GetDirection());
	}
#pragma endregion Aiming

#pragma region

	this->m_pingPacketList = this->m_networkModule->PacketBuffer_GetPingPacket();

	//Check for updates
	if (this->m_pingPacketList.size() != 0)
	{
		std::list<PingPacket>::iterator itr;

		for (itr = this->m_pingPacketList.begin(); itr != this->m_pingPacketList.end(); itr++)
		{
			this->m_player2_Ping.SetPos(DirectX::XMLoadFloat3(&itr->newPos));
			this->m_player2_Ping.m_gComp->active = true;
			this->m_player2_Ping.m_time = 0;
			SoundHandler::instance().PlaySound3D(Sounds3D::PING_EFFECT_SOUND, this->m_player2_Ping.m_pos, false, false);
		}

		this->m_pingPacketList.empty();
	}

	if (inputHandler->IsKeyPressed(SDL_SCANCODE_T))
	{
		float distance = this->m_cHandler->GetGraphicsHandler()->Ping_GetDistanceToClosestOBB(PING_DISTANCE);

		if (distance < PING_DISTANCE)
		{
			//Calculate the point using the camera dir vector
			DirectX::XMVECTOR dir = this->m_cameraRef->GetDirection();
			DirectX::XMVECTOR scaledDir = DirectX::XMVectorScale(dir, distance - 1);	//-1 to get abit of distance from what we are hiting
			DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&this->m_cameraRef->GetCameraPos());

			DirectX::XMVECTOR newPos = DirectX::XMVectorAdd(camPos, scaledDir);

			this->m_player1_Ping.SetPos(newPos);;	//Set the pos for the ping
			this->m_player1_Ping.m_gComp->active = true;
			this->m_player1_Ping.m_time = 0;

			this->m_networkModule->SendPingPacket(this->m_player1_Ping.m_pos);

			SoundHandler::instance().PlaySound3D(Sounds3D::PING_EFFECT_SOUND, this->m_player1_Ping.m_pos, false, false);
		}
	}

	this->m_player1_Ping.Update(dt);
	this->m_player2_Ping.Update(dt);
#pragma endregion Ping



#pragma region

	if (this->m_networkModule->GetNrOfConnectedClients() != 0)	//There is connected players
	{
		PhysicsComponent* pp = this->m_player1.GetPhysicsComponent();
		if (pp != nullptr)
		{
			DirectX::XMFLOAT4X4 newrot;
			DirectX::XMStoreFloat4x4(&newrot, pp->PC_OBB.ort);
			this->m_networkModule->SendEntityUpdatePacket(pp->PC_entityID, pp->PC_pos, pp->PC_velocity, newrot);	//Send the update data for the player
		}

		/*if (this->m_player1.GetGrabbed() != nullptr)*/
		if (this->m_player1.GetBall() != nullptr &&
			this->m_player2.GetGrabbed() != this->m_player1.GetBall()) //send update of player1 ball if player2 has not grabbed it
		{
			pp = this->m_player1.GetBall()->GetPhysicsComponent();
			DirectX::XMFLOAT4X4 newrot;
			DirectX::XMStoreFloat4x4(&newrot, pp->PC_OBB.ort);
			this->m_networkModule->SendEntityUpdatePacket(pp->PC_entityID, pp->PC_pos, pp->PC_velocity, newrot);
		}
		if (this->m_player1.GetGrabbed() == this->m_player2.GetBall()) //send updates of player2 ball if player1 has grabbed it
		{
			pp = this->m_player2.GetBall()->GetPhysicsComponent();
			DirectX::XMFLOAT4X4 newrot;
			DirectX::XMStoreFloat4x4(&newrot, pp->PC_OBB.ort);
			this->m_networkModule->SendEntityUpdatePacket(pp->PC_entityID, pp->PC_pos, pp->PC_velocity, newrot);
		}
		Entity* ent = nullptr;
		if (this->m_networkModule->IsHost())
		{

			for (size_t i = 0; i < this->m_dynamicEntitys.size(); i++)	//Change start and end with physics packet
			{
				ent = this->m_dynamicEntitys.at(i);

				if (ent != this->m_player2.GetGrabbed() &&
					ent->GetEntityID() != DEFINED_IDS::CHAIN_1 && ent->GetEntityID() != DEFINED_IDS::CHAIN_2 &&
					ent->GetEntityID() != DEFINED_IDS::BALL_1 && ent->GetEntityID() != DEFINED_IDS::BALL_2	//if the hosting player 
					)
					//If it is not grabbed by player2 and is not a chain link
				{
					pp = this->m_dynamicEntitys.at(i)->GetPhysicsComponent();
					DirectX::XMFLOAT4X4 newrot;
					DirectX::XMStoreFloat4x4(&newrot, pp->PC_OBB.ort);
					this->m_networkModule->SendEntityUpdatePacket(pp->PC_entityID, pp->PC_pos, pp->PC_velocity, newrot);	//Send the update
				}
			}
		}

		if (this->m_networkModule->IsHost())
		{
			PhysicsComponent* pc = nullptr;
			for (PlatformEntity* e : this->m_platformEntities)
			{
				pc = e->GetPhysicsComponent();
				DirectX::XMFLOAT4X4 newrot;
				DirectX::XMStoreFloat4x4(&newrot, pc->PC_OBB.ort);
				this->m_networkModule->SendEntityUpdatePacket(pc->PC_entityID, pc->PC_pos, pc->PC_velocity,newrot);
	}
		}

	}

#pragma endregion Network_Send_Updates

#pragma region

	//Update all puzzle entities
#pragma region
//Commong variables needed for logic checks
	DirectX::XMFLOAT3 playerPos;
	DirectX::XMStoreFloat3(&playerPos, this->m_player1.GetPhysicsComponent()->PC_pos);
	//Buttons and levers require input for logical evaluation of activation
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_E))
	{
		//Iterator version of looping
		/*for (std::vector<ButtonEntity*>::iterator i = this->m_buttonEntities.begin(); i != this->m_buttonEntities.end(); i++)
		{
			DirectX::XMFLOAT3 playerPos;
			DirectX::XMStoreFloat3(&playerPos, this->m_player1.GetPhysicsComponent()->PC_pos);

			(*i)->CheckPressed(playerPos);
		}
		for (std::vector<LeverEntity*>::iterator i = this->m_leverEntities.begin(); i != this->m_leverEntities.end(); i++)
		{
			DirectX::XMFLOAT3 playerPos;
			DirectX::XMStoreFloat3(&playerPos, this->m_player1.GetPhysicsComponent()->PC_pos);

			(*i)->CheckPressed(playerPos);
		}*/
		//c++11 looping construct
		for (ButtonEntity* e : this->m_buttonEntities)
		{
			e->CheckPressed(playerPos);
		}
		for (LeverEntity* e : this->m_leverEntities)
		{
			e->CheckPressed(playerPos);
		}

	}

	if (inputHandler->IsKeyDown(SDL_SCANCODE_E))
	{

		int increasing = (inputHandler->IsKeyDown(SDL_SCANCODE_LSHIFT)) ? -1 : 1;	//Only uses addition but branches, kind of
		//increasing = -1 + (!inputHandler->IsKeyDown(SDL_SCANCODE_LSHIFT)) * 2;		//No branching calculation, but uses multiplication and addition

		for (std::vector<WheelEntity*>::iterator i = this->m_wheelEntities.begin(); i != this->m_wheelEntities.end(); i++)
		{
			(*i)->CheckPlayerInteraction(playerPos, increasing);
		}
	}
	else if (inputHandler->IsKeyReleased(SDL_SCANCODE_E))
	{
		for (std::vector<WheelEntity*>::iterator i = this->m_wheelEntities.begin(); i != this->m_wheelEntities.end(); i++)
		{
			(*i)->CheckPlayerInteraction(playerPos, 0);
		}
	}
	//Buttons require updates to countdown their reset timer
	for (std::vector<ButtonEntity*>::iterator i = this->m_buttonEntities.begin(); i != this->m_buttonEntities.end(); i++)
	{
		(*i)->Update(dt, inputHandler);
	}
	//Lever require updates to animate
	for (std::vector<LeverEntity*>::iterator i = this->m_leverEntities.begin(); i != this->m_leverEntities.end(); i++)
	{
		(*i)->Update(dt, inputHandler);
	}
	//Wheels require updates to rotate based on state calculated in CheckPlayerInteraction
	for (std::vector<WheelEntity*>::iterator i = this->m_wheelEntities.begin(); i != this->m_wheelEntities.end(); i++)
	{
		(*i)->Update(dt, inputHandler);
	}
	//Doors require updates to change opening state and animate
	for (std::vector<DoorEntity*>::iterator i = this->m_doorEntities.begin(); i != this->m_doorEntities.end(); i++)
	{
		(*i)->Update(dt, inputHandler);
	}

#pragma endregion Puzzle element update logic

#pragma region
	//Check for state changes that should be sent over the network
	LeverSyncState* leverSync = nullptr;
	for (LeverEntity* e : this->m_leverEntities)
	{
		leverSync = e->GetSyncState();
		if (leverSync != nullptr)
		{
			this->m_networkModule->SendStateLeverPacket(leverSync->entityID, leverSync->isActive);
			delete leverSync;
		}
	}
	ButtonSyncState* buttonSync = nullptr;
	for (ButtonEntity* e : this->m_buttonEntities)
	{
		buttonSync = e->GetSyncState();
		if (buttonSync != nullptr)
		{
			this->m_networkModule->SendStateButtonPacket(buttonSync->entityID, buttonSync->isActive);
			delete buttonSync;
		}
	}
	WheelSyncState* wheelSync = nullptr;
	for (WheelEntity* e : this->m_wheelEntities)
	{
		wheelSync = e->GetSyncState();
		if (wheelSync != nullptr)
		{
			this->m_networkModule->SendStateWheelPacket(wheelSync->entityID, wheelSync->rotationState, wheelSync->rotationAmount);
			delete wheelSync;
		}
	}
#pragma endregion Puzzle elements synchronization 

#pragma region
	for (size_t i = 0; i < this->m_platformEntities.size(); i++)
	{
		this->m_platformEntities[i]->Update(dt, inputHandler);
	}
#pragma endregion Platforms

#pragma endregion Update_Puzzle_Elements

#pragma region
	// We only send updates for player1 since player2 will recive the updates from the network
	AnimationComponent* ap = this->m_player1.GetAnimationComponent();
	if (this->m_player1.GetRagdoll()->state ==RagdollState:: RAGDOLL || this->m_player1.GetRagdoll()->state == RagdollState::KEYFRAMEBLEND)
	{
		GraphicsAnimationComponent* gp = (GraphicsAnimationComponent*)this->m_player1.GetGraphicComponent();

		for (int i = 0; i < gp->jointCount; i++)	//Iterate all joints
		{
			//Send a packet for E V E R Y joint
			this->m_networkModule->SendAnimationPacket(this->m_player1.GetEntityID(), RAGDOLL_STATE, 0.f, Blending::NO_TRANSITION, false, false, 0.f, 1.0, i, gp->finalJointTransforms[i]);
		}
	}
	else if (this->m_player1.isAnimationChanged())
	{
		this->m_networkModule->SendAnimationPacket(this->m_player1.GetEntityID(), ap->currentState, ap->transitionDuration, ap->blendFlag, ap->target_State->isLooping, ap->lockAnimation, ap->playingSpeed, ap->velocity, 0, DirectX::XMMATRIX());
	}

#pragma endregion Send_Player_Animation_Update

	for (size_t i = 0; i < m_fieldEntities.size(); i++)
	{
		int fieldActivated = m_fieldEntities[i]->Update(dt, inputHandler);
		this->m_clearedLevel = fieldActivated;
	}
	// Reactionary level director acts
	this->m_director.Update(dt);

#pragma region
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_INSERT) ||
		this->m_networkModule->PacketBuffer_GetResetPacket().size() != 0)
	{

		if (inputHandler->IsKeyPressed(SDL_SCANCODE_INSERT))	//If we clicked
		{
			this->m_networkModule->SendFlagPacket(SYNC_RESET);	//Send packet to other client
		}

		// Reset player-position to spawn
		if (this->m_networkModule->IsHost())
		{
			m_player1.GetPhysicsComponent()->PC_pos = m_player1_Spawn;
			m_player2.GetPhysicsComponent()->PC_pos = m_player2_Spawn;
			m_player1.GetBall()->GetPhysicsComponent()->PC_pos =
				DirectX::XMVectorAdd(m_player1_Spawn, DirectX::XMVectorSet(0.0f, .11f, 1.5f, 0.f));
			m_player2.GetBall()->GetPhysicsComponent()->PC_pos =
				DirectX::XMVectorAdd(m_player2_Spawn, DirectX::XMVectorSet(0.0f, .11f, 1.5f, 0.f));
			m_player1.GetPhysicsComponent()->PC_velocity = { 0 };
			m_player2.GetPhysicsComponent()->PC_velocity = { 0 };
			m_player1.GetBall()->GetPhysicsComponent()->PC_velocity = { 0 };
			m_player2.GetBall()->GetPhysicsComponent()->PC_velocity = { 0 };
		}
		else
		{
			m_player1.GetPhysicsComponent()->PC_pos = m_player2_Spawn;
			m_player2.GetPhysicsComponent()->PC_pos = m_player1_Spawn;
			m_player1.GetBall()->GetPhysicsComponent()->PC_pos =
				DirectX::XMVectorAdd(m_player2_Spawn, DirectX::XMVectorSet(0.0f, .11f, 1.5f, 0.f));
			m_player2.GetBall()->GetPhysicsComponent()->PC_pos =
				DirectX::XMVectorAdd(m_player1_Spawn, DirectX::XMVectorSet(0.0f, .11f, 1.5f, 0.f));
			m_player1.GetPhysicsComponent()->PC_velocity = { 0 };
			m_player2.GetPhysicsComponent()->PC_velocity = { 0 };
			m_player1.GetBall()->GetPhysicsComponent()->PC_velocity = { 0 };
			m_player2.GetBall()->GetPhysicsComponent()->PC_velocity = { 0 };
		}

		// Iterate through chainlink list to reset velocity and position of players, chain links, and balls
		this->m_cHandler->GetPhysicsHandler()->ResetChainLink();

	}
#ifdef DEVELOPMENTFUNCTIONS
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_Y))
	{
		//TODO: NOCLIP BOOOOIS
		if (inputHandler->IsKeyDown(SDL_SCANCODE_LSHIFT))
		{
			m_player1.GetPhysicsComponent()->PC_gravityInfluence = 1;
			m_player1.GetPhysicsComponent()->PC_steadfast = false;
		}
		else
		{
			m_player1.GetPhysicsComponent()->PC_gravityInfluence = 0;
			m_player1.GetPhysicsComponent()->PC_steadfast = true;
		}
		//Result: Sloppy teleport :(
		m_player1.GetPhysicsComponent()->PC_pos = DirectX::XMVectorAdd(
			m_player1.GetPhysicsComponent()->PC_pos,
			(DirectX::XMVectorScale(m_player1.GetLookDir(), 3.0f)));
		this->m_cHandler->GetPhysicsHandler()->ResetChainLink();
	}
#endif // DEVELOPMENTFUNCTIONS


#pragma endregion Reset KEY


#ifdef DEVELOPMENTFUNCTIONS
#pragma region
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_M))
	{
		//SoundHandler::instance().PlaySound2D(Sounds2D::MENU, false, false);
	}
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_N))
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, this->m_player2.GetPhysicsComponent()->PC_pos);
		SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_CHAIN_DRAG_1, pos, true, false);
	}
#pragma endregion MUSIC_KEYS  
#endif // DEVELOPMENTFUNCTIONS

	if (this->m_player1.GetRagdoll()->state == RagdollState::ANIMATED)
	{
		this->m_cameraRef->Update();
	}
	else
	{
		this->m_cameraRef->RagdollCameraUpdate(this->m_player1.GetPhysicsComponent()->PC_pos, this->m_player1.GetRagdoll()->state);
	}

	//Update the listner pos and direction for sound
	DirectX::XMFLOAT3 dir;
	DirectX::XMStoreFloat3(&dir, this->m_cameraRef->GetDirection());
	DirectX::XMFLOAT3 up;
	this->m_cameraRef->GetCameraUp(up);
	SoundHandler::instance().UpdateListnerPos(this->m_cameraRef->GetCameraPos(), dir, up);


	if (inputHandler->IsKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		this->m_networkModule->SendFlagPacket(PacketTypes::DISCONNECT_REQUEST);
		this->m_cHandler->RemoveLastUIComponent();
		this->m_cHandler->RemoveLastUIComponent();
		this->m_gsh->PopStateFromStack();

		//MenuState* menuState = new MenuState();
		//result = menuState->Initialize(this->m_gsh, this->m_cHandler, this->m_cameraRef);

		//if (result > 0)
		//{
		//	//Push it to the gamestate stack/vector
		//	this->m_gsh->PushStateToStack(menuState);
		//}
		//else
		//{
		//	//Delete it
		//	delete menuState;
		//	menuState = nullptr;
		//}

		result = 1;
	}

	//Controls overlay
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_F1))
	{
		this->m_controlsOverlay->active = 1;
	}
	if (inputHandler->IsKeyReleased(SDL_SCANCODE_F1))
	{
		this->m_controlsOverlay->active = 0;
	}

	return result;
}

int LevelState::CreateLevel(LevelData::Level * data)
{
	Resources::ResourceHandler* resHandler = Resources::ResourceHandler::GetInstance();
#pragma region
	this->m_cameraRef->UpdateProjection(this->m_levelPaths[this->m_curLevel].farPlane);
#pragma endregion Camera projection matrix update
#pragma region
	//Get how many static and dynamic components that will be needed in the level
	int staticEntityCount = 0;
	int dynamicEntityCount = 0;
	//Normal entities
	for (size_t i = 0; i < data->numEntities; i++)
	{
		staticEntityCount += data->entities[i].isStatic;
		dynamicEntityCount += !data->entities[i].isStatic;
	}
	//AI entities
	dynamicEntityCount += data->numAI;
	//Puzzle elements
	dynamicEntityCount += data->numButton;
	dynamicEntityCount += data->numLever;
	dynamicEntityCount += data->numWheel;
	dynamicEntityCount += data->numDoor;
	dynamicEntityCount += CHAIN_SEGMENTS * 2;

	this->m_cHandler->ResizeGraphicsStatic(staticEntityCount);
	this->m_cHandler->ResizeGraphicsDynamic(dynamicEntityCount);
#pragma endregion Update component list sizes

#pragma region
	DirectX::XMVECTOR rot;
	DirectX::XMVECTOR pos;
	rot.m128_f32[3] = 0.0f;	//Set w to 0
	pos.m128_f32[3] = 0.0f;	//Set w to 0

	DirectX::XMMATRIX translate;
	DirectX::XMMATRIX rotate;
	Resources::Model* modelPtr;
	Resources::Status st = Resources::ST_OK;

	this->m_player1_Spawn = DirectX::XMVectorSet( //Store spawnPoint for player 1
		data->spawns[0].position[0],
		data->spawns[0].position[1],
		data->spawns[0].position[2],
		0);
	this->m_player2_Spawn = DirectX::XMVectorSet(	//Store spawnPoint for player 2
		data->spawns[1].position[0],
		data->spawns[1].position[1],
		data->spawns[1].position[2],
		0);
#pragma endregion preparations and variable

#pragma region
	//NETWORK SAFETY
	/*
	This is only a safety check if the network module was not initialized.
	It should have been initialized in the menu state and have been connected
	to the other player. By using isHost() function we know can determine if
	we should switch the players ID and position or not.
	*/
	if (!this->m_networkModule)
	{
		this->m_networkModule = new NetworkModule();
		this->m_networkModule->Initialize();
	}

	if (this->m_networkModule->IsHost())
	{
		this->m_player1.GetPhysicsComponent()->PC_pos = this->m_player1_Spawn;
		//this->m_player1.GetPhysicsComponent()->PC_pos = DirectX::XMVectorAdd(this->m_player1_Spawn, DirectX::XMVectorSet(0, 0, 0, 0));
		this->m_cHandler->GetPhysicsHandler()->CreateRagdollBodyWithChainAndBall(1, this->m_player1.GetAnimationComponent()->skeleton->GetSkeletonData()->joints,
			this->m_player1.GetAnimationComponent(),
			DirectX::XMVectorAdd(this->m_player1.GetPhysicsComponent()->PC_pos, DirectX::XMVectorSet(10, 0, 0, 0)),
			this->m_player1.GetPhysicsComponent(),
			this->m_player1.GetBall()->GetPhysicsComponent());

		this->m_player2.GetPhysicsComponent()->PC_pos = DirectX::XMVectorAdd(this->m_player2_Spawn, DirectX::XMVectorSet(0, 1, 0, 0));
	}
	else
	{
		this->m_player1.GetPhysicsComponent()->PC_pos = this->m_player2_Spawn;
		this->m_cHandler->GetPhysicsHandler()->CreateRagdollBodyWithChainAndBall(1, this->m_player1.GetAnimationComponent()->skeleton->GetSkeletonData()->joints,
			this->m_player1.GetAnimationComponent(),
			DirectX::XMVectorAdd(this->m_player1.GetPhysicsComponent()->PC_pos, DirectX::XMVectorSet(10, 0, 0, 0)),
			this->m_player1.GetPhysicsComponent(),
			this->m_player1.GetBall()->GetPhysicsComponent());
		this->m_player2.GetPhysicsComponent()->PC_pos = this->m_player1_Spawn;
	}
#pragma endregion Network
#pragma region
	this->m_player1.GetBall()->GetPhysicsComponent()->PC_pos =
		DirectX::XMVectorAdd(
			m_player1.GetPhysicsComponent()->PC_pos, DirectX::XMVectorSet(2.0f, 0.0f, 0.0f, 0.0f));
	m_player2.GetBall()->GetPhysicsComponent()->PC_pos =
		DirectX::XMVectorAdd(
			m_player2.GetPhysicsComponent()->PC_pos, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));


#pragma region
	float linkLenght = 0.5f;
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
	PhysicsComponent* previous = this->m_player1.GetPhysicsComponent();
	PhysicsComponent* next = nullptr;
	PhysicsComponent* PC_ptr = nullptr;
	this->m_Player1ChainPhysicsComp.push_back(this->m_player1.GetPhysicsComponent());
	for (int i = 0; i < CHAIN_SEGMENTS; i++)
	{
		if (i != 0)
		{
			linkLenght = 0.5f;
		}
		unsigned int entityID = DEFINED_IDS::CHAIN_1;
		PC_ptr = this->m_cHandler->GetPhysicsComponent();

		PC_ptr->PC_pos = DirectX::XMVectorAdd(this->m_player1.GetPhysicsComponent()->PC_pos, DirectX::XMVectorScale(diffVec, float(i)));
		PC_ptr->PC_entityID = entityID;
		PC_ptr->PC_BVtype = BV_Sphere;
		PC_ptr->PC_Sphere.radius = 0.1f;
		PC_ptr->PC_mass = 0.2f;
		PC_ptr->PC_friction = 1.0f;
		GraphicsComponent* GC_ptr = this->m_cHandler->GetDynamicGraphicsComponent();
		GC_ptr->modelID = CHAIN_SEGMENT_MODEL_ID;
		GC_ptr->active = false;
		resHandler->GetModel(GC_ptr->modelID, GC_ptr->modelPtr);
		DynamicEntity* chainLink = new DynamicEntity();
		chainLink->Initialize(entityID, PC_ptr, GC_ptr);
		this->m_dynamicEntitys.push_back(chainLink);

		next = PC_ptr;
		this->m_Player1ChainPhysicsComp.push_back(PC_ptr);
		PC_ptr = nullptr;

		if (i == 0)
		{

			this->m_player1.GetRagdoll()->link_index = this->m_cHandler->GetPhysicsHandler()->CreateLink(previous, next, linkLenght, PhysicsLinkType::PL_CHAIN);
		}
		else
		{
			this->m_cHandler->GetPhysicsHandler()->CreateLink(previous, next, linkLenght, PhysicsLinkType::PL_CHAIN);
		}


		previous = next;

	}

	this->m_Player1ChainPhysicsComp.push_back(this->m_player1.GetBall()->GetPhysicsComponent());
	this->m_cHandler->GetPhysicsHandler()->CreateLink(previous, this->m_player1.GetBall()->GetPhysicsComponent(), linkLenght, PhysicsLinkType::PL_CHAIN);
	diffVec = DirectX::XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
	linkLenght = 0.5f;
	previous = this->m_player2.GetPhysicsComponent();
	next = nullptr;
	this->m_Player2ChainPhysicsComp.push_back(this->m_player2.GetPhysicsComponent());
	for (int i = 1; i <= CHAIN_SEGMENTS; i++)
	{
		if (i != 1)
		{
			linkLenght = 0.50f;
		}
		unsigned int entityID = DEFINED_IDS::CHAIN_2;
		PhysicsComponent* PC_ptr = this->m_cHandler->GetPhysicsComponent();

		PC_ptr->PC_pos = DirectX::XMVectorAdd(this->m_player2.GetPhysicsComponent()->PC_pos, DirectX::XMVectorScale(diffVec, float(i)));
		PC_ptr->PC_entityID = entityID;
		PC_ptr->PC_BVtype = BV_Sphere;
		PC_ptr->PC_Sphere.radius = 0.1f;
		PC_ptr->PC_mass = 0.2f;
		PC_ptr->PC_friction = 1.0f;
		GraphicsComponent* GC_ptr = this->m_cHandler->GetDynamicGraphicsComponent();
		GC_ptr->modelID = CHAIN_SEGMENT_MODEL_ID;
		GC_ptr->active = false;
		resHandler->GetModel(GC_ptr->modelID, GC_ptr->modelPtr);
		DynamicEntity* chainLink = new DynamicEntity();
		chainLink->Initialize(entityID, PC_ptr, GC_ptr);
		this->m_dynamicEntitys.push_back(chainLink);


		next = PC_ptr;
		this->m_Player2ChainPhysicsComp.push_back(PC_ptr);
		if (i == 1)
		{
			this->m_cHandler->GetPhysicsHandler()->CreateLink(previous, next, linkLenght, PhysicsLinkType::PL_CHAIN);
		}
		else
		{
			this->m_cHandler->GetPhysicsHandler()->CreateLink(previous, next, linkLenght, PhysicsLinkType::PL_CHAIN);
		}


		previous = next;
	}

	this->m_Player2ChainPhysicsComp.push_back(this->m_player2.GetBall()->GetPhysicsComponent());
	this->m_cHandler->GetPhysicsHandler()->CreateLink(previous, this->m_player2.GetBall()->GetPhysicsComponent(), linkLenght, PhysicsLinkType::PL_CHAIN);

#pragma endregion Create_Chain_Link

	this->m_cHandler->GetPhysicsHandler()->ResetChainLink();

#pragma endregion Checkpoints


	for (size_t i = 0; i < data->numEntities; i++)
	{
		LevelData::EntityHeader* currEntity = &data->entities[i]; //Current entity
		currEntity->EntityID += DEFINED_IDS::NUMMBER_OF_IDS;	//ADD number of predefined ids to avoid conflict from editor
		GraphicsComponent* t_gc;
		Resources::Model * modelPtr;

		resHandler->GetModel(currEntity->modelID, modelPtr);

		if (modelPtr->GetSkeleton() != nullptr)
		{
			t_gc = m_cHandler->GetGraphicsAnimationComponent();
		}
		else
		{
			if (currEntity->isStatic)
				t_gc = m_cHandler->GetStaticGraphicsComponent();
			else
				t_gc = m_cHandler->GetDynamicGraphicsComponent();
		}

		t_gc->modelID = currEntity->modelID;
		t_gc->active = true;
		t_gc->modelPtr = modelPtr; //Get and apply a pointer to the model

		//Create GraphicsComponent

		//Create world matrix from data
		memcpy(pos.m128_f32, currEntity->position, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		memcpy(rot.m128_f32, currEntity->rotation, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		translate = DirectX::XMMatrixTranslationFromVector(pos);
#pragma region
		//Create the rotation matrix
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rot.m128_f32[1]));
		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rot.m128_f32[0]));
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rot.m128_f32[2]));

		DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);
		//rotate    = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
#pragma endregion Calculate rotation matrix
		//Create the world matrix from a rotation and translation
		t_gc->worldMatrix = DirectX::XMMatrixMultiply(rotate, translate);

		//Create Physics component
		PhysicsComponent* t_pc = m_cHandler->GetPhysicsComponent();
		t_pc->PC_entityID = currEntity->EntityID;		//Set Entity ID
		//We cannot set pos directly because of bounding box vs model offsets
		//t_pc->PC_pos		   = pos;						//Set Position
		t_pc->PC_rotation = rot;						//Set Rotation
		t_pc->PC_is_Static = currEntity->isStatic;		//Set IsStatic
		t_pc->PC_active = true;						//Set Active
		t_pc->PC_BVtype = BV_OBB;

		//t_pc->PC_OBB.ort = DirectX::XMMatrixMultiply(t_pc->PC_OBB.ort, rotate);
		//st = Resources::ResourceHandler::GetInstance()->GetModel(currEntity->modelID, modelPtr);



		DirectX::XMMATRIX tempOBBPos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y, modelPtr->GetOBBData().position.z });
		tempOBBPos = DirectX::XMMatrixMultiply(tempOBBPos, t_gc->worldMatrix);

		t_pc->PC_pos = tempOBBPos.r[3];
		DirectX::XMStoreFloat3(&t_gc->pos, tempOBBPos.r[3]);
		//t_pc->PC_pos.m128_f32[3] = 1.0f;

		//get information from file
		//static components should have the mass of 0
		//t_pc->PC_mass = 0;
		//t_pc->PC_friction = 0.55f;
#ifdef _DEBUG
		if (st != Resources::ST_OK)
			std::cout << "Model could not be found when loading level data,  ID: " << currEntity->modelID << std::endl;//NOTE: IS offseted by DEFINED_IDS::NUMMBER_OF_IDS
#endif // _DEBUG

		t_pc->PC_OBB = m_ConvertOBB(modelPtr->GetOBBData()); //Convert and insert OBB data

		t_pc->PC_OBB.ort = DirectX::XMMatrixMultiply(t_pc->PC_OBB.ort, rotate);
		t_pc->PC_OBB.ort = DirectX::XMMatrixTranspose(t_pc->PC_OBB.ort);
		//This is where the final rotation is stores. We want to use this rotation matrix and get the extensions for the OBB
		//Create a vector describing the extension of the AABB
		//Rotate the extension according the OBB ort
		//And lastly store the result in graphics components as the model bounds used in culling
		DirectX::XMStoreFloat3(&t_gc->extensions, DirectX::XMVector3Transform(DirectX::XMVectorSet(t_pc->PC_OBB.ext[0], t_pc->PC_OBB.ext[1], t_pc->PC_OBB.ext[2], 0.0f), t_pc->PC_OBB.ort));
		t_gc->ort = t_pc->PC_OBB.ort;
		//sets the friction of the static environment if extension in y is smaller than 0.5 we assume its the floor and give it a higher friction than the walls 
		if (t_pc->PC_OBB.ext[1] < 0.5f && DirectX::XMVector3Equal(t_pc->PC_OBB.ort.r[1], DirectX::XMVectorSet(0, 1, 0, 0)))
		{
			t_pc->PC_mass = 0;
			t_pc->PC_friction = 1.0f;
		}
		else //walls get lower friction 
		{
			t_pc->PC_mass = 0;
			t_pc->PC_friction = 0.0f;
		}

		if (t_pc->PC_is_Static) {
			StaticEntity* tse = new StaticEntity();
			tse->Initialize(t_pc->PC_entityID, t_pc, t_gc, nullptr);// Entity needs its ID
			this->m_staticEntitys.push_back(tse); //Push new entity to list
		}
		else {

			DynamicEntity* tde = new DynamicEntity();
			tde->Initialize(t_pc->PC_entityID, t_pc, t_gc, nullptr);// Entity needs its ID
			this->m_dynamicEntitys.push_back(tde); //Push new entity to list
		}
	}
#pragma region
	for (size_t i = 0; i < data->numAI; i++)
	{
		AIComponent* t_ac = m_cHandler->GetAIComponent();
		t_ac->AC_triggered = true;// Temp: Needed for AIHandler->Update()
		t_ac->AC_entityID = data->aiComponents[i].EntityID + DEFINED_IDS::NUMMBER_OF_IDS;	//Add nummber of predefined ids to avoid conflict from editor
		t_ac->AC_time = data->aiComponents[i].time;
		t_ac->AC_speed = data->aiComponents[i].speed;
		t_ac->AC_pattern = data->aiComponents[i].pattern;
		t_ac->AC_nrOfWaypoint = data->aiComponents[i].nrOfWaypoints;
		for (int x = 0; x < t_ac->AC_nrOfWaypoint; x++)
		{
			t_ac->AC_waypoints[x] = {
				data->aiComponents[i].wayPoints[x][0],
				data->aiComponents[i].wayPoints[x][1],
				data->aiComponents[i].wayPoints[x][2]
			};
		}
		t_ac->AC_position = t_ac->AC_waypoints[0];
#pragma region Graphics
		resHandler->GetModel(data->aiComponents[i].modelID, modelPtr);
		GraphicsComponent* t_gc = m_cHandler->GetDynamicGraphicsComponent();
		t_gc->active = 1;
		t_gc->modelID = data->aiComponents[i].modelID;
		t_gc->modelPtr = modelPtr;
		//Create world matrix from data
		memcpy(rot.m128_f32, data->aiComponents[i].rotation, sizeof(float) * 3);//Convert from POD to DirectX Vector
		translate = DirectX::XMMatrixTranslationFromVector(t_ac->AC_position);
		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rot.m128_f32[0]));
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rot.m128_f32[1]));
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rot.m128_f32[2]));
		//Create the rotation matrix
		DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);
		t_gc->worldMatrix = DirectX::XMMatrixMultiply(rotate, translate);

		st = Resources::ResourceHandler::GetInstance()->GetModel(data->aiComponents[i].modelID, modelPtr);
#ifdef _DEBUG
		if (st != Resources::ST_OK)
			std::cout << "Model could not be found when loading level data,  ID: " << data->aiComponents[i].modelID << std::endl;
#endif // _DEBUG
#pragma endregion
#pragma region Physics
		PhysicsComponent* t_pc = m_cHandler->GetPhysicsComponent();
		t_pc->PC_pos = t_ac->AC_position;
		t_pc->PC_entityID = data->aiComponents[i].EntityID + DEFINED_IDS::NUMMBER_OF_IDS; //Add nummber of predefined ids to avoid conflict from editor
		t_pc->PC_is_Static = false;
		t_pc->PC_steadfast = true;
		t_pc->PC_gravityInfluence = 0;
		t_pc->PC_friction = 0.0f;
		t_pc->PC_elasticity = 0.1f;
		t_pc->PC_BVtype = BV_OBB;
		t_pc->PC_mass = 0;
		t_pc->PC_AABB.ext[0] = modelPtr->GetOBBData().extension[0];
		t_pc->PC_AABB.ext[1] = modelPtr->GetOBBData().extension[1];
		t_pc->PC_AABB.ext[2] = modelPtr->GetOBBData().extension[2];
		DirectX::XMVECTOR tempRot = DirectX::XMVector3Transform(DirectX::XMVECTOR{ t_pc->PC_AABB.ext[0],
			t_pc->PC_AABB.ext[1] , t_pc->PC_AABB.ext[2] }, rotate);
		t_pc->PC_AABB.ext[0] = abs(tempRot.m128_f32[0]);
		t_pc->PC_AABB.ext[1] = abs(tempRot.m128_f32[1]);
		t_pc->PC_AABB.ext[2] = abs(tempRot.m128_f32[2]);
		t_pc->PC_OBB = m_ConvertOBB(modelPtr->GetOBBData()); //Convert and insert OBB data

		// Adjust OBB in physics component - hack...
		DirectX::XMMATRIX tempOBBPos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{
			modelPtr->GetOBBData().position.x,
			modelPtr->GetOBBData().position.y,
			modelPtr->GetOBBData().position.z
	});
		tempOBBPos = DirectX::XMMatrixMultiply(tempOBBPos, t_gc->worldMatrix);
		t_pc->PC_OBB.ort = rotate;
#pragma endregion

		PlatformEntity* tpe = new PlatformEntity();
		tpe->Initialize(t_pc->PC_entityID, t_pc, t_gc, t_ac);
		this->m_platformEntities.push_back(tpe);
}

	m_cHandler->WaypointTime();

#pragma endregion AI

#pragma region
	for (size_t i = 0; i < data->numCheckpoints; i++)
	{
		Field* tempField = this->m_cHandler->GetPhysicsHandler()->CreateField(
			data->checkpoints[i].position,
			DEFINED_IDS::PLAYER_1,	//EntityID Player1
			DEFINED_IDS::PLAYER_2,	//EntityID Player2
			data->checkpoints[i].ext,
			data->checkpoints[i].ort
		);
		FieldEntity* tempFE = new FieldEntity();
		tempFE->Initialize(data->checkpoints[i].entityID + DEFINED_IDS::NUMMBER_OF_IDS, tempField);
		this->m_fieldEntities.push_back(tempFE);
		this->m_fieldEntities[i]->AddObserver(&this->m_director, this->m_director.GetID());
	}

	// TODO: Field Data for States in Level Director
#pragma endregion Creating Fields

	//Create the PuzzleElements
#pragma region
	//Create the Buttons
	for (size_t i = 0; i < data->numButton; i++)
	{
		LevelData::ButtonHeader tempHeader = data->buttons[i];
		tempHeader.EntityID += DEFINED_IDS::NUMMBER_OF_IDS; //Add nummber of predefined ids to avoid conflict from editor
		ButtonEntity* tempEntity = new ButtonEntity();

		//Create world matrix from data
		memcpy(pos.m128_f32, tempHeader.position, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		memcpy(rot.m128_f32, tempHeader.rotation, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		//Convert the useless values into proper system supported radians. Glorious.
		rot = DirectX::XMVectorSet(DirectX::XMConvertToRadians(DirectX::XMVectorGetX(rot)), DirectX::XMConvertToRadians(DirectX::XMVectorGetY(rot)), DirectX::XMConvertToRadians(DirectX::XMVectorGetZ(rot)), 1.0f);
		translate = DirectX::XMMatrixTranslationFromVector(pos);
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(rot.m128_f32[1]);
		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(rot.m128_f32[0]);
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(rot.m128_f32[2]);
		//Should just use this function instead of a bunch
		//DirectX::waa(rot);
		//Create the rotation matrix
		DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);
		//rotate    = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);

		GraphicsComponent* button1G = m_cHandler->GetDynamicGraphicsComponent();

		button1G->active = true;
		button1G->modelID = tempHeader.modelID;
		button1G->worldMatrix = DirectX::XMMatrixMultiply(rotate, translate);
		resHandler->GetModel(button1G->modelID, button1G->modelPtr);
		PhysicsComponent* button1P = m_cHandler->GetPhysicsComponent();
		button1P->PC_entityID = tempHeader.EntityID;								//Set Entity ID
		button1P->PC_pos = pos;														//Set Position
		button1P->PC_rotation = rot;												//Set Rotation
		button1P->PC_is_Static = true;												//Set IsStatic
		button1P->PC_active = true;													//Set Active
		button1P->PC_gravityInfluence = 1.0f;
		button1P->PC_mass = 0;
		button1P->PC_friction = 0.00f;
		button1P->PC_collides = true;
		button1P->PC_elasticity = 0.2f;
		button1P->PC_steadfast = true;
		//DirectX::XMQuaternionRotationMatrix

		button1P->PC_BVtype = BV_OBB;
		//Copy the bounding volume data from the model into the physics component for reference
		button1P->PC_OBB = m_ConvertOBB(button1G->modelPtr->GetOBBData()); //Convert and insert OBB data
		button1P->PC_AABB.ext[0] = button1G->modelPtr->GetOBBData().extension[0];
		button1P->PC_AABB.ext[1] = button1G->modelPtr->GetOBBData().extension[1];
		button1P->PC_AABB.ext[2] = button1G->modelPtr->GetOBBData().extension[2];
		button1P->PC_OBB.ext[0] = button1P->PC_AABB.ext[0] * 2.0f;
		button1P->PC_OBB.ext[1] = button1P->PC_AABB.ext[1] * 2.0f;
		button1P->PC_OBB.ext[2] = button1P->PC_AABB.ext[2] * 2.0f;

		DirectX::XMMATRIX tempOBBPos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{ button1G->modelPtr->GetOBBData().position.x, button1G->modelPtr->GetOBBData().position.y, button1G->modelPtr->GetOBBData().position.z });
		tempOBBPos = DirectX::XMMatrixMultiply(tempOBBPos, button1G->worldMatrix);

		button1P->PC_pos = tempOBBPos.r[3];
		DirectX::XMStoreFloat3(&button1G->pos, tempOBBPos.r[3]);

		button1P->PC_OBB.ort = DirectX::XMMatrixMultiply(button1P->PC_OBB.ort, rotate);
		button1P->PC_OBB.ort = DirectX::XMMatrixTranspose(button1P->PC_OBB.ort);
		//This is where the final rotation is stored. We want to use this rotation matrix and get the extensions for the OBB
		//Create a vector describing the extension of the AABB
		//Rotate the extension according the OBB ort
		//And lastly store the result in graphics components as the model bounds used in culling
		DirectX::XMStoreFloat3(&button1G->extensions, DirectX::XMVector3Transform(DirectX::XMVectorSet(button1P->PC_OBB.ext[0], button1P->PC_OBB.ext[1], button1P->PC_OBB.ext[2], 0.0f), button1P->PC_OBB.ort));

		//Calculate the actual OBB extension
		DirectX::XMVECTOR tempRot = DirectX::XMVector3Transform(DirectX::XMVECTOR{ button1P->PC_AABB.ext[0],
			button1P->PC_AABB.ext[1] , button1P->PC_AABB.ext[2] }, rotate);
		//Use the matrix that is used to rotate the extensions as the orientation for the OBB
		button1P->PC_OBB.ort = rotate;
		button1G->ort = button1P->PC_OBB.ort;
		DirectX::XMStoreFloat3(&button1G->extensions, DirectX::XMVector3Transform(DirectX::XMVectorSet(button1P->PC_OBB.ext[0], button1P->PC_OBB.ext[1], button1P->PC_OBB.ext[2], 0.0f), button1P->PC_OBB.ort));

		button1P->PC_AABB.ext[0] = abs(tempRot.m128_f32[0]);
		button1P->PC_AABB.ext[1] = abs(tempRot.m128_f32[1]);
		button1P->PC_AABB.ext[2] = abs(tempRot.m128_f32[2]);
		tempEntity->Initialize(tempHeader.EntityID, button1P, button1G, tempHeader.interactionDistance + 2.0f, tempHeader.resetTime);
		this->m_buttonEntities.push_back(tempEntity);
	}
	//Create the levers
	for (size_t i = 0; i < data->numLever; i++)
	{
		LevelData::LeverHeader tempHeader = data->levers[i];
		tempHeader.EntityID += DEFINED_IDS::NUMMBER_OF_IDS;	//Add nummber of predefined ids to avoid conflict from editor
		LeverEntity* tempEntity = new LeverEntity();

		//Create world matrix from data
		memcpy(pos.m128_f32, tempHeader.position, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		memcpy(rot.m128_f32, tempHeader.rotation, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
																		  //Convert the useless values into proper system supported radians. Glorious.
		rot = DirectX::XMVectorSet(DirectX::XMConvertToRadians(DirectX::XMVectorGetX(rot)), DirectX::XMConvertToRadians(DirectX::XMVectorGetY(rot)), DirectX::XMConvertToRadians(DirectX::XMVectorGetZ(rot)), 1.0f);
		translate = DirectX::XMMatrixTranslationFromVector(pos);
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(rot.m128_f32[1]);
		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(rot.m128_f32[0]);
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(rot.m128_f32[2]);
		//Should just use this function instead of a bunch
		//DirectX::waa(rot);
		//Create the rotation matrix
		DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);
		//rotate    = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);

		GraphicsComponent* lever1G = m_cHandler->GetDynamicGraphicsComponent();
		lever1G->active = true;
		lever1G->modelID = tempHeader.modelID;
		lever1G->worldMatrix = DirectX::XMMatrixMultiply(rotate, translate);
		resHandler->GetModel(lever1G->modelID, lever1G->modelPtr);
		PhysicsComponent* lever1P = m_cHandler->GetPhysicsComponent();
		lever1P->PC_entityID = tempHeader.EntityID;									//Set Entity ID
		lever1P->PC_pos = pos;														//Set Position
		lever1P->PC_rotation = rot;													//Set Rotation
		lever1P->PC_is_Static = true;												//Set IsStatic
		lever1P->PC_active = true;													//Set Active
		lever1P->PC_gravityInfluence = 1.0f;
		lever1P->PC_mass = 0;
		lever1P->PC_friction = 0.00f;
		lever1P->PC_collides = true;
		lever1P->PC_elasticity = 0.2f;
		lever1P->PC_steadfast = true;
		//DirectX::XMQuaternionRotationMatrix

		//Copy the bounding volume data from the model into the physics component for reference
		lever1P->PC_AABB.ext[0] = lever1G->modelPtr->GetOBBData().extension[0];
		lever1P->PC_AABB.ext[1] = lever1G->modelPtr->GetOBBData().extension[1];
		lever1P->PC_AABB.ext[2] = lever1G->modelPtr->GetOBBData().extension[2];
		lever1P->PC_OBB.ext[0] = lever1P->PC_AABB.ext[0] * 2.0f;
		lever1P->PC_OBB.ext[1] = lever1P->PC_AABB.ext[1] * 2.0f;
		lever1P->PC_OBB.ext[2] = lever1P->PC_AABB.ext[2] * 2.0f;


		lever1P->PC_BVtype = BV_OBB;

		//Calculate the actual OBB extension
		DirectX::XMVECTOR tempRot = DirectX::XMVector3Transform(DirectX::XMVECTOR{ lever1P->PC_AABB.ext[0],
			lever1P->PC_AABB.ext[1] , lever1P->PC_AABB.ext[2] }, rotate);
		//Use the matrix that is used to rotate the extensions as the orientation for the OBB
		lever1P->PC_OBB.ort = rotate;


		lever1P->PC_AABB.ext[0] = abs(tempRot.m128_f32[0]);
		lever1P->PC_AABB.ext[1] = abs(tempRot.m128_f32[1]);
		lever1P->PC_AABB.ext[2] = abs(tempRot.m128_f32[2]);
		tempEntity->Initialize(tempHeader.EntityID, lever1P, lever1G, tempHeader.interactionDistance);
		this->m_leverEntities.push_back(tempEntity);
	}
	//Create the Wheels
	for (size_t i = 0; i < data->numWheel; i++)
	{
		LevelData::WheelHeader tempHeader = data->wheels[i];
		tempHeader.EntityID += DEFINED_IDS::NUMMBER_OF_IDS;	//Add nummber of predefined ids to avoid conflict from editor
		WheelEntity* tempEntity = new WheelEntity();

		//Create world matrix from data
		memcpy(pos.m128_f32, tempHeader.position, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		memcpy(rot.m128_f32, tempHeader.rotation, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
																		  //Convert the useless values into proper system supported radians. Glorious.
		rot = DirectX::XMVectorSet(DirectX::XMConvertToRadians(DirectX::XMVectorGetX(rot)), DirectX::XMConvertToRadians(DirectX::XMVectorGetY(rot)), DirectX::XMConvertToRadians(DirectX::XMVectorGetZ(rot)), 1.0f);
		translate = DirectX::XMMatrixTranslationFromVector(pos);
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(rot.m128_f32[1]);
		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(rot.m128_f32[0]);
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(rot.m128_f32[2]);
		//Should just use this function instead of a bunch
		//DirectX::waa(rot);
		//Create the rotation matrix
		DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);
		//rotate    = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);

		GraphicsComponent* wheel1G = m_cHandler->GetDynamicGraphicsComponent();
		wheel1G->active = true;
		wheel1G->modelID = tempHeader.modelID;
		wheel1G->worldMatrix = DirectX::XMMatrixMultiply(rotate, translate);
		resHandler->GetModel(wheel1G->modelID, wheel1G->modelPtr);
		PhysicsComponent* wheel1P = m_cHandler->GetPhysicsComponent();
		wheel1P->PC_entityID = tempHeader.EntityID;								//Set Entity ID
		wheel1P->PC_pos = pos;														//Set Position
		wheel1P->PC_rotation = rot;												//Set Rotation
		wheel1P->PC_is_Static = true;												//Set IsStatic
		wheel1P->PC_active = true;													//Set Active
		wheel1P->PC_gravityInfluence = 1.0f;
		wheel1P->PC_mass = 0;
		wheel1P->PC_friction = 0.00f;
		wheel1P->PC_collides = true;
		wheel1P->PC_elasticity = 0.2f;
		wheel1P->PC_steadfast = true;
		//DirectX::XMQuaternionRotationMatrix

		//Copy the bounding volume data from the model into the physics component for reference
		//wheel1P->PC_AABB.ext[0] = wheel1G->modelPtr->GetOBBData().extension[0];
		//wheel1P->PC_AABB.ext[1] = wheel1G->modelPtr->GetOBBData().extension[1];
		//wheel1P->PC_AABB.ext[2] = wheel1G->modelPtr->GetOBBData().extension[2];
		//wheel1P->PC_OBB.ext[0] = wheel1P->PC_AABB.ext[0];
		//wheel1P->PC_OBB.ext[1] = wheel1P->PC_AABB.ext[1];
		//wheel1P->PC_OBB.ext[2] = wheel1P->PC_AABB.ext[2];


		wheel1P->PC_BVtype = BV_OBB;
		//Copy the bounding volume data from the model into the physics component for reference
		wheel1P->PC_OBB = m_ConvertOBB(wheel1G->modelPtr->GetOBBData()); //Convert and insert OBB data
		wheel1P->PC_AABB.ext[0] = wheel1G->modelPtr->GetOBBData().extension[0];
		wheel1P->PC_AABB.ext[1] = wheel1G->modelPtr->GetOBBData().extension[1];
		wheel1P->PC_AABB.ext[2] = wheel1G->modelPtr->GetOBBData().extension[2];
		wheel1P->PC_OBB.ext[0] = wheel1P->PC_AABB.ext[0] + 0.5f ;
		wheel1P->PC_OBB.ext[1] = wheel1P->PC_AABB.ext[1];
		wheel1P->PC_OBB.ext[2] = wheel1P->PC_AABB.ext[2];

		DirectX::XMMATRIX tempOBBPos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{ wheel1G->modelPtr->GetOBBData().position.x, wheel1G->modelPtr->GetOBBData().position.y, wheel1G->modelPtr->GetOBBData().position.z });
		tempOBBPos = DirectX::XMMatrixMultiply(tempOBBPos, wheel1G->worldMatrix);

		wheel1P->PC_pos = tempOBBPos.r[3];
		DirectX::XMStoreFloat3(&wheel1G->pos, tempOBBPos.r[3]);

		wheel1P->PC_OBB.ort = DirectX::XMMatrixMultiply(wheel1P->PC_OBB.ort, rotate);
		wheel1P->PC_OBB.ort = DirectX::XMMatrixTranspose(wheel1P->PC_OBB.ort);


		//Calculate the actual OBB extension
		DirectX::XMVECTOR tempRot = DirectX::XMVector3Transform(DirectX::XMVECTOR{ wheel1P->PC_AABB.ext[0],
			wheel1P->PC_AABB.ext[1] , wheel1P->PC_AABB.ext[2] }, rotate);
		//Use the matrix that is used to rotate the extensions as the orientation for the OBB
		wheel1P->PC_OBB.ort = rotate;


		wheel1P->PC_AABB.ext[0] = abs(tempRot.m128_f32[0]);
		wheel1P->PC_AABB.ext[1] = abs(tempRot.m128_f32[1]);
		wheel1P->PC_AABB.ext[2] = abs(tempRot.m128_f32[2]);
		tempEntity->Initialize(tempHeader.EntityID, wheel1P, wheel1G, tempHeader.interactionDistance, tempHeader.min, tempHeader.max, tempHeader.time, tempHeader.resetTime > 0.0f, tempHeader.resetTime, tempHeader.resetDelay);
		this->m_wheelEntities.push_back(tempEntity);
	}
	//Create the doors
	for (size_t i = 0; i < data->numDoor; i++)
	{
		LevelData::DoorHeader tempHeader = data->doors[i];
		tempHeader.EntityID += DEFINED_IDS::NUMMBER_OF_IDS;	//Add nummber of predefined ids to avoid conflict from editor
		DoorEntity* tempEntity = new DoorEntity();

		//Create world matrix from data
		memcpy(pos.m128_f32, tempHeader.position, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		memcpy(rot.m128_f32, tempHeader.rotation, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
																		  //Convert the useless values into proper system supported radians. Glorious.
		//Convert the useless values into proper system supported radians. Glorious.
		rot = DirectX::XMVectorSet(DirectX::XMConvertToRadians(DirectX::XMVectorGetX(rot)), DirectX::XMConvertToRadians(DirectX::XMVectorGetY(rot)), DirectX::XMConvertToRadians(DirectX::XMVectorGetZ(rot)), 1.0f);
		translate = DirectX::XMMatrixTranslationFromVector(pos);
		/*DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(rot.m128_f32[1]);
		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(rot.m128_f32[0]);
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(rot.m128_f32[2]);*/
		//Should just use this function instead of a bunch

		//Create the rotation matrix
		/*DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);*/
		rotate = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMVectorGetX(rot), DirectX::XMVectorGetY(rot), DirectX::XMVectorGetZ(rot), 1.0f));
		//rotate    = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);

		GraphicsComponent* door1G = m_cHandler->GetDynamicGraphicsComponent();
		door1G->active = true;
		door1G->modelID = tempHeader.modelID;
		door1G->worldMatrix = DirectX::XMMatrixMultiply(rotate, translate);
		resHandler->GetModel(door1G->modelID, door1G->modelPtr);
		PhysicsComponent* door1P = m_cHandler->GetPhysicsComponent();
		door1P->PC_entityID = tempHeader.EntityID;			//Set Entity ID
		door1P->PC_pos = pos;								//Set Position
		door1P->PC_rotation = rot;							//Set Rotation
		door1P->PC_is_Static = false;						//Set IsStatic
		door1P->PC_active = true;							//Set Active
		door1P->PC_gravityInfluence = 1.0f;
		door1P->PC_mass = 0;
		door1P->PC_friction = 0.00f;
		door1P->PC_collides = true;
		door1P->PC_elasticity = 0.2f;
		door1P->PC_steadfast = true;
		//DirectX::XMQuaternionRotationMatrix

		//Copy the bounding volume data from the model into the physics component for reference
		door1P->PC_AABB.ext[0] = door1G->modelPtr->GetOBBData().extension[0];
		door1P->PC_AABB.ext[1] = door1G->modelPtr->GetOBBData().extension[1];
		door1P->PC_AABB.ext[2] = door1G->modelPtr->GetOBBData().extension[2];

		door1P->PC_OBB.ext[0] = door1P->PC_AABB.ext[0];
		door1P->PC_OBB.ext[1] = door1P->PC_AABB.ext[1];
		door1P->PC_OBB.ext[2] = door1P->PC_AABB.ext[2];

		door1P->PC_BVtype = BV_OBB;

		Resources::Model* modelPtr = nullptr;
		modelPtr = door1G->modelPtr;

		DirectX::XMMATRIX tempOBBPos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y
			, modelPtr->GetOBBData().position.z });
		tempOBBPos = DirectX::XMMatrixMultiply(tempOBBPos, door1G->worldMatrix);
		door1P->PC_pos = tempOBBPos.r[3];

		door1P->PC_OBB.ort = rotate;

		std::vector<ElementState> subjectStates;
		tempEntity->Initialize(tempHeader.EntityID, door1P, door1G, subjectStates, tempHeader.rotateTime);

		this->m_doorEntities.push_back(tempEntity);
	}
#pragma endregion Create puzzle entities

	//Connect puzzle entities
#pragma region
	//Connect Platforms to other things
	for (size_t i = 0; i < data->numAI; i++)
	{
		//LevelData::AiHeader tempHeader = data->aiComponents[i];
		PlatformEntity* toConnect = nullptr;
		//Find our platform and save it to our pointer(toConnect)
		for (std::vector<PlatformEntity*>::iterator observer = this->m_platformEntities.begin(); observer != this->m_platformEntities.end() && toConnect == nullptr; observer++)
		{	
			//Add nummber of predefined ids to avoid conflict from editor
			if ((*observer)->GetEntityID() == (data->aiComponents[i].EntityID + DEFINED_IDS::NUMMBER_OF_IDS))
			{
				toConnect = (*observer);
			}
		}
		//Find our connections
		for (int connectionIndex = 0; connectionIndex < data->aiComponents[i].Listener.numConnections; connectionIndex++)
		{
			//Get the ID
			//Add nummber of predefined ids to avoid conflict from editor
			unsigned int connectionID = data->aiComponents[i].Listener.SenderID[connectionIndex] + DEFINED_IDS::NUMMBER_OF_IDS;
			//Cycle through every puzzle element list until you find the connection ID
			Entity* entityToObserve = nullptr;
			bool foundConnection = false;
			for (std::vector<ButtonEntity*>::iterator other = this->m_buttonEntities.begin(); other != this->m_buttonEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
					toConnect->GetAIComponent()->AC_triggered = false;// temp - have to test the platforms to make this right
				}
			}
			for (std::vector<LeverEntity*>::iterator other = this->m_leverEntities.begin(); other != this->m_leverEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
					toConnect->GetAIComponent()->AC_triggered = false;// temp
				}
			}
			for (std::vector<WheelEntity*>::iterator other = this->m_wheelEntities.begin(); other != this->m_wheelEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
					toConnect->GetAIComponent()->AC_triggered = false;// temp
				}
			}
			for (std::vector<DoorEntity*>::iterator other = this->m_doorEntities.begin(); other != this->m_doorEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
					toConnect->GetAIComponent()->AC_triggered = false;// temp
				}
			}
		}
	}
	//Connect Doors to other things
	for (size_t i = 0; i < data->numDoor; i++)
	{
		LevelData::DoorHeader tempHeader = data->doors[i];
		tempHeader.EntityID += DEFINED_IDS::NUMMBER_OF_IDS;
		DoorEntity* toConnect = nullptr;
		//Find our door and save it in doorToConnect
		for (std::vector<DoorEntity*>::iterator observer = this->m_doorEntities.begin(); observer != this->m_doorEntities.end() && toConnect == nullptr; observer++)
		{
			if ((*observer)->GetEntityID() == tempHeader.EntityID)
			{
				toConnect = (*observer);
			}
		}
		//Find our connections
		for (int connectionIndex = 0; connectionIndex < tempHeader.Listener.numConnections; connectionIndex++)
		{
			//Get the ID
			unsigned int connectionID = tempHeader.Listener.SenderID[connectionIndex] + DEFINED_IDS::NUMMBER_OF_IDS;
			//Cycle through every puzzle element list until you find the connection ID
			Entity* entityToObserve = nullptr;
			bool foundConnection = false;
			for (std::vector<ButtonEntity*>::iterator other = this->m_buttonEntities.begin(); other != this->m_buttonEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<LeverEntity*>::iterator other = this->m_leverEntities.begin(); other != this->m_leverEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<WheelEntity*>::iterator other = this->m_wheelEntities.begin(); other != this->m_wheelEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<DoorEntity*>::iterator other = this->m_doorEntities.begin(); other != this->m_doorEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			//If we found our connection, add it to the doors list
			if (foundConnection)
			{
				toConnect->AddSubjectState(connectionID, EVENT(tempHeader.Listener.Event[connectionIndex]));
			}
		}
	}
	//Connect Buttons to other things
	for (size_t i = 0; i < data->numButton; i++)
	{
		LevelData::ButtonHeader tempHeader = data->buttons[i];
		tempHeader.EntityID += DEFINED_IDS::NUMMBER_OF_IDS;
		ButtonEntity* toConnect = nullptr;
		//Find our door and save it in doorToConnect
		for (std::vector<ButtonEntity*>::iterator observer = this->m_buttonEntities.begin(); observer != this->m_buttonEntities.end() && toConnect == nullptr; observer++)
		{
			if ((*observer)->GetEntityID() == tempHeader.EntityID)
			{
				toConnect = (*observer);
			}
		}
		//Find our connections
		for (int connectionIndex = 0; connectionIndex < tempHeader.Listener.numConnections; connectionIndex++)
		{
			//Get the ID
			unsigned int connectionID = tempHeader.Listener.SenderID[connectionIndex] + DEFINED_IDS::NUMMBER_OF_IDS;
			//Cycle through every puzzle element list until you find the connection ID
			Entity* entityToObserve = nullptr;
			bool foundConnection = false;
			for (std::vector<ButtonEntity*>::iterator other = this->m_buttonEntities.begin(); other != this->m_buttonEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<LeverEntity*>::iterator other = this->m_leverEntities.begin(); other != this->m_leverEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<WheelEntity*>::iterator other = this->m_wheelEntities.begin(); other != this->m_wheelEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<DoorEntity*>::iterator other = this->m_doorEntities.begin(); other != this->m_doorEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
		}
	}
	//Connect Levers to other things
	for (size_t i = 0; i < data->numLever; i++)
	{
		LevelData::LeverHeader tempHeader = data->levers[i];
		tempHeader.EntityID += DEFINED_IDS::NUMMBER_OF_IDS;
		LeverEntity* toConnect = nullptr;
		//Find our door and save it in doorToConnect
		for (std::vector<LeverEntity*>::iterator observer = this->m_leverEntities.begin(); observer != this->m_leverEntities.end() && toConnect == nullptr; observer++)
		{
			if ((*observer)->GetEntityID() == tempHeader.EntityID)
			{
				toConnect = (*observer);
			}
		}
		//Find our connections
		for (int connectionIndex = 0; connectionIndex < tempHeader.Listener.numConnections; connectionIndex++)
		{
			//Get the ID
			unsigned int connectionID = tempHeader.Listener.SenderID[connectionIndex] + DEFINED_IDS::NUMMBER_OF_IDS;
			//Cycle through every puzzle element list until you find the connection ID
			Entity* entityToObserve = nullptr;
			bool foundConnection = false;
			for (std::vector<ButtonEntity*>::iterator other = this->m_buttonEntities.begin(); other != this->m_buttonEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<LeverEntity*>::iterator other = this->m_leverEntities.begin(); other != this->m_leverEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<WheelEntity*>::iterator other = this->m_wheelEntities.begin(); other != this->m_wheelEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<DoorEntity*>::iterator other = this->m_doorEntities.begin(); other != this->m_doorEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
		}
	}
	//Connect Wheels to other things
	for (size_t i = 0; i < data->numWheel; i++)
	{
		LevelData::WheelHeader tempHeader = data->wheels[i];
		tempHeader.EntityID += DEFINED_IDS::NUMMBER_OF_IDS;
		WheelEntity* toConnect = nullptr;
		//Find our door and save it in doorToConnect
		for (std::vector<WheelEntity*>::iterator observer = this->m_wheelEntities.begin(); observer != this->m_wheelEntities.end() && toConnect == nullptr; observer++)
		{
			if ((*observer)->GetEntityID() == tempHeader.EntityID)
			{
				toConnect = (*observer);
			}
		}
		//Find our connections
		for (int connectionIndex = 0; connectionIndex < tempHeader.Listener.numConnections; connectionIndex++)
		{
			//Get the ID
			unsigned int connectionID = tempHeader.Listener.SenderID[connectionIndex] + DEFINED_IDS::NUMMBER_OF_IDS;
			//Cycle through every puzzle element list until you find the connection ID
			Entity* entityToObserve = nullptr;
			bool foundConnection = false;
			for (std::vector<ButtonEntity*>::iterator other = this->m_buttonEntities.begin(); other != this->m_buttonEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<LeverEntity*>::iterator other = this->m_leverEntities.begin(); other != this->m_leverEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<WheelEntity*>::iterator other = this->m_wheelEntities.begin(); other != this->m_wheelEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
			for (std::vector<DoorEntity*>::iterator other = this->m_doorEntities.begin(); other != this->m_doorEntities.end() && !foundConnection; other++)
			{
				if ((*other)->GetEntityID() == connectionID)
				{
					entityToObserve = (*other);
					entityToObserve->AddObserver(toConnect, toConnect->GetEntityID());
					foundConnection = true;
				}
			}
		}
	}
#pragma endregion Connect puzzle entities

	m_cHandler->GetPhysicsHandler()->SortComponents();
	PhysicsHandler* ptr = nullptr;
	ptr = m_cHandler->GetPhysicsHandler();
	int size = m_cHandler->GetPhysicsHandler()->GetNrOfComponents();
	int index = 0;

	for (index; index < size; index++)
	{
		PhysicsComponent* t_pc = ptr->GetComponentAt(index);
		ptr->TransferBoxesToBullet(t_pc, index);
	}
	//this->m_cHandler->GetPhysicsHandler()->SetIgnoreCollisions();
	//this->m_cHandler->GetPhysicsHandler()->GetBulletInterpreterRef()->SetIgnoreCollisions(this->m_player1.GetPhysicsComponent(), this->m_player2.GetBall()->GetPhysicsComponent());
	//this->m_cHandler->GetPhysicsHandler()->GetBulletInterpreterRef()->SetIgnoreCollisions(this->m_player2.GetPhysicsComponent(), this->m_player1.GetBall()->GetPhysicsComponent());
	//Before generating the Octree, syn the physics data with the graphics data
#pragma region 
//
//#pragma region
//	for (ButtonEntity* i : this->m_buttonEntities)
//	{
//		i->SyncComponents();
//	}
//	for (WheelEntity* i : this->m_wheelEntities)
//	{
//		i->SyncComponents();
//	}
//	for (LeverEntity* i : this->m_leverEntities)
//	{
//		i->SyncComponents();
//	}
//#pragma endregion puzzle
//
//#pragma region
	m_cHandler->GetGraphicsHandler()->GenerateOctree();
	//for (Entity* i : this->m_staticEntitys)
	//{
	//	i->SyncComponents();
	//}
	//#pragma endregion static
	//
#pragma endregion Sync components

	
#ifdef _DEBUG
	//This keeps track of any resource lib access outside of level loading. 
	Resources::ResourceHandler::GetInstance()->ResetQueryCounter();
#endif // _DEBUG




	m_cHandler->GetGraphicsHandler()->GenerateStaticSceneShadows();

#pragma region
	DirectX::XMVECTOR cubePos;
	switch (m_curLevel)
	{
	case 0: //Tutorial
		cubePos = DirectX::XMVectorSet(0.0f, 2.0f, -19.0f, 1.0f);
		break;
	case 1:
		cubePos = DirectX::XMVectorSet(8.0f, 4.0f, -2.0f, 1.0f);
		break;
	case 2:
		cubePos = DirectX::XMVectorSet(6.0f, 3.0f, -67.0f, 1.0f);
		break;
	case 3:
		cubePos = DirectX::XMVectorSet(0.0f, 2.0f, 2.0f, 1.0f);
		break;
	case 4:
		cubePos = DirectX::XMVectorSet(-11.0f, 3.0f, -12.0f, 1.0f);
		break;
	case 5:
		cubePos = DirectX::XMVectorSet(20.0f, 2.0f, 12.0f, 1.0f);
		break;
	case 6:
		cubePos = DirectX::XMVectorSet(15.0f, 5.0f, -19.0f, 1.0f);
		break;
	case 7:
		cubePos = DirectX::XMVectorSet(0.0f, 2.0f, -19.0f, 1.0f);
		break;
	default:
		cubePos = DirectX::XMVectorSet(0.0f, 2.0f, -19.0f, 1.0f);
		break;
	}
#pragma endregion Get cube map pos

	m_cHandler->GetGraphicsHandler()->GenerateSceneCubeMap(cubePos);



	return 1;
}

int LevelState::UnloadLevel()
{
	int result = 0;
	//Clear grabbing of balls
	this->m_player1.SetGrabbed(nullptr);
	this->m_networkModule->SendGrabPacket(this->m_player1.GetEntityID(), -1);
	//Clear components from GraphicsHandler.
	this->m_cHandler->ResizeGraphicsDynamic(0);
	this->m_cHandler->ResizeGraphicsStatic(0);
	//Clear internal lists
#pragma region
	for (size_t i = 0; i < this->m_staticEntitys.size(); i++)
	{
		delete this->m_staticEntitys[i];
		this->m_staticEntitys[i] = nullptr;
	}
	this->m_staticEntitys.clear();
	//Clear the puzzle entities
	for (size_t i = 0; i < this->m_doorEntities.size(); i++)
	{
		delete this->m_doorEntities[i];
		this->m_doorEntities[i] = nullptr;
	}
	this->m_doorEntities.clear();
	for (size_t i = 0; i < this->m_buttonEntities.size(); i++)
	{
		delete this->m_buttonEntities[i];
		this->m_buttonEntities[i] = nullptr;
	}
	this->m_buttonEntities.clear();
	for (size_t i = 0; i < this->m_leverEntities.size(); i++)
	{
		delete this->m_leverEntities[i];
		this->m_leverEntities[i] = nullptr;
	}
	this->m_leverEntities.clear();
	for (size_t i = 0; i < this->m_wheelEntities.size(); i++)
	{
		delete this->m_wheelEntities[i];
		this->m_wheelEntities[i] = nullptr;
	}
	this->m_wheelEntities.clear();
	for (size_t i = 0; i < this->m_fieldEntities.size(); i++)
	{
		delete this->m_fieldEntities[i];
		this->m_fieldEntities[i] = nullptr;
	}
	this->m_fieldEntities.clear();

	for (size_t i = 0; i < this->m_platformEntities.size(); i++)
	{
		delete this->m_platformEntities[i];
		this->m_platformEntities[i] = nullptr;
	}
	this->m_platformEntities.clear();
#pragma endregion Clear entity lists
	//In order to correctly load the components into the physics handler we need to flush the old ones because the Active variable doesn't work according to Axel.
	//Shutdown PhysicsHandler and initialize it again.
#pragma region
	PhysicsHandler* pHandler = this->m_cHandler->GetPhysicsHandler();

	pHandler->ClearPhysicsHandler();
#pragma endregion Physics handler restart

	this->m_director.Initialize();
#pragma region
#pragma region
	//We then need to recreate the persistent components here
	PhysicsComponent* playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = DEFINED_IDS::PLAYER_1;	//Set Entity ID
	playerP->PC_pos = DirectX::XMVectorSet(0, 2, 0, 0);			//Set Position (Will be set in createLevel)
	playerP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	playerP->PC_is_Static = false;								//Set IsStatic							
	playerP->PC_mass = 10;
	playerP->PC_BVtype = BV_OBB;

	//Should be done
	/*playerP->PC_OBB.ext[0] = playerG->modelPtr->GetOBBData().extension[0];
	playerP->PC_OBB.ext[1] = playerG->modelPtr->GetOBBData().extension[1];
	playerP->PC_OBB.ext[2] = playerG->modelPtr->GetOBBData().extension[2];*/
	playerP->PC_OBB.ext[0] = this->m_player1.GetGraphicComponent()->modelPtr->GetOBBData().extension[0];
	playerP->PC_OBB.ext[1] = this->m_player1.GetGraphicComponent()->modelPtr->GetOBBData().extension[1];
	playerP->PC_OBB.ext[2] = this->m_player1.GetGraphicComponent()->modelPtr->GetOBBData().extension[2];

	playerP->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	playerP->PC_friction = 1.0f;
	this->m_player1.SetPhysicsComponent(playerP);

	//reset player1 animation component to idle animation
	this->m_player1.GetAnimationComponent()->previousState = this->m_player1.GetAnimationComponent()->currentState;
	this->m_player1.SetAnimationComponent(PLAYER_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true, false, 1.0f, 1.0f);
	this->m_player1.GetAnimationComponent()->currentState = PLAYER_IDLE;

#pragma endregion Player 1
#pragma region
	//We then need to recreate the persistent components here
	playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = DEFINED_IDS::PLAYER_2;	//Set Entity ID
	playerP->PC_pos = DirectX::XMVectorSet(0, 2, 0, 0);			//Set Position (Will be set in createLevel)
	playerP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	playerP->PC_is_Static = false;								//Set IsStatic							
	playerP->PC_mass = 10;
	playerP->PC_BVtype = BV_OBB;

	//Should be done
	/*playerP->PC_OBB.ext[0] = playerG->modelPtr->GetOBBData().extension[0];
	playerP->PC_OBB.ext[1] = playerG->modelPtr->GetOBBData().extension[1];
	playerP->PC_OBB.ext[2] = playerG->modelPtr->GetOBBData().extension[2];*/
	playerP->PC_OBB.ext[0] = this->m_player2.GetGraphicComponent()->modelPtr->GetOBBData().extension[0];
	playerP->PC_OBB.ext[1] = this->m_player2.GetGraphicComponent()->modelPtr->GetOBBData().extension[1];
	playerP->PC_OBB.ext[2] = this->m_player2.GetGraphicComponent()->modelPtr->GetOBBData().extension[2];

	playerP->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	playerP->PC_friction = 1.0f;
	this->m_player2.SetPhysicsComponent(playerP);

	//reset player2 animation component to idle animation
	this->m_player2.GetAnimationComponent()->previousState = this->m_player2.GetAnimationComponent()->currentState;
	this->m_player2.SetAnimationComponent(PLAYER_IDLE, 0.50f, Blending::SMOOTH_TRANSITION, true, false, 1.0f, 1.0f);
	this->m_player2.GetAnimationComponent()->currentState = PLAYER_IDLE;


#pragma endregion Player 2
#pragma region 
	PhysicsComponent* ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = DEFINED_IDS::BALL_1;									//Set Entity ID
	ballP->PC_pos = { 0 };									//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	ballP->PC_rotationVelocity = DirectX::XMVectorSet(0, 0, 0, 0);
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_Sphere;
	ballP->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	ballP->PC_OBB.ext[0] = 0.5f;
	ballP->PC_OBB.ext[1] = 0.5f;
	ballP->PC_OBB.ext[2] = 0.5f;
	ballP->PC_Sphere.radius = 0.25;
	ballP->PC_mass = 50;
	//We do not know the position of the ball in our dynamic components list. We need to flush this list too btw.
	this->m_player1.GetBall()->SetPhysicsComponent(ballP);

#pragma endregion ball1
#pragma region
	ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = DEFINED_IDS::BALL_2;									//Set Entity ID
	ballP->PC_pos = { 0 };									//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_Sphere;
	ballP->PC_Sphere.radius = 0.25;
	ballP->PC_OBB.ext[0] = 0.5f;
	ballP->PC_OBB.ext[1] = 0.5f;
	ballP->PC_OBB.ext[2] = 0.5f;
	ballP->PC_mass = 50;
	//We do not know the position of the ball in our dynamic components list. We need to flush this list too btw.
	this->m_player2.GetBall()->SetPhysicsComponent(ballP);
#pragma endregion ball2
#pragma endregion 
	//We have a special case with the dynamic entities, save the balls and Re-insert them into the dynamic list
	DynamicEntity* ball1 = nullptr;
	DynamicEntity* ball2 = nullptr;
	ball1 = static_cast<DynamicEntity*>(this->m_player1.GetBall());
	ball1->SyncComponents();
	ball2 = static_cast<DynamicEntity*>(this->m_player2.GetBall());
	ball2->SyncComponents();
	//Sync components to make sure the values in the graphics components are somewhat sane



	// Clear the dynamic entities. Don't delete the balls
	for (size_t i = 0; i < this->m_dynamicEntitys.size(); i++)
	{
		if (this->m_dynamicEntitys[i] != ball1 && this->m_dynamicEntitys[i] != ball2)
		{
			delete this->m_dynamicEntitys[i];
			this->m_dynamicEntitys[i] = nullptr;
		}
	}
	this->m_dynamicEntitys.clear();



	//Re-introduce them into our dynamic list
	this->m_dynamicEntitys.push_back(ball1);
	this->m_dynamicEntitys.push_back(ball2);

	this->m_Player1ChainPhysicsComp.clear();
	this->m_Player2ChainPhysicsComp.clear();

	Resources::ResourceHandler::GetInstance()->UnloadCurrentLevel();

	return 1;
}

int LevelState::LoadNext()
{
	int result = 0;
	//Increase the level count and reset the cleared level flag
	this->m_curLevel++;
	this->m_clearedLevel = 0;

	if (this->m_curLevel >= this->m_levelPaths.size())
	{
		//If the code comes here it means that the user finished the last level
		//Behavior will be to start the first level
		//Next behavior is to pop ourselves and go back to the menu
		//The last behavior is to pop ourselves and push a Credit state
		this->m_curLevel = 0;
		this->m_networkModule->SendFlagPacket(PacketTypes::DISCONNECT_REQUEST);
		this->m_cHandler->RemoveLastUIComponent();
		this->m_cHandler->RemoveLastUIComponent();
		this->m_gsh->PopStateFromStack();
		CreditState* creditState = new CreditState();
		int result = creditState->Initialize(this->m_gsh, this->m_cHandler, this->m_cameraRef);
		if (result > 0)
		{
			this->m_gsh->PushStateToStack(creditState);
		}
		else
		{
			delete creditState;
			creditState = nullptr;
		}
	}
	else
	{
	Resources::Status st = Resources::Status::ST_OK;
	std::string path = this->m_levelPaths.at(this->m_curLevel).levelPath;

	//We also need to clear the internal lists, lets have another function do that
	this->UnloadLevel();

	LevelData::Level* level;    //pointer for resourcehandler data. This data is actually stored in the file loader so don't delete it.
	path = this->m_levelPaths.at(this->m_curLevel).levelPath;

	//Begin by clearing the current level data by calling UnloadLevel.
	//Cheat and use the singletons for ResourceHandler, FileLoader, LightHandler
#pragma region
	printf("LOAD LEVEL %d\n", this->m_curLevel);
	//Load LevelData from file
	st = Resources::FileLoader::GetInstance()->LoadLevel(path, level);
	//if not successful
	if (st != Resources::ST_OK)
	{
		//Error loading file.
		printf("ERROR message: %s -  Error occcured: %s!", "Failed loading file!", "In LevelState::LoadNext()");
		return -1;
	}
	//Load Resources of the level
	st = Resources::ResourceHandler::GetInstance()->LoadLevel(level->resources, level->numResources);
	//if not successful
	if (st != Resources::ST_OK)
	{
		//Error loading level from resource handler.
		printf("ERROR message: %s -  Error occcured: %s!", "Failed loading level!", "In LevelState::LoadNext()");
		return -2;
	}

	//Load Lights of the level

	if (!LIGHTING::LightHandler::GetInstance()->LoadLevelLight(level))
	{
		//Error loading lights through LightHandler.
		printf("ERROR message: %s -  Error occcured: %s!", "Failed loading lights!", "In LevelState::LoadNext()");
		return -3;
	}
#pragma endregion Loading data


#pragma region
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 1.4f, 0.0f, 0.0f);

	m_cameraRef->SetCameraPivot(
		&this->m_cHandler->GetPhysicsHandler()->GetComponentAt(0)->PC_pos,
		targetOffset,
		1.3f
	);

#pragma endregion Set_Camera

	//Call the CreateLevel with the level data.
	result = this->CreateLevel(level);
	}
	
	return 1;
}

int LevelState::GetLevelIndex()
{
	return this->m_curLevel;
}

std::string LevelState::GetLevelPath()
{
	return this->m_levelPaths.at(min(this->m_levelPaths.size() - 1, this->m_curLevel)).levelPath;
}

void LevelState::SetCurrentLevelID(int currentLevelID)
{
	this->m_curLevel = min(currentLevelID, int(this->m_levelPaths.size() - 1));
}

int LevelState::EnterState()
{
	return 0;
}

int LevelState::LeaveState()
{
	this->m_networkModule->SendFlagPacket(PacketTypes::DISCONNECT_REQUEST);

	return 0;
}

void LevelState::UpdateGraphicalLinks()
{
	DirectX::XMVECTOR temp = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, 3, 0));
	GraphicalLink* lastComp = nullptr;
	DirectX::XMVECTOR lastPos;
	DirectX::XMVECTOR diffVec;
	DirectX::XMVECTOR par; // ony used for storing uneeded value
	DirectX::XMVECTOR per;

	for (size_t i = 0; i < this->m_grapichalLinkListPlayer1.size(); i++)
	{
		float t = (float)i / (float)this->m_grapichalLinkListPlayer1.size();
		DirectX::XMVECTOR pos = this->GetInterpolatedSplinePoint(t, &this->m_Player1ChainPhysicsComp);

		//Set rot before setting pos

		this->m_grapichalLinkListPlayer1.at(i).SetPos(pos);
		if (i != 0 && i < this->m_grapichalLinkListPlayer1.size())
		{
			lastComp = &this->m_grapichalLinkListPlayer1.at(i - 1);
			lastPos = DirectX::XMLoadFloat3(&lastComp->m_pos);
			diffVec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(pos, lastPos));	//Towards current

			lastComp->m_rotMat.r[0] = DirectX::XMVectorSetW(diffVec, 0.0f);
			DirectX::XMVector3ComponentsFromNormal(&par, &per, lastComp->m_rotMat.r[2], diffVec);
			per = DirectX::XMVector3Normalize(per);
			lastComp->m_rotMat.r[2] = DirectX::XMVectorSetW(per, 0.0f);
			lastComp->m_rotMat.r[1] = DirectX::XMVectorSetW(DirectX::XMVector3Cross(diffVec, per), 0.0f);
		}

	}
	for (size_t i = 0; i < this->m_grapichalLinkListPlayer2.size(); i++)
	{
		float t = (float)i / (float)this->m_grapichalLinkListPlayer2.size();
		DirectX::XMVECTOR pos = this->GetInterpolatedSplinePoint(t, &this->m_Player2ChainPhysicsComp);


		this->m_grapichalLinkListPlayer2.at(i).SetPos(pos);
		if (i != 0 && i < this->m_grapichalLinkListPlayer2.size())
		{
			lastComp = &this->m_grapichalLinkListPlayer2.at(i - 1);

			lastPos = DirectX::XMLoadFloat3(&lastComp->m_pos);
			diffVec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(pos, lastPos));	//Towards current
			lastComp->m_rotMat.r[0] = DirectX::XMVectorSetW(diffVec, 0.0f);
			DirectX::XMVector3ComponentsFromNormal(&par, &per, lastComp->m_rotMat.r[1], diffVec);
			per = DirectX::XMVector3Normalize(per);
			lastComp->m_rotMat.r[1] = DirectX::XMVectorSetW(per, 0.0f);
			lastComp->m_rotMat.r[2] = DirectX::XMVectorSetW(DirectX::XMVector3Cross(diffVec, per), 0.0f);
		}
	}
}
DirectX::XMVECTOR LevelState::GetInterpolatedSplinePoint(float t, std::vector<PhysicsComponent*>*list)
{
	this->delta_t = 1.f / (float)list->size();

	int p = int((t / this->delta_t));

#define BOUNDS(pp){ if (pp < 0) pp = int(0); else if(pp >= (int)list->size()-1)pp = int(list->size()-1);}
	int p0 = p - 1;			BOUNDS(p0);
	int p1 = p; 			BOUNDS(p1);
	int p2 = p + 1;			BOUNDS(p2);
	int p3 = p + 2;			BOUNDS(p3);

	float lt = (t - delta_t*(float)p) / delta_t;

	DirectX::XMVECTOR pos = DirectX::XMVectorCatmullRom(list->at(p0)->PC_pos,
		list->at(p1)->PC_pos,
		list->at(p2)->PC_pos,
		list->at(p3)->PC_pos,
		lt);

	return pos;
}