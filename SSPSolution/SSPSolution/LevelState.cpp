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
	GraphicsComponent* tempComp = this->m_cHandler->GetGraphicsComponent();
	//Set Component values
	tempComp->active = 1;
	tempComp->modelID = 1337;
	tempComp->worldMatrix = DirectX::XMMatrixIdentity();
	//Give Components to entities
	this->m_player1.Initialize();
	return result;
}

int LevelState::Update(float dt, InputHandler * inputHandler)
{
	int result = 0;

	this->m_player1.Update(dt, inputHandler);

	return result;
}
