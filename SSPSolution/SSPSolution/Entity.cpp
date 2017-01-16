#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

int Entity::SyncComponents()
{
	int result = 1;

	if (this->m_pComp != nullptr)
	{
		if (this->m_gComp != nullptr)
		{
			this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation), DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));
			result = 1;
		}
		else
		{
			result = -2;
		}
	}
	else
	{
		if (this->m_gComp == nullptr)
		{
			result = -3;
		}
		result = -1;
	}

	return result;
}

void Entity::UnsafeSyncComponents()
{
	this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation), DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));
}

PhysicsComponent* Entity::SetPhysicsComponent(PhysicsComponent * pComp)
{
	PhysicsComponent* tempReturn = this->m_pComp;
	this->m_pComp = pComp;
	return tempReturn;
}

GraphicsComponent* Entity::SetGraphicsComponent(GraphicsComponent * gComp)
{
	GraphicsComponent* tempReturn = this->m_gComp;
	this->m_gComp = gComp;
	return tempReturn;
}

AIComponent * Entity::SetAIComponent(AIComponent * aiComp)
{
	AIComponent* tempReturn = this->m_aiComp;
	this->m_aiComp = aiComp;
	return tempReturn;
}

bool Entity::SetGrabbed(int isGrabbed)
{
	bool lastValue = this->m_isGrabbed;
	this->m_isGrabbed = isGrabbed;
	return lastValue;
}

int Entity::SetEntityID(int entityID)
{
	int lastValue = this->m_entityID;
	this->m_entityID = entityID;
	return lastValue;
}

PhysicsComponent * Entity::GetPhysicsComponent()
{
	return this->m_pComp;
}

GraphicsComponent * Entity::GetGraphicComponent()
{
	return this->m_gComp;
}

AIComponent * Entity::GetAIComponent()
{
	return this->m_aiComp;
}

bool Entity::GetGrabbed()
{
	return this->m_isGrabbed;
}

int Entity::GetEntityID()
{
	return this->m_entityID;
}

int Entity::InitializeBase(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp)
{
	int result = 1;
	this->m_isGrabbed = false;
	this->m_subject = Subject();
	this->m_entityID = entityID;
	this->m_pComp = pComp;
	this->m_gComp = gComp;
	return result;
}
