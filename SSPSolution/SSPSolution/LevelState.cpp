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
	for (size_t i = 0; i < this->m_wheelEntities.size(); i++)
	{
		delete this->m_wheelEntities[i];
		this->m_wheelEntities[i] = nullptr;
	}
	this->m_wheelEntities.clear();
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
	playerP->PC_pos = DirectX::XMVectorSet(0, 5, 0, 0);		//Set Position
	playerP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	playerP->PC_is_Static = false;							//Set IsStatic
	playerP->PC_active = true;								//Set Active
	playerP->PC_mass = 5;
	playerP->PC_BVtype = BV_AABB;
	playerP->PC_AABB.ext[0] = 1.5;
	playerP->PC_AABB.ext[1] = 1.5;
	playerP->PC_AABB.ext[2] = 1.5;
	playerG->worldMatrix = DirectX::XMMatrixIdentity();		//FIX THIS
	this->m_player1.Initialize(0, playerP, playerG);

	//creating the ball
	DynamicEntity* ball = new DynamicEntity();
	GraphicsComponent* ballG = m_cHandler->GetGraphicsComponent();
	ballG->modelID = 1337;
	ballG->active = true;
	resHandler->GetModel(ballG->modelID, ballG->modelPtr);
	PhysicsComponent* ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = 1;									//Set Entity ID
	ballP->PC_pos = DirectX::XMVectorSet(10, 5, 0, 0);		//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_AABB;
	ballP->PC_AABB.ext[0] = 1.5;
	ballP->PC_AABB.ext[1] = 1.5;
	ballP->PC_AABB.ext[2] = 1.5;
	ballP->PC_mass = 10;
	ballG->worldMatrix = DirectX::XMMatrixIdentity();
	ball->Initialize(1, ballP, ballG);
	this->m_dynamicEntitys.push_back(ball);

	Entity* ptr = (Entity*)ball;
	this->m_player1.SetGrabbed(ball);

	this->m_cHandler->GetPhysicsHandler()->CreateChainLink(1, 0, 10, 2);

	StaticEntity* golv = new StaticEntity();
	GraphicsComponent* golvG = m_cHandler->GetGraphicsComponent();
	golvG->modelID = 1337;
	golvG->active = true;
	resHandler->GetModel(golvG->modelID, golvG->modelPtr);
	PhysicsComponent* golvP = m_cHandler->GetPhysicsComponent();
	golvP->PC_entityID = 1;								//Set Entity ID
	golvP->PC_pos = DirectX::XMVectorSet(0, 0, 0, 0);		//Set Position
	golvP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	golvP->PC_is_Static = true;							//Set IsStatic
	golvP->PC_active = true;								//Set Active
	golvP->PC_BVtype = BV_Plane;
	golvP->PC_Plane.PC_normal = DirectX::XMVectorSet(0, 1, 0, 0);
	golvP->PC_OBB.ort = DirectX::XMMatrixIdentity();
	golvP->PC_friction = 0.9;
	golvG->worldMatrix = DirectX::XMMatrixIdentity();
	golv->Initialize(2, golvP, golvG);
	this->m_staticEntitys.push_back(golv);
	
	
	this->m_director.Initialize();

	WheelEntity* wheel1 = new WheelEntity();
	DoorEntity* door1 = new DoorEntity();

	//DOOR
	GraphicsComponent* door1G = m_cHandler->GetGraphicsComponent();
	door1G->modelID = 1337;
	door1G->active = true;
	door1G->worldMatrix = DirectX::XMMatrixIdentity();		
	resHandler->GetModel(door1G->modelID, door1G->modelPtr);
	PhysicsComponent* door1P = m_cHandler->GetPhysicsComponent();
	door1P->PC_entityID = 666;								//Set Entity ID
	door1P->PC_pos = DirectX::XMVectorSet(0, 1, 5, 0);		//Set Position
	door1P->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	door1P->PC_is_Static = true;							//Set IsStatic
	door1P->PC_active = true;								//Set Active
	door1P->PC_mass = 5;
	door1P->PC_BVtype = BV_AABB;
	door1P->PC_AABB.ext[0] = 1.5;
	door1P->PC_AABB.ext[1] = 1.5;
	door1P->PC_AABB.ext[2] = 1.5;
	door1->Initialize(666, door1P, door1G, 0.0f, DirectX::XM_PI / 2.1f, 1.0f);

	//BUTTON
	GraphicsComponent* wheel1G = m_cHandler->GetGraphicsComponent();
	wheel1G->modelID = 1337;
	wheel1G->active = true;
	wheel1G->worldMatrix = DirectX::XMMatrixIdentity();		
	resHandler->GetModel(wheel1G->modelID, wheel1G->modelPtr);
	PhysicsComponent* wheel1P = m_cHandler->GetPhysicsComponent();
	wheel1P->PC_entityID = 616;									//Set Entity ID
	wheel1P->PC_pos = DirectX::XMVectorSet(0, 2, 8, 0);		//Set Position
	wheel1P->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);	//Set Rotation
	wheel1P->PC_is_Static = true;								//Set IsStatic
	wheel1P->PC_active = true;									//Set Active
	wheel1P->PC_mass = 5;
	wheel1P->PC_BVtype = BV_AABB;
	wheel1P->PC_AABB.ext[0] = 1.5;
	wheel1P->PC_AABB.ext[1] = 1.5;
	wheel1P->PC_AABB.ext[2] = 1.5;
	wheel1->Initialize(616, wheel1P, wheel1G);
	wheel1->AddObserver(door1, door1->GetEntityID());
	this->m_wheelEntities.push_back(wheel1);

	this->m_doorEntities.push_back(door1);

	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;
	dt = dt / 1000000;
	//this->m_player1.Update(dt, inputHandler);

	//for (size_t i = 0; i < m_entities.size(); i++)
	//{
	//	if (i == 0)
	//	{
	//		Player* ptr = (Player*)this->m_entities.at(i);
	//		DirectX::XMVECTOR lookDir = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->m_cameraRef->GetLookAt()), DirectX::XMLoadFloat3(&this->m_cameraRef->GetCameraPos()));
	//		DirectX::XMFLOAT3 temp;
	//		this->m_cameraRef->GetCameraUp(temp);

	//		DirectX::XMVECTOR upDir = DirectX::XMLoadFloat3(&temp);
	//		DirectX::XMVECTOR rightDir = DirectX::XMVector3Cross(upDir, lookDir);

	//		ptr->SetLookDir(lookDir);
	//		ptr->SetRightDir(rightDir);
	//		ptr->SetUpDir(upDir);
	//	}

	//	this->m_entities.at(i)->Update(dt, inputHandler);
	//}

#pragma region
	//update player for throw functionallity
	DirectX::XMVECTOR playerLookDir = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->m_cameraRef->GetLookAt()), DirectX::XMLoadFloat3(&this->m_cameraRef->GetCameraPos()));
	DirectX::XMFLOAT3 temp;

	this->m_cameraRef->GetCameraUp(temp);
	
	DirectX::XMVECTOR upDir = DirectX::XMLoadFloat3(&temp);
	DirectX::XMVECTOR rightDir = DirectX::XMVector3Cross(upDir, playerLookDir);

	this->m_player1.SetRightDir(rightDir);
	this->m_player1.SetUpDir(upDir);
	this->m_player1.SetLookDir(playerLookDir);
	this->m_player1.Update(dt, inputHandler);
#pragma endregion temp player throw functionality

	//update all dynamic entities
	for (int i = 0; i < this->m_dynamicEntitys.size(); i++)
	{
		this->m_dynamicEntitys.at(i)->Update(dt, inputHandler);
	}

	//Update all puzzle entities
	//Buttons require input for logical evaluation
	if (inputHandler->IsKeyPressed(SDL_SCANCODE_R))
	{
		for (std::vector<ButtonEntity*>::iterator i = this->m_buttonEntities.begin(); i != this->m_buttonEntities.end(); i++)
		{
			DirectX::XMFLOAT3 playerPos;
			DirectX::XMStoreFloat3(&playerPos, this->m_player1.GetPhysicsComponent()->PC_pos);

			(*i)->CheckPressed(playerPos);
		}
		bool increasing = !inputHandler->IsKeyDown(SDL_SCANCODE_LSHIFT);
		for (std::vector<WheelEntity*>::iterator i = this->m_wheelEntities.begin(); i != this->m_wheelEntities.end(); i++)
		{
			DirectX::XMFLOAT3 playerPos;
			DirectX::XMStoreFloat3(&playerPos, this->m_player1.GetPhysicsComponent()->PC_pos);

			(*i)->CheckPlayerInteraction(playerPos, increasing);
		}
		
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
		if ((*i)->GetIsOpened())
		{
			printf("America Fuck yea!");
		}
	}
	//Lock the camera to the player
	this->LockCameraToPlayer();

	// Reactionary level director acts
	this->m_director.Update(dt);

	return result;
}

int LevelState::CreateLevel(LevelData::Level * data)
{
	DirectX::XMVECTOR rot;
	DirectX::XMVECTOR pos;
	DirectX::XMMATRIX translate;
	DirectX::XMMATRIX rotate;
	Resources::Model* modelPtr;
	Resources::Status st = Resources::ST_OK;
	Resources::ResourceHandler* resHandler = Resources::ResourceHandler::GetInstance();

	//Entity* player = new Player();
	//GraphicsComponent* playerG = m_cHandler->GetGraphicsComponent();
	//playerG->modelID = 1337;
	//playerG->active = true;
	//resHandler->GetModel(playerG->modelID, playerG->modelPtr);
	//PhysicsComponent* playerP = m_cHandler->GetPhysicsComponent();
	//playerP->PC_entityID = 0;								//Set Entity ID
	//playerP->PC_pos = DirectX::XMVectorSet(0, 5, 0, 0);		//Set Position
	//playerP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	//playerP->PC_is_Static = false;							//Set IsStatic
	//playerP->PC_active = true;								//Set Active
	//playerP->PC_mass = 5;
	//playerP->PC_BVtype = BV_AABB;
	//playerP->PC_AABB.ext[0] = 1.5;
	//playerP->PC_AABB.ext[1] = 1.5;
	//playerP->PC_AABB.ext[2] = 1.5;
	//player->Initialize();
	//player->SetGraphicsComponent(playerG);
	//player->SetPhysicsComponent(playerP);
	//this->m_entities.push_back(player);

	//Entity* ball = new Player();
	//GraphicsComponent* ballG = m_cHandler->GetGraphicsComponent();
	//ballG->modelID = 1337;
	//ballG->active = true;
	//resHandler->GetModel(ballG->modelID, ballG->modelPtr);
	//PhysicsComponent* ballP = m_cHandler->GetPhysicsComponent();
	//ballP->PC_entityID = 1;								//Set Entity ID
	//ballP->PC_pos = DirectX::XMVectorSet(10, 5, 0, 0);		//Set Position
	//ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	//ballP->PC_is_Static = false;							//Set IsStatic
	//ballP->PC_active = true;								//Set Active
	//ballP->PC_BVtype = BV_AABB;
	//ballP->PC_AABB.ext[0] = 1.5;
	//ballP->PC_AABB.ext[1] = 1.5;
	//ballP->PC_AABB.ext[2] = 1.5;
	//ballP->PC_mass = 10;
	//ball->Initialize();
	//ball->SetGraphicsComponent(ballG);
	//ball->SetPhysicsComponent(ballP);
	//this->m_entities.push_back(ball);


	//Player* ptr = (Player*)player;
	//ptr->SetGrabbed(ball);

	//this->m_cHandler->GetPhysicsHandler()->CreateChainLink(1, 0, 10, 2);

	//Entity* golv = new Player();
	//GraphicsComponent* golvG = m_cHandler->GetGraphicsComponent();
	//golvG->modelID = 1337;
	//golvG->active = true;
	//resHandler->GetModel(golvG->modelID, golvG->modelPtr);
	//PhysicsComponent* golvP = m_cHandler->GetPhysicsComponent();
	//golvP->PC_entityID = 1;								//Set Entity ID
	//golvP->PC_pos = DirectX::XMVectorSet(0, 0, 0, 0);		//Set Position
	//golvP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	//golvP->PC_is_Static = true;							//Set IsStatic
	//golvP->PC_active = true;								//Set Active
	//golvP->PC_BVtype = BV_Plane;
	//golvP->PC_Plane.PC_normal = DirectX::XMVectorSet(0, 1, 0, 0);
	//golvP->PC_OBB.ort = DirectX::XMMatrixIdentity();
	//golvP->PC_friction = 0.9;
	//golv->Initialize();
	//golv->SetGraphicsComponent(golvG);
	//golv->SetPhysicsComponent(golvP);
	//this->m_entities.push_back(golv);

		//For each entity in level
	for (size_t i = 0; i < data->numEntities; i++)
	{
		LevelData::EntityHeader* currEntity = &data->entities[i]; //Current entity

		//Create GraphicsComponent
		GraphicsComponent* t_gc = m_cHandler->GetGraphicsComponent();
		t_gc->modelID = currEntity->modelID;
		t_gc->active = true;
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
		t_pc->PC_entityID  = currEntity->EntityID;		//Set Entity ID
		t_pc->PC_pos	   = pos;						//Set Position
		t_pc->PC_rotation  = rot;						//Set Rotation
		t_pc->PC_is_Static = currEntity->isStatic;		//Set IsStatic
		t_pc->PC_active    = true;						//Set Active

		st = Resources::ResourceHandler::GetInstance()->GetModel(currEntity->modelID, modelPtr);
#ifdef _DEBUG
		if (st != Resources::ST_OK)
			std::cout << "Model could not be found when loading level data,  ID: " << currEntity->modelID << std::endl;
#endif // _DEBUG

		t_pc->PC_OBB = m_ConvertOBB( modelPtr->GetOBBData()); //Convert and insert OBB data
	
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
