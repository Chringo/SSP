#include "Entity.h"



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

PhysicsComponent * Entity::GetPhysiscComponent() const
{
	PhysicsComponent* ptr = this->m_pComp;
	return ptr;
}
