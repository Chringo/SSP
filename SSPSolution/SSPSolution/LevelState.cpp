#include "LevelState.h"



LevelState::LevelState()
{
}


LevelState::~LevelState()
{
}

int LevelState::ShutDown()
{
	int result = 1;
	return result;
}

int LevelState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* camera)
{
	int result = 1;
	result = GameState::InitializeBase(gsh, cHandler, camera);

	//Read from file
	//Get Components
	GraphicsComponent* tempGComp = this->m_cHandler->GetGraphicsComponent();
	PhysicsComponent* tempPComp = this->m_cHandler->GetPhysicsComponent();
	//Set Component values
	tempGComp->active = 1;
	tempGComp->modelID = 1337;
	tempGComp->worldMatrix = DirectX::XMMatrixIdentity();
	tempPComp->PC_active = 1;
	tempPComp->PC_pos = DirectX::XMVectorSet(0.0f, 1.0f, 6.0f, 1.0f);
	//Give Components to entities
	this->m_player1.Initialize();
	this->m_player1.SetGraphicsComponent(tempGComp);
	this->m_player1.SetPhysicsComponent(tempPComp);
	this->m_player1.SetSpeed(0.1f);
	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;
	dt = 1000000 / dt;
	this->m_player1.Update(dt, inputHandler);
	//Update the cameras position
	DirectX::XMVECTOR offset = DirectX::XMVectorSet(0.0f, 1.0f, -8.5f, 0.0f);
	PhysicsComponent* pComponent = this->m_player1.GetPhysicsComponentRef();
	DirectX::XMVECTOR pos = pComponent->PC_pos;
	pos = DirectX::XMVectorAdd(pos, offset);
	//DirectX::XMVectorAdd(pos, offset);

	this->m_cameraRef->SetCameraPos(DirectX::XMVectorAdd(pos, offset));
	offset = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	pos = DirectX::XMVectorAdd(pos, offset);
	this->m_cameraRef->SetLookAt(pos);
	this->m_cameraRef->Update();
	return result;
}
