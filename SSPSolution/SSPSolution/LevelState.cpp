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
	// Clear the dynamic entitys
	for (size_t i = 0; i < this->m_dynamicEntitys.size(); i++)
	{
		delete this->m_dynamicEntitys[i];
		this->m_dynamicEntitys[i] = nullptr;
	}
	
	// Clear the static entitys
	for (size_t i = 0; i < this->m_staticEntitys.size(); i++)
	{
		delete this->m_staticEntitys[i];
		this->m_staticEntitys[i] = nullptr;
	}

	// Clear level director
	this->m_director.Shutdown();
	
	return result;
}

int LevelState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 1;
	result = GameState::InitializeBase(gsh, cHandler, cameraRef);
	Resources::ResourceHandler* resHandler = Resources::ResourceHandler::GetInstance();

	// creating the player
	this->m_player1 = Player();
	GraphicsComponent* playerG = m_cHandler->GetGraphicsComponent();
	playerG->modelID = 1337;
	playerG->active = true;
	resHandler->GetModel(playerG->modelID, playerG->modelPtr);
	PhysicsComponent* playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = 0;								//Set Entity ID
	playerP->PC_pos = DirectX::XMVectorSet(20, 20, 20, 0);		//Set Position
	playerP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	playerP->PC_is_Static = false;							//Set IsStatic
	playerP->PC_active = true;								//Set Active
	playerP->PC_mass = 5;
	playerP->PC_BVtype = BV_AABB;
	playerP->PC_AABB.ext[0] = 0.5;
	playerP->PC_AABB.ext[1] = 0.5;
	playerP->PC_AABB.ext[2] = 0.5;
	playerG->worldMatrix = DirectX::XMMatrixIdentity();		//FIX THIS
	this->m_player1.Initialize(0, playerP, playerG);

	//Player 2
	this->m_player2 = Player();
	playerG = m_cHandler->GetGraphicsComponent();
	playerG->modelID = 1337;
	playerG->active = true;
	resHandler->GetModel(playerG->modelID, playerG->modelPtr);
	playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = 1;								//Set Entity ID
															//playerP->PC_pos = DirectX::XMVectorSet(0, -100, 0, 0);		//Set Position
	playerP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	playerP->PC_is_Static = false;							//Set IsStatic
	playerP->PC_active = true;								//Set Active
	playerP->PC_mass = 5;
	playerP->PC_BVtype = BV_AABB;
	playerP->PC_AABB.ext[0] = 0.5;
	playerP->PC_AABB.ext[1] = 0.5;
	playerP->PC_AABB.ext[2] = 0.5;
	playerG->worldMatrix = DirectX::XMMatrixIdentity();		//FIX THIS
	this->m_player2.Initialize(3, playerP, playerG);
	
	//this->m_dynamicEntitys.push_back();
	//creating the ball
	DynamicEntity* ball = new DynamicEntity();
	GraphicsComponent* ballG = m_cHandler->GetGraphicsComponent();
	ballG->modelID = 1337;
	ballG->active = true;
	resHandler->GetModel(ballG->modelID, ballG->modelPtr);
	PhysicsComponent* ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = 2;									//Set Entity ID
	ballP->PC_pos = DirectX::XMVectorSet(10, 5, 0, 0);		//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_AABB;
	ballP->PC_AABB.ext[0] = 0.5;
	ballP->PC_AABB.ext[1] = 0.5;
	ballP->PC_AABB.ext[2] = 0.5;
	ballP->PC_mass = 1;
	ballG->worldMatrix = DirectX::XMMatrixIdentity();
	ball->Initialize(1, ballP, ballG);
	this->m_dynamicEntitys.push_back(ball);

	//Entity* ptr = (Entity*)ball;
	//this->m_player1.SetGrabbed(ball);

	//this->m_cHandler->GetPhysicsHandler()->CreateChainLink(1, 0, 10, 2);

	DynamicEntity* platform = new DynamicEntity();
	GraphicsComponent* platformG = m_cHandler->GetGraphicsComponent();
	platformG->modelID = 1337;
	platformG->active = true;
	resHandler->GetModel(platformG->modelID, platformG->modelPtr);
	PhysicsComponent* platformP = m_cHandler->GetPhysicsComponent();
	platformP->PC_pos = DirectX::XMVectorSet(-3, 1, -40, 0);
	platformG->worldMatrix = DirectX::XMMatrixTranslationFromVector(platformP->PC_pos);
	platformP->PC_is_Static = true;
	platformP->PC_AABB.ext[0] = 5;
	platformP->PC_AABB.ext[1] = 0.1f;
	platformP->PC_AABB.ext[2] = 5;
	AIComponent* platformTERMINATOR = m_cHandler->GetAIComponent();
#pragma region AIComp variables
	platformTERMINATOR->AC_active = true;
	// entityID resolved in initialise down below

	platformTERMINATOR->AC_triggered = true;
	platformTERMINATOR->AC_time = 0;

	platformTERMINATOR->AC_speed = 0.15f;
	// AC_dir resolved in update loop

	platformTERMINATOR->AC_position = platformP->PC_pos;
	platformTERMINATOR->AC_pattern = 2;// Circular
	platformTERMINATOR->AC_direction = 0;
	platformTERMINATOR->AC_latestWaypointID = 0;
	platformTERMINATOR->AC_nextWaypointID = 1;
	platformTERMINATOR->AC_nrOfWaypoint = 4;
	platformTERMINATOR->AC_waypoints[0] = platformP->PC_pos;
	platformTERMINATOR->AC_waypoints[1] = DirectX::XMVectorSet(-3, 1, 0, 0);
	platformTERMINATOR->AC_waypoints[2] = DirectX::XMVectorSet(-3, 15, 0, 0);
	platformTERMINATOR->AC_waypoints[3] = DirectX::XMVectorSet(-3, 15, -40, 0);
#pragma endregion

	platform->Initialize(3, platformP, platformG, platformTERMINATOR);
	platformP->PC_entityID = platform->GetEntityID();
	platformTERMINATOR->AC_entityID = platform->GetEntityID();
	this->m_dynamicEntitys.push_back(platform);

	//this->m_cameraRef->SetCameraPivot(this->m_player1.GetPhysicsComponent()->PC_pos, 10);
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0, 3.0, 0.0, 0.0);
	
	if (this->m_networkModule->IsHost())
	{
		m_cameraRef->SetCameraPivot(
		&this->m_cHandler->GetPhysicsHandler()->GetDynamicComponentAt(0)->PC_pos,
		targetOffset,
		10.0f
		);
	}
	else // Player 2
	{
		m_cameraRef->SetCameraPivot(
			&this->m_cHandler->GetPhysicsHandler()->GetDynamicComponentAt(1)->PC_pos,
			targetOffset,
			10.0f
		);
	}

	
	this->m_director.Initialize();

	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;
	dt = 1000000 / dt;

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
			for (itr = this->m_entityPacketList.begin(); itr != this->m_entityPacketList.end(); itr++)
			{

				if (itr->entityID == -1)	//TEMP HARDCODED PLAYER1 TO SEND ID -1, REMOVE WHEN PLAYER IS IN A LIST
				{
					PhysicsComponent* pp = this->m_player1.GetPhysicsComponent();

					// Update the component
					pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
					pp->PC_rotation = DirectX::XMLoadFloat3(&itr->newRotation);
					pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);
				}
				else if (itr->entityID == -2)
				{
					PhysicsComponent* pp = this->m_player2.GetPhysicsComponent();

					// Update the component
					pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
					pp->PC_rotation = DirectX::XMLoadFloat3(&itr->newRotation);
					pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);
				}
				else
				{
					// Find the entity
					DynamicEntity* ent = this->m_dynamicEntitys.at(itr->entityID);	// The entity identified by the ID sent from the other client
					PhysicsComponent* pp = ent->GetPhysicsComponent();

					// Update the component
					pp->PC_pos = DirectX::XMLoadFloat3(&itr->newPos);
					pp->PC_rotation = DirectX::XMLoadFloat3(&itr->newRotation);
					pp->PC_velocity = DirectX::XMLoadFloat3(&itr->newVelocity);
				}
			}
		}
		this->m_entityPacketList.clear();	//Clear the list
	}
#pragma endregion Network_update_entities

	float yaw = inputHandler->GetMouseDelta().x;
	float pitch = inputHandler->GetMouseDelta().y;
	float mouseSens = 0.000018f * dt;

	if (inputHandler->GetMouseDelta().y || inputHandler->GetMouseDelta().x)
		this->m_cameraRef->RotateCameraPivot(inputHandler->GetMouseDelta().y * mouseSens, inputHandler->GetMouseDelta().x * mouseSens);

	

	//update player for throw functionallity
	DirectX::XMVECTOR playerLookDir = DirectX::XMVector4Normalize( DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->m_cameraRef->GetLookAt()), DirectX::XMLoadFloat3(&this->m_cameraRef->GetCameraPos())));
	DirectX::XMFLOAT3 temp;

	this->m_cameraRef->GetCameraUp(temp);
	
	DirectX::XMVECTOR upDir = DirectX::XMLoadFloat3(&temp);
	DirectX::XMVECTOR rightDir = m_cameraRef->GetRight(); //DirectX::XMVector3Cross(upDir, playerLookDir);

	if (this->m_networkModule->IsHost())
	{	
		this->m_player1.SetRightDir(rightDir);
		this->m_player1.SetUpDir(upDir);
		this->m_player1.SetLookDir(playerLookDir);
		this->m_player1.Update(dt, inputHandler);
		this->m_player2.SyncComponents();
	}
	else
	{
		this->m_player2.SetRightDir(rightDir);
		this->m_player2.SetUpDir(upDir);
		this->m_player2.SetLookDir(playerLookDir);
		this->m_player2.Update(dt, inputHandler);
		this->m_player1.SyncComponents();
	}
	

	if ( (this->m_networkModule->IsHost() == true) && (this->m_networkModule->GetNrOfConnectedClients() != 0) )	//Player is host and there is connected clients
	{
		PhysicsComponent* pp = this->m_player1.GetPhysicsComponent();
		this->m_networkModule->SendEntityUpdatePacket(-1, pp->PC_pos, pp->PC_velocity, pp->PC_rotation);	//Send the update data for only player
	}
	else if( (this->m_networkModule->IsHost() == false) && (this->m_networkModule->GetNrOfConnectedClients() != 0) )
	{
		PhysicsComponent* pp = this->m_player2.GetPhysicsComponent();
		this->m_networkModule->SendEntityUpdatePacket(-2, pp->PC_pos, pp->PC_velocity, pp->PC_rotation);	//Send the update data for only player
	}

	if (inputHandler->IsKeyPressed(SDL_SCANCODE_G))
	{
		this->m_player1.SetGrabbed(this->m_dynamicEntitys.at(0));
	}
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_H))
	{
		this->m_player1.SetGrabbed(nullptr);
	}

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

	if (this->m_player1.GetIsAming()) //Might actualy already be set to this
	{
		this->m_player1.SetLookDir(this->m_cameraRef->GetDirection());
	}

	//update all dynamic (moving) entities
	for (int i = 0; i < this->m_dynamicEntitys.size(); i++)
	{
		this->m_dynamicEntitys.at(i)->Update(dt, inputHandler);
	}

	for (int i = 0; i < this->m_staticEntitys.size(); i++)
	{
		this->m_staticEntitys.at(i)->Update(dt, inputHandler);
	}
	//this->LockCameraToPlayer();

	// Reactionary level director acts
	this->m_director.Update(dt);

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
				DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0, 3.0, 0.0, 0.0);
				m_cameraRef->SetCameraPivot(
					&this->m_cHandler->GetPhysicsHandler()->GetDynamicComponentAt(1)->PC_pos,
					targetOffset,
					10.0f
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

	return result;
}

int LevelState::CreateLevel(LevelData::Level * data)
{
	DirectX::XMVECTOR rot;
	DirectX::XMVECTOR pos;
	rot.m128_f32[3] = 0.0f;	//Set w to 0
	pos.m128_f32[3] = 0.0f;	//Set w to 0

	DirectX::XMMATRIX translate;
	DirectX::XMMATRIX rotate;
	Resources::Model* modelPtr;
	Resources::Status st = Resources::ST_OK;
	Resources::ResourceHandler* resHandler = Resources::ResourceHandler::GetInstance();

	


	m_player1_Spawn = DirectX::XMVectorSet( //Store spawnPoint for player 1
		data->spawns[0].position[0],
		data->spawns[0].position[1],
		data->spawns[0].position[2],
		0);

	m_player2_Spawn = DirectX::XMVectorSet(	//Store spawnPoint for player 2
		data->spawns[1].position[0],
		data->spawns[1].position[1],
		data->spawns[1].position[2],
		0);



	m_player1.GetPhysicsComponent()->PC_pos = m_player1_Spawn;

	for (size_t i = 0; i < data->numEntities; i++)
	{
		LevelData::EntityHeader* currEntity = &data->entities[i]; //Current entity

		//Create GraphicsComponent
		GraphicsComponent* t_gc = m_cHandler->GetGraphicsComponent();
		t_gc->modelID			= currEntity->modelID;
		t_gc->active			= true;
		resHandler->GetModel(t_gc->modelID, t_gc->modelPtr); //Get and apply a pointer to the model
		//Create world matrix from data
		memcpy(pos.m128_f32, currEntity->position, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		memcpy(rot.m128_f32, currEntity->rotation, sizeof(float) * 3);	  //Convert from POD to DirectX Vector
		translate = DirectX::XMMatrixTranslationFromVector(pos);
		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rot.m128_f32[0]));
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rot.m128_f32[1]));
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rot.m128_f32[2]));
		//Create the rotation matrix
		DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);
		//rotate    = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
		t_gc->worldMatrix = DirectX::XMMatrixMultiply(rotate,translate);

		//Create Physics component
		PhysicsComponent* t_pc = m_cHandler->GetPhysicsComponent();
		t_pc->PC_entityID	   = currEntity->EntityID;		//Set Entity ID
		t_pc->PC_pos		   = pos;						//Set Position
		t_pc->PC_rotation	   = rot;						//Set Rotation
		t_pc->PC_is_Static	   = currEntity->isStatic;		//Set IsStatic
		t_pc->PC_active		   = true;						//Set Active




		st = Resources::ResourceHandler::GetInstance()->GetModel(currEntity->modelID, modelPtr);

		//get information from file
		t_pc->PC_BVtype = BV_AABB;

		t_pc->PC_AABB.ext[0] = modelPtr->GetOBBData().extension[0];
		t_pc->PC_AABB.ext[1] = modelPtr->GetOBBData().extension[1];
		t_pc->PC_AABB.ext[2] = modelPtr->GetOBBData().extension[2];

		DirectX::XMVECTOR tempRot = DirectX::XMVector3Transform(DirectX::XMVECTOR{ t_pc->PC_AABB.ext[0],
			t_pc->PC_AABB.ext[1] , t_pc->PC_AABB.ext[2] }, rotate);

		t_pc->PC_AABB.ext[0] = abs(tempRot.m128_f32[0]);
		t_pc->PC_AABB.ext[1] = abs(tempRot.m128_f32[1]);
		t_pc->PC_AABB.ext[2] = abs(tempRot.m128_f32[2]);

		/*DirectX::XMMATRIX tempRot = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{ t_pc->PC_AABB.ext[0],
			t_pc->PC_AABB.ext[1] , t_pc->PC_AABB.ext[2] });
		tempRot = tempRot*rotate;

		t_pc->PC_AABB.ext[0] = abs(tempRot.r[3].m128_f32[0]);
		t_pc->PC_AABB.ext[1] = abs(tempRot.r[3].m128_f32[1]);
		t_pc->PC_AABB.ext[2] = abs(tempRot.r[3].m128_f32[2]);*/

		t_pc->PC_friction = 1.0f;
#ifdef _DEBUG
		if (st != Resources::ST_OK)
			std::cout << "Model could not be found when loading level data,  ID: " << currEntity->modelID << std::endl;
#endif // _DEBUG

		t_pc->PC_OBB = m_ConvertOBB( modelPtr->GetOBBData()); //Convert and insert OBB data
	
		//t_pc->PC_OBB.ort = DirectX::XMMatrixMultiply(t_pc->PC_OBB.ort, rotate);



		if (t_pc->PC_is_Static) {
			StaticEntity* tse = new StaticEntity();
			tse->SetGraphicsComponent(t_gc);
			tse->SetPhysicsComponent(t_pc);
			this->m_staticEntitys.push_back(tse); //Push new entity to list
		}
		else {
			
			DynamicEntity* tde = new DynamicEntity();
			tde->SetGraphicsComponent(t_gc);
			tde->SetPhysicsComponent(t_pc);
			this->m_dynamicEntitys.push_back(tde); //Push new entity to list
			
		}

	}

	return 1;
}

void LevelState::LockCameraToPlayer()
{
	DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&this->m_cameraRef->GetCameraPos());
	DirectX::XMVECTOR camLookAt = DirectX::XMLoadFloat3(&this->m_cameraRef->GetLookAt());
	PhysicsComponent* player = nullptr;

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(camLookAt, camPos);

	player = this->m_cHandler->GetPhysicsHandler()->GetDynamicComponentAt(0);

	camPos = DirectX::XMVectorAdd(player->PC_pos, DirectX::XMVectorScale(diffVec, -3));
	camPos = DirectX::XMVectorAdd(camPos, DirectX::XMVectorSet(0, 3, 0, 0));
	camLookAt = DirectX::XMVectorAdd(camPos, diffVec);

	this->m_cameraRef->SetCameraPos(camPos);
	this->m_cameraRef->SetLookAt(camLookAt);

	//this->m_physicsHandler.ApplyForceToComponent(player, DirectX::XMVectorSet(translateCameraX, translateCameraY, translateCameraZ, 0), 1.0f);
}
