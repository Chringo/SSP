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
	for (size_t i = 0; i < m_entities.size(); i++)
	{
		delete m_entities[i];
		m_entities[i] = nullptr;
	}
	
	return result;
}

int LevelState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 1;
	result = GameState::InitializeBase(gsh, cHandler, cameraRef);

	////Read from file
	////Get Components
	//GraphicsComponent* tempGComp = this->m_cHandler->GetGraphicsComponent();
	//PhysicsComponent* tempPComp = this->m_cHandler->GetPhysicsComponent();
	//////Set Component values
	//tempGComp->active = 1;
	//tempGComp->modelID = 1337;
	//tempGComp->worldMatrix = DirectX::XMMatrixIdentity();
	//tempPComp->PC_active = 1;
	//tempPComp->PC_pos = DirectX::XMVectorSet(0.0f, 1.0f, 6.0f, 1.0f);
	//////Give Components to entities
	//this->m_player1.Initialize();
	//this->m_player1.SetGraphicsComponent(tempGComp);
	//this->m_player1.SetPhysicsComponent(tempPComp);
	//this->m_player1.SetSpeed(0.1f);

	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;
	dt = 1000000 / dt;
	//this->m_player1.Update(dt, inputHandler);

	for (size_t i = 0; i < m_entities.size(); i++)
	{
		this->m_entities.at(i)->Update(dt, inputHandler);
	}

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

	Entity* player = new Player();
	GraphicsComponent* playerG = m_cHandler->GetGraphicsComponent();
	playerG->modelID = 0;
	playerG->active = true;
	PhysicsComponent* playerP = m_cHandler->GetPhysicsComponent();
	playerP->PC_entityID = 0;								//Set Entity ID
	playerP->PC_pos = DirectX::XMVectorSet(0, 5, 0, 0);		//Set Position
	playerP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	playerP->PC_is_Static = false;							//Set IsStatic
	playerP->PC_active = true;								//Set Active
	playerP->PC_BVtype = BV_AABB;
	playerP->PC_AABB.ext[0] = 1.5;
	playerP->PC_AABB.ext[1] = 1.5;
	playerP->PC_AABB.ext[2] = 1.5;
	player->Initialize();
	player->SetGraphicsComponent(playerG);
	player->SetPhysicsComponent(playerP);
	this->m_entities.push_back(player);

	Entity* ball = new StaticEntity();
	GraphicsComponent* ballG = m_cHandler->GetGraphicsComponent();
	ballG->modelID = 1;
	ballG->active = true;
	PhysicsComponent* ballP = m_cHandler->GetPhysicsComponent();
	ballP->PC_entityID = 0;								//Set Entity ID
	ballP->PC_pos = DirectX::XMVectorSet(10, 5, 0, 0);		//Set Position
	ballP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	ballP->PC_is_Static = false;							//Set IsStatic
	ballP->PC_active = true;								//Set Active
	ballP->PC_BVtype = BV_AABB;
	ballP->PC_AABB.ext[0] = 1.5;
	ballP->PC_AABB.ext[1] = 1.5;
	ballP->PC_AABB.ext[2] = 1.5;
	ballP->PC_mass = 10;
	ball->Initialize();
	ball->SetGraphicsComponent(ballG);
	ball->SetPhysicsComponent(ballP);
	this->m_entities.push_back(ball);

	Player* ptr = (Player*)player;
	ptr->SetGrabbed(ball);

	this->m_cHandler->GetPhysicsHandler()->CreateChainLink(1, 0, 10, 2);

	Entity* golv = new StaticEntity();
	GraphicsComponent* golvG = m_cHandler->GetGraphicsComponent();
	golvG->modelID = 1;
	golvG->active = true;
	PhysicsComponent* golvP = m_cHandler->GetPhysicsComponent();
	golvP->PC_entityID = 0;								//Set Entity ID
	golvP->PC_pos = DirectX::XMVectorSet(0, 0, 0, 0);		//Set Position
	golvP->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);//Set Rotation
	golvP->PC_is_Static = true;							//Set IsStatic
	golvP->PC_active = true;								//Set Active
	golvP->PC_BVtype = BV_Plane;
	golvP->PC_Plane.PC_normal = DirectX::XMVectorSet(0, 1, 0, 0);
	golvP->PC_OBB.ort = DirectX::XMMatrixIdentity();
	golv->Initialize();
	golv->SetGraphicsComponent(golvG);
	golv->SetPhysicsComponent(golvP);
	this->m_entities.push_back(golv);

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
	
		Entity* te;
		if (t_pc->PC_is_Static) {
			te = new StaticEntity();
		}
		else {
			te = new Player(); //TEMP! Change this to future class, such as dynamicEntity
			te->Initialize();
			t_pc->PC_AABB.ext[0] = 2;
			t_pc->PC_AABB.ext[1] = 2;
			t_pc->PC_AABB.ext[2] = 2;
		}

		te->SetGraphicsComponent(t_gc);
		te->SetPhysicsComponent(t_pc);
		
		this->m_entities.push_back(te); //Push new entity to list

	}
	return 1;
}
