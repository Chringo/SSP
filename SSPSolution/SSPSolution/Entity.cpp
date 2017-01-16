#include "Entity.h"



int Entity::InitializeBase()
{
	this->m_subject = Subject();
	return 0;
}

Entity::Entity()
{
}


Entity::~Entity()
{
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
