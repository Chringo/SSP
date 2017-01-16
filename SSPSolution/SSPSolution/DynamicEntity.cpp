#include "DynamicEntity.h"

DynamicEntity::DynamicEntity()
{
}

DynamicEntity::~DynamicEntity()
{
}


int DynamicEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp)
{
	this->InitializeBase(entityID, pComp, gComp);
	return 0;
}

int DynamicEntity::Update(float dT, InputHandler * inputHandler)
{
	int result = 1;

	//Copy the world matrix into the graphics component for rendering purposes
	this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation), DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));

	return result;
}

int DynamicEntity::React(int entityID, EVENT reactEvent)
{
	return 0;
}
