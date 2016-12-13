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

int LevelState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler);

	//Read from file
	//Get Components
	GraphicsComponent* tempGComp = this->m_cHandler->GetGraphicsComponent();
	PhysicsComponent* tempPComp = this->m_cHandler->GetPhysicsComponent();
	//Set Component values
	tempGComp->active = 1;
	tempGComp->modelID = 1337;
	tempGComp->worldMatrix = DirectX::XMMatrixIdentity();
	tempPComp->PC_active = 1;
	tempPComp->PC_pos = DirectX::XMVectorSet(0.0f, 2.0f, 1.0f, 1.0f);
	//Give Components to entities
	this->m_player1.Initialize();
	this->m_player1.SetGraphicsComponent(tempGComp);
	this->m_player1.SetPhysicsComponent(tempPComp);
	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	int result = 0;

	this->m_player1.Update(dt, inputHandler);

	return result;
}
