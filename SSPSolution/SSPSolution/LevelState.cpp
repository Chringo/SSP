#include "LevelState.h"

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
	float closestDistance = 9999999999;
	PhysicsComponent* pc = this->m_player1.GetPhysicsComponent();
	
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


	return closest;
}

LevelState::LevelState()
{
}

LevelState::~LevelState()
{
	ShutDown();
}

int LevelState::ShutDown()
{
	int result = 1;
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

	return result;
}

int LevelState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 1;
	result = GameState::InitializeBase(gsh, cHandler, cameraRef);
	Resources::ResourceHandler* resHandler = Resources::ResourceHandler::GetInstance();
	this->m_cHandler->GetGraphicsHandler()->ResizeDynamicComponents(2);
	this->m_cHandler->ResizeGraphicsPersistent(4);

	#pragma region
	this->m_player1 = Player();
	GraphicsComponent* playerG = m_cHandler->GetGraphicsAnimationComponent();
	playerG->modelID = 1117267500;
	playerG->active = true;
	resHandler->GetModel(playerG->modelID, playerG->modelPtr);
	PhysicsComponent* playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = 1;	//Set Entity ID
	playerP->PC_pos = DirectX::XMVectorSet(0, 0, 0, 0);			//Set Position (Will be set in createLevel)
	playerP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	playerP->PC_is_Static = false;								//Set IsStatic							
	playerP->PC_mass = 5;
	playerP->PC_BVtype = BV_OBB;

	/*playerP->PC_OBB.ext[0] = playerG->modelPtr->GetOBBData().extension[0];
	playerP->PC_OBB.ext[1] = playerG->modelPtr->GetOBBData().extension[1];
	playerP->PC_OBB.ext[2] = playerG->modelPtr->GetOBBData().extension[2];*/

	playerP->PC_OBB.ext[0] = 0.5f;
	playerP->PC_OBB.ext[1] = 0.5f;
	playerP->PC_OBB.ext[2] = 0.5f;

	playerP->PC_velocity = DirectX::XMVectorSet(0,0,0,0);
	playerP->PC_friction = 3.5f;
	playerG->worldMatrix = DirectX::XMMatrixIdentity();		//FIX THIS

	/*TEMP ANIM STUFF*/
	#pragma region
	AnimationComponent* playerAnim1 = nullptr;

	((GraphicsAnimationComponent*)playerG)->jointCount = playerG->modelPtr->GetSkeleton()->GetSkeletonData()->jointCount;

	playerAnim1 = m_cHandler->GetAnimationComponent();

	playerAnim1->skeleton = playerG->modelPtr->GetSkeleton();
	/*If the graphics component have a skeleton for the player, assign the joints and animations to the animation component.*/
	if (playerAnim1->skeleton != nullptr)
	{
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
			playerAnim1->playingSpeed = 2.0f;
		}
	}
	/*If the graphics component don't have a skeleton, set the source state to ERROR state.*/
	else
	{
		playerAnim1->source_State->stateIndex = ANIMATION_ERROR;
	}
	
#pragma endregion Animation_Player1

	this->m_player1.Initialize(playerP->PC_entityID, playerP, playerG, playerAnim1);
	this->m_player1.SetMaxSpeed(5.0f);
	this->m_player1.SetAcceleration(26.0f);
	this->m_cHandler->GetPhysicsHandler()->ApplyPlayer1ToBullet(playerP);
	#pragma endregion Player1
	
	#pragma region
	this->m_player2 = Player();
	playerG = m_cHandler->GetGraphicsAnimationComponent();
	playerG->modelID = 1117267500;
	playerG->active = true;
	resHandler->GetModel(playerG->modelID, playerG->modelPtr);
	playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = 2;	//Set Entity ID
	playerP->PC_pos = { 0 };								//Set Position
	playerP->PC_is_Static = false;							//Set IsStatic
	playerP->PC_active = true;								//Set Active
	playerP->PC_mass = 5;
	playerP->PC_velocity = DirectX::XMVectorSet(0,0,0,0);
	playerP->PC_BVtype = BV_OBB;
	playerP->PC_OBB.ext[0] = 0.5f;
	playerP->PC_OBB.ext[1] = 0.5f;
	playerP->PC_OBB.ext[2] = 0.5f;
	playerG->worldMatrix = DirectX::XMMatrixIdentity();		//FIX THIS
														
	/*TEMP ANIM STUFF*/
	#pragma region
	AnimationComponent* playerAnim2 = nullptr;

	((GraphicsAnimationComponent*)playerG)->jointCount = playerG->modelPtr->GetSkeleton()->GetSkeletonData()->jointCount;

	playerAnim2 = m_cHandler->GetAnimationComponent();

	playerAnim2->skeleton = playerG->modelPtr->GetSkeleton();
	/*If the graphics component have a skeleton for the player, assign the joints and animations to the animation component.*/
	if (playerAnim2->skeleton != nullptr)
	{
		playerAnim2->active = 1;
		for (int i = 0; i < ((GraphicsAnimationComponent*)playerG)->jointCount; i++)
		{
			((GraphicsAnimationComponent*)playerG)->finalJointTransforms[i] = DirectX::XMMatrixIdentity();
		}

		if (playerG->modelPtr->GetSkeleton()->GetNumAnimations() > 0)
		{
			int numAnimations = playerG->modelPtr->GetSkeleton()->GetNumAnimations();

			playerAnim2->animation_States = playerG->modelPtr->GetSkeleton()->GetAllAnimations();

			playerAnim2->source_State = playerAnim1->animation_States->at(0)->GetAnimationStateData();
			playerAnim2->source_State->isLooping = true;
			playerAnim2->playingSpeed = 2.0f;
		}
	}
	/*If the graphics component don't have a skeleton, set the source state to ERROR state.*/
	else
	{
		playerAnim2->source_State->stateIndex = ANIMATION_ERROR;
	}
	#pragma endregion Animation_Player2

	this->m_player2.Initialize(playerP->PC_entityID, playerP, playerG, playerAnim2);
	this->m_player2.SetMaxSpeed(2.0f);
	this->m_player2.SetAcceleration(0.5f);


	#pragma endregion Player2

	#pragma region
	////Ball1
	DynamicEntity* ball = new DynamicEntity();
	GraphicsComponent* ballG = m_cHandler->GetPersistentGraphicsComponent();
	ballG->modelID = 1321651915;
	ballG->active = true;
	resHandler->GetModel(ballG->modelID, ballG->modelPtr);
	PhysicsComponent* ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = 3;									//Set Entity ID
	ballP->PC_pos = { 0 };									//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_OBB;
	ballP->PC_OBB.ext[0] = 0.5f;
	ballP->PC_OBB.ext[1] = 0.5f;
	ballP->PC_OBB.ext[2] = 0.5f;
	ballP->PC_mass = 10;
	ballG->worldMatrix = DirectX::XMMatrixIdentity();
	ball->Initialize(3, ballP, ballG);
	this->m_dynamicEntitys.push_back(ball);
	m_player1.SetBall(ball);
	#pragma endregion Ball1

	#pragma region
	////Ball2
	DynamicEntity* ball2 = new DynamicEntity();
	ballG = m_cHandler->GetPersistentGraphicsComponent();
	ballG->modelID = 1321651915;
	ballG->active = true;
	resHandler->GetModel(ballG->modelID, ballG->modelPtr);
	ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = 4;									//Set Entity ID
	ballP->PC_pos = { 0 };									//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_OBB;
	ballP->PC_OBB.ext[0] = 0.5;
	ballP->PC_OBB.ext[1] = 0.5;
	ballP->PC_OBB.ext[2] = 0.5;
	ballP->PC_mass = 10;
	ballG->worldMatrix = DirectX::XMMatrixIdentity();
	ball2->Initialize(4, ballP, ballG);
	this->m_dynamicEntitys.push_back(ball2);
	m_player2.SetBall(ball2);
	#pragma endregion Ball2

	#pragma region
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 1.4f, 0.0f, 0.0f);

	m_cameraRef->SetCameraPivot(
		&this->m_cHandler->GetPhysicsHandler()->GetDynamicComponentAt(0)->PC_pos,
		targetOffset,
		1.3f
	);

#pragma endregion Set_Camera

	this->m_director.Initialize();

	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;
	dt = dt / 1000000;

	this->m_networkModule->Update();

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

					if ((int)itr->entityID == 1)	//Packets for player2
					{
						pp = this->m_player2.GetPhysicsComponent();

						// Update the component
						pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
						pp->PC_rotation = DirectX::XMLoadFloat3(&itr->newRotation);
						pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);

					}
					else if ((int)itr->entityID == 2)	//Packets for player1
					{
						pp = this->m_player1.GetPhysicsComponent();

						// Update the component
						pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
						pp->PC_rotation = DirectX::XMLoadFloat3(&itr->newRotation);
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
								pp->PC_rotation = DirectX::XMLoadFloat3(&itr->newRotation);
								pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);

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
		float yaw = inputHandler->GetMouseDelta().x;
		float pitch = inputHandler->GetMouseDelta().y;
		float mouseSens = 0.1f * dt;

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
	#pragma endregion Update/Syncing

	#pragma region
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_G))
		{
			Entity* closestBall = this->GetClosestBall(3);
			
			if (closestBall != nullptr)	//If a ball was found
			{				
				this->m_player1.SetGrabbed(closestBall);
				this->m_networkModule->SendGrabPacket(this->m_player1.GetEntityID(), closestBall->GetEntityID());	
			}

		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_H))
		{
			this->m_player1.SetGrabbed(nullptr);
			this->m_networkModule->SendGrabPacket(this->m_player1.GetEntityID(), -1);
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
						this->m_player2.SetGrabbed(this->m_player1.GetBall());
					}
					else if (itr->grabbedID == 4)
					{
						this->m_player2.SetGrabbed(this->m_player2.GetBall());
					}

				}
				else //Drop
				{
					this->m_player2.SetGrabbed(nullptr);
				}

			}
		}
		this->m_grabPacketList.clear();
	#pragma endregion Grab_Requests

	#pragma region
		//Aming for player1 (SHOULD BE FOR THE CONTROLED PLAYER)
		if (inputHandler->IsMouseKeyPressed(SDL_BUTTON_RIGHT) && !this->m_player1.GetIsAming())
		{
			this->m_player1.SetAiming(true);
			this->m_cameraRef->SetDistance(2);
		}

		if (inputHandler->IsMouseKeyReleased(SDL_BUTTON_RIGHT) && this->m_player1.GetIsAming())
		{
			this->m_player1.SetAiming(false);
			this->m_cameraRef->SetDistance(10);
		}

		if (this->m_player1.GetIsAming())
		{
			this->m_player1.SetLookDir(this->m_cameraRef->GetDirection());
		}
	#pragma endregion Aiming

	#pragma region

		if (this->m_networkModule->GetNrOfConnectedClients() != 0)	//There is connected players
		{
			PhysicsComponent* pp = this->m_player1.GetPhysicsComponent();
			if (pp != nullptr)
			{
				this->m_networkModule->SendEntityUpdatePacket(pp->PC_entityID, pp->PC_pos, pp->PC_velocity, pp->PC_rotation);	//Send the update data for the player

				Entity* ent = nullptr;
				for (size_t i = 0; i < this->m_dynamicEntitys.size(); i++)	//Change start and end with physics packet
				{
					ent = this->m_dynamicEntitys.at(i);

					if (ent != this->m_player2.GetGrabbed())	//If it is not grabbed by player2
					{
						pp = this->m_dynamicEntitys.at(i)->GetPhysicsComponent();
						this->m_networkModule->SendEntityUpdatePacket(pp->PC_entityID, pp->PC_pos, pp->PC_velocity, pp->PC_rotation);	//Send the update
					}
				}
			}

		}

	#pragma endregion Network_Send_Updates

	#pragma region
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_T))
		{
			// Reset player-position to spawn
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
			// Iterate through chainlink list to reset velocity and position of players, chain links, and balls
			//this->m_cHandler->GetPhysicsHandler()->ResetChainLink();
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_Y))
		{
			//TODO: NOCLIP BOOOOIS
			if (inputHandler->IsKeyDown(SDL_SCANCODE_LSHIFT))
			{
				//m_player1.GetPhysicsComponent()->PC_gravityInfluence = 1;
				m_player1.GetPhysicsComponent()->PC_Bullet_AffectedByGravity = true;
				m_player1.GetPhysicsComponent()->PC_steadfast = false;
			}
			else
			{
				//m_player1.GetPhysicsComponent()->PC_gravityInfluence = 0;
				m_player1.GetPhysicsComponent()->PC_Bullet_AffectedByGravity = false;
				m_player1.GetPhysicsComponent()->PC_steadfast = true;
			}
			//Result: Sloppy teleport :(
			m_player1.GetPhysicsComponent()->PC_pos = DirectX::XMVectorAdd(
				m_player1.GetPhysicsComponent()->PC_pos,
				(DirectX::XMVectorScale(m_player1.GetLookDir(), 3.0f)));
		}

		//Update all puzzle entities
	#pragma region
		//Commong variables needed for logic checks
		DirectX::XMFLOAT3 playerPos;
		DirectX::XMStoreFloat3(&playerPos, this->m_player1.GetPhysicsComponent()->PC_pos);
		//Buttons and levers require input for logical evaluation of activation
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_R))
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
		if (inputHandler->IsKeyDown(SDL_SCANCODE_R))
		{

			int increasing = (inputHandler->IsKeyDown(SDL_SCANCODE_LSHIFT)) ? -1 : 1;	//Only uses addition but branches, kind of
			//increasing = -1 + (!inputHandler->IsKeyDown(SDL_SCANCODE_LSHIFT)) * 2;		//No branching calculation, but uses multiplication and addition

			for (std::vector<WheelEntity*>::iterator i = this->m_wheelEntities.begin(); i != this->m_wheelEntities.end(); i++)
			{
				(*i)->CheckPlayerInteraction(playerPos, increasing);
			}
		}
		else if (inputHandler->IsKeyReleased(SDL_SCANCODE_R))
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
		//Wheels require updates to rotate based on state calculated in CheckPlayerInteraction
		for (std::vector<WheelEntity*>::iterator i = this->m_wheelEntities.begin(); i != this->m_wheelEntities.end(); i++)
		{
			(*i)->Update(dt, inputHandler);
		}
		//Doors require updates to change opening state
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
		//Lock the camera to the player

		for (size_t i = 0; i < this->m_platformEntities.size(); i++)
		{
			this->m_platformEntities[i]->Update(dt, inputHandler);
		}
	#pragma endregion Update_Puzzle_Elements

	this->m_cHandler->GetPhysicsHandler()->CheckFieldIntersection();
	for (size_t i = 0; i < m_fieldEntities.size(); i++)
	{
		m_fieldEntities[i]->Update(dt, inputHandler);
	}
	// Reactionary level director acts
	this->m_director.Update(dt);

	#pragma region
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_M))
		{
			SoundHandler::instance().PlaySound2D(Sounds2D::MENU1, false, false);
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_N))
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, this->m_player2.GetPhysicsComponent()->PC_pos);
			SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_CHAIN_DRAG_1, pos, true, false);
		}
	#pragma endregion MUSIC_KEYS

	#pragma region
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_J))
		{
			if (this->m_networkModule->GetNrOfConnectedClients() <= 0)	//If the network module is NOT connected to other clients
			{
				if (this->m_networkModule->Join(this->m_ip))				//If we succsefully connected
				{
					printf("Joined client with the ip %s\n", this->m_ip);

					//TEMP SULOTION
					//Move the camera to player 2 since we joined a game 
					DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 1.4f, 0.0f, 0.0f);
					m_cameraRef->SetCameraPivot(
						&this->m_cHandler->GetPhysicsHandler()->GetDynamicComponentAt(1)->PC_pos,
						targetOffset,
						1.3f
					);
				}
				else
				{
					printf("Failed to connect to the client %s\n", this->m_ip);
				}

			}
			else
			{
				printf("Join failed since this module is already connected to other clients\n");
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_K))
		{
			this->m_networkModule->SendFlagPacket(DISCONNECT_REQUEST);
		}
	#pragma endregion Network_Key_events

	this->m_cameraRef->Update(dt);

	//Update the listner pos and direction for sound
	DirectX::XMFLOAT3 dir;
	DirectX::XMStoreFloat3(&dir, this->m_cameraRef->GetDirection());
	DirectX::XMFLOAT3 up;
	this->m_cameraRef->GetCameraUp(up);
	SoundHandler::instance().UpdateListnerPos(this->m_cameraRef->GetCameraPos(), dir, up);

	return result;
}

int LevelState::CreateLevel(LevelData::Level * data)
{
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
	staticEntityCount += data->numButton;
	staticEntityCount += data->numLever;
	staticEntityCount += data->numWheel;
	dynamicEntityCount += data->numDoor;

	this->m_cHandler->ResizeGraphicsStatic(staticEntityCount);
	this->m_cHandler->ResizeGraphicsDynamic(dynamicEntityCount);





	DirectX::XMVECTOR rot;
	DirectX::XMVECTOR pos;
	rot.m128_f32[3] = 0.0f;	//Set w to 0
	pos.m128_f32[3] = 0.0f;	//Set w to 0

	DirectX::XMMATRIX translate;
	DirectX::XMMATRIX rotate;
	Resources::Model* modelPtr;
	Resources::Status st = Resources::ST_OK;
	Resources::ResourceHandler* resHandler = Resources::ResourceHandler::GetInstance();

	std::vector<DynamicEntity*> aiEntities;

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
		this->m_player2.GetPhysicsComponent()->PC_pos = this->m_player2_Spawn;
	}
	else
	{
		this->m_player1.GetPhysicsComponent()->PC_pos = this->m_player2_Spawn;
		this->m_player2.GetPhysicsComponent()->PC_pos = this->m_player1_Spawn;
	}

	this->m_player1.GetBall()->GetPhysicsComponent()->PC_pos =
		DirectX::XMVectorAdd(
			m_player1.GetPhysicsComponent()->PC_pos, DirectX::XMVectorSet(2, 1, 2, 0));
	
	this->m_player2.GetBall()->GetPhysicsComponent()->PC_pos =
		DirectX::XMVectorAdd(
			this->m_player2.GetPhysicsComponent()->PC_pos, DirectX::XMVectorSet(2, 1, 2, 0));
	//this->m_cHandler->GetPhysicsHandler()->CreateChainLink(this->m_player1.GetPhysicsComponent(), m_player1.GetBall()->GetPhysicsComponent(), 5, 1.0);
	//this->m_cHandler->GetPhysicsHandler()->CreateChainLink(this->m_player2.GetPhysicsComponent(), m_player2.GetBall()->GetPhysicsComponent(), 5, 1.0);

	for (size_t i = 0; i < data->numEntities; i++)
	{
		LevelData::EntityHeader* currEntity = &data->entities[i]; //Current entity
		GraphicsComponent* t_gc;
		Resources::Model * modelPtr;

		/*AnimationComponent* t_anim = nullptr;*/

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
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rot.m128_f32[1]));
		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rot.m128_f32[0]));
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rot.m128_f32[2]));
		//Create the rotation matrix
		
		DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);
		//rotate    = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
		t_gc->worldMatrix = DirectX::XMMatrixMultiply(rotate, translate);

		//Create Physics component
		PhysicsComponent* t_pc = m_cHandler->GetPhysicsComponent();
		t_pc->PC_entityID	   = currEntity->EntityID;		//Set Entity ID
		//t_pc->PC_pos		   = pos;						//Set Position
		t_pc->PC_rotation	   = rot;						//Set Rotation
		t_pc->PC_is_Static	   = currEntity->isStatic;		//Set IsStatic
		t_pc->PC_active		   = true;						//Set Active
		t_pc->PC_BVtype = BV_OBB;
		//t_pc->PC_OBB.ort = DirectX::XMMatrixMultiply(t_pc->PC_OBB.ort, rotate);
		st = Resources::ResourceHandler::GetInstance()->GetModel(currEntity->modelID, modelPtr);

		DirectX::XMMATRIX tempOBBPos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y
			, modelPtr->GetOBBData().position.z });
		tempOBBPos = DirectX::XMMatrixMultiply(tempOBBPos, t_gc->worldMatrix);

		t_pc->PC_pos = tempOBBPos.r[3];
		//t_pc->PC_pos.m128_f32[3] = 1.0f;

		//get information from file
		//static components should have the mass of 0
		t_pc->PC_mass = 0;
		t_pc->PC_friction = 0.5f;
#ifdef _DEBUG
		if (st != Resources::ST_OK)
			std::cout << "Model could not be found when loading level data,  ID: " << currEntity->modelID << std::endl;
#endif // _DEBUG

		t_pc->PC_OBB = m_ConvertOBB( modelPtr->GetOBBData()); //Convert and insert OBB data
	
		t_pc->PC_OBB.ort = DirectX::XMMatrixMultiply(t_pc->PC_OBB.ort, rotate);
		t_pc->PC_OBB.ort = DirectX::XMMatrixTranspose(t_pc->PC_OBB.ort);

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
	for (size_t i = 0; i < data->numAI; i++)
	{
		AIComponent* t_ac = m_cHandler->GetAIComponent();
		t_ac->AC_triggered = true;// Temp: Needed for AIHandler->Update()
		t_ac->AC_entityID = data->aiComponents[i].EntityID;
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
		t_pc->PC_pos = DirectX::XMVectorAdd(t_ac->AC_position, DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y, modelPtr->GetOBBData().position.z,0 });
		t_pc->PC_entityID = data->aiComponents[i].EntityID;
		t_pc->PC_is_Static = false;
		t_pc->PC_steadfast = true;
		t_pc->PC_gravityInfluence = 0;
		t_pc->PC_friction = 0.7f;
		t_pc->PC_elasticity = 0.1f;
		t_pc->PC_BVtype = BV_AABB;
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

		//DirectX::XMVECTOR hejsan = DirectX::XMVectorAdd(t_ac->AC_position, DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y, modelPtr->GetOBBData().position.z,0 });
		//DirectX::XMVECTOR obbPos = DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y, modelPtr->GetOBBData().position.z,1 };
		////t_pc->PC_pos = DirectX::XMVectorAdd(t_ac->AC_position, DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y, modelPtr->GetOBBData().position.z,0 });
		//DirectX::XMVectorSubtract(t_ac->AC_position, DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y, modelPtr->GetOBBData().position.z,0 });
		//DirectX::XMMATRIX tempPos = t_pc->PC_OBB.ort;
		//tempPos.r[3] = obbPos;
		//DirectX::XMMATRIX tempBPos = DirectX::XMMatrixTranslationFromVector(t_ac->AC_position);
		//tempPos = tempPos*tempBPos;

		/*t_pc->PC_OBB.ort.r[3] = t_ac->AC_position;
		t_pc->PC_OBB.ort.r[3].m128_f32[3] = 1.0f;*/
		//t_pc->PC_OBB.ort.r[3] = hejsan;
		//t_pc->PC_OBB.ort.r[3] = DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x*-1, modelPtr->GetOBBData().position.y*-1, modelPtr->GetOBBData().position.z*-1,1 };
		//t_pc->PC_pos = hejsan;

		/* DANGER ZONE */
		//t_pc->PC_OBB.ort.r[3] = DirectX::XMVECTOR{ modelPtr->GetOBBData().position.x, modelPtr->GetOBBData().position.y, modelPtr->GetOBBData().position.z, 1.0f };
#pragma endregion

		PlatformEntity* tpe = new PlatformEntity();
		tpe->Initialize(t_pc->PC_entityID, t_pc, t_gc, t_ac);
		this->m_platformEntities.push_back(tpe);
	}
	
#pragma region Creating Field
	//for (size_t i = 0; i < data->numCheckpoints; i++)
	//{
	//	OBB* tOBB = new OBB();
	//	memcpy(&tOBB->ort, &static_cast<DirectX::XMMATRIX>(data->checkpoints[i].ort), sizeof(float) * 16);
	//	memcpy(&tOBB->ext, data->checkpoints[i].ext, sizeof(float) * 3);
	//	DirectX::XMVECTOR tPos = {
	//		data->checkpoints[i].position[0],
	//		data->checkpoints[i].position[1],
	//		data->checkpoints[i].position[2]
	//	};
	//	Field* tempField = this->m_cHandler->GetPhysicsHandler()->CreateField(
	//		tPos,
	//		1,	//EntityID Player1
	//		3,	//Temporary checking ball (entityID: 3) for Player1 as if it was Player2
	//		tOBB
	//	);
	//	FieldEntity* tempFE = new FieldEntity();
	//	tempFE->Initialize(data->checkpoints[i].entityID, tempField);
	//	this->m_fieldEntities.push_back(tempFE);
	//	this->m_fieldEntities[i]->AddObserver(&this->m_director, this->m_director.GetID());
	//	delete tOBB;
	//}

	// TODO: Field Data for States in Level Director
	/*for (size_t k = 0; k < this->m_director.GetNrOfStates(); k++)
	{
		for (size_t i = 0; i < this->m_currentState->nrOfFields; i++)
		{
			this->m_currentState->fieldMap[i].FD_entityID = nullptr;
			this->m_currentState->fieldMap[i].FD_first_inside = nullptr;
			this->m_currentState->fieldMap[i].FD_second_inside = nullptr;
		}
	}*/

#pragma endregion

	//Create the PuzzleElements
#pragma region
	//Create the Buttons
	for (size_t i = 0; i < data->numButton; i++)
	{
		LevelData::ButtonHeader tempHeader = data->buttons[i];
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

		GraphicsComponent* button1G = m_cHandler->GetStaticGraphicsComponent();
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

		//Copy the bounding volume data from the model into the physics component for reference
		button1P->PC_AABB.ext[0] = button1G->modelPtr->GetOBBData().extension[0];
		button1P->PC_AABB.ext[1] = button1G->modelPtr->GetOBBData().extension[1];
		button1P->PC_AABB.ext[2] = button1G->modelPtr->GetOBBData().extension[2];
		button1P->PC_OBB.ext[0] = button1P->PC_AABB.ext[0] * 2.0f;
		button1P->PC_OBB.ext[1] = button1P->PC_AABB.ext[1] * 2.0f;
		button1P->PC_OBB.ext[2] = button1P->PC_AABB.ext[2] * 2.0f;


		button1P->PC_BVtype = BV_OBB;
		//Check for rotation, if found then set the bounding volume to OBB
		float ignoreIfLess = 0.0000000001f;
		if (abs(tempHeader.rotation[0]) > ignoreIfLess || abs(tempHeader.rotation[1]) > ignoreIfLess || abs(tempHeader.rotation[2]) > ignoreIfLess)
		{
			//There is a rotation
			button1P->PC_BVtype = BV_OBB;
		}

		//Calculate the actual OBB extension
		DirectX::XMVECTOR tempRot = DirectX::XMVector3Transform(DirectX::XMVECTOR{ button1P->PC_AABB.ext[0],
			button1P->PC_AABB.ext[1] , button1P->PC_AABB.ext[2] }, rotate);
		//Use the matrix that is used to rotate the extensions as the orientation for the OBB
		button1P->PC_OBB.ort = rotate;


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

		GraphicsComponent* lever1G = m_cHandler->GetStaticGraphicsComponent();
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

		GraphicsComponent* wheel1G = m_cHandler->GetStaticGraphicsComponent();
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
		wheel1P->PC_AABB.ext[0] = wheel1G->modelPtr->GetOBBData().extension[0];
		wheel1P->PC_AABB.ext[1] = wheel1G->modelPtr->GetOBBData().extension[1];
		wheel1P->PC_AABB.ext[2] = wheel1G->modelPtr->GetOBBData().extension[2];
		wheel1P->PC_OBB.ext[0] = wheel1P->PC_AABB.ext[0] * 2.0f;
		wheel1P->PC_OBB.ext[1] = wheel1P->PC_AABB.ext[1] * 2.0f;
		wheel1P->PC_OBB.ext[2] = wheel1P->PC_AABB.ext[2] * 2.0f;


		wheel1P->PC_BVtype = BV_OBB;
		//Check for rotation, if found then set the bounding volume to OBB
		float ignoreIfLess = 0.0000000001f;
		if (abs(tempHeader.rotation[0]) > ignoreIfLess || abs(tempHeader.rotation[1]) > ignoreIfLess || abs(tempHeader.rotation[2]) > ignoreIfLess)
		{
			//There is a rotation
			wheel1P->PC_BVtype = BV_OBB;
		}

		//Calculate the actual OBB extension
		DirectX::XMVECTOR tempRot = DirectX::XMVector3Transform(DirectX::XMVECTOR{ wheel1P->PC_AABB.ext[0],
			wheel1P->PC_AABB.ext[1] , wheel1P->PC_AABB.ext[2] }, rotate);
		//Use the matrix that is used to rotate the extensions as the orientation for the OBB
		wheel1P->PC_OBB.ort = rotate;


		wheel1P->PC_AABB.ext[0] = abs(tempRot.m128_f32[0]);
		wheel1P->PC_AABB.ext[1] = abs(tempRot.m128_f32[1]);
		wheel1P->PC_AABB.ext[2] = abs(tempRot.m128_f32[2]);
		tempEntity->Initialize(tempHeader.EntityID, wheel1P, wheel1G, tempHeader.interactionDistance, tempHeader.min, tempHeader.max, tempHeader.time, tempHeader.resetTime < 0.0f, tempHeader.resetTime, tempHeader.resetDelay);
		this->m_wheelEntities.push_back(tempEntity);
	}
	//Create the doors
	for (size_t i = 0; i < data->numDoor; i++)
	{
		LevelData::DoorHeader tempHeader = data->doors[i];
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
		door1P->PC_entityID = tempHeader.EntityID;								//Set Entity ID
		door1P->PC_pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorSet(0,2,2,1));														//Set Position
		door1P->PC_rotation = rot;												//Set Rotation
		door1P->PC_is_Static = false;												//Set IsStatic
		door1P->PC_active = true;													//Set Active
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
		door1P->PC_Power = 1338;

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
			if ((*observer)->GetEntityID() == data->aiComponents[i].EntityID)
			{
				toConnect = (*observer);
			}
		}
		//Find our connections
		for (int connectionIndex = 0; connectionIndex < data->aiComponents[i].Listener.numConnections; connectionIndex++)
		{
			//Get the ID
			unsigned int connectionID = data->aiComponents[i].Listener.SenderID[connectionIndex];
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
			unsigned int connectionID = tempHeader.Listener.SenderID[connectionIndex];
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
			unsigned int connectionID = tempHeader.Listener.SenderID[connectionIndex];
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
			unsigned int connectionID = tempHeader.Listener.SenderID[connectionIndex];
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
			unsigned int connectionID = tempHeader.Listener.SenderID[connectionIndex];
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
		PhysicsComponent* t_pc = ptr->GetDynamicComponentAt(index);
		ptr->TransferBoxesToBullet(t_pc, index);
	}
	
	size = this->m_doorEntities.size();
	//for (int i = 0; i < size; i++)
	//{
	//	PhysicsComponent* door;
	//	
	//	//this->m_dynamicEntitys.push_back
	//	door = this->m_doorEntities.at(i)->GetPhysicsComponent();
	//	ptr->TransferBoxesToBullet(door, index);
	//	//this->m_player1.GetPhysicsComponent()->PC_pos = door->PC_pos;
	//}

	m_cHandler->GetGraphicsHandler()->GenerateOctree();

	return 1;
}

