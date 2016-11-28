//#include "PhysicsLibrary.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
	this->m_pos = DirectX::XMVectorSet(0, 0, 0, 0);
	this->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
}
PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::Update(const float &dt)
{

	float x = DirectX::XMVectorGetX(this->m_pos);
	float y = DirectX::XMVectorGetY(this->m_pos);
	float z = DirectX::XMVectorGetZ(this->m_pos);

	float velX = DirectX::XMVectorGetX(this->m_velocity);
	float velY = DirectX::XMVectorGetY(this->m_velocity);
	float velZ = DirectX::XMVectorGetZ(this->m_velocity);


	x += velX * dt;
	y += velY * dt;
	z += velZ * dt;

	this->m_pos = DirectX::XMVectorSet(x, y, z, 0.0f);



	//this->m_pos = DirectX::XMVectorAdd(this->m_pos, DirectX::XMVectorScale(this->m_velocity, dt));
}

DirectX::XMVECTOR PhysicsComponent::GetPos()const
{
	return this->m_pos;
}
DirectX::XMVECTOR PhysicsComponent::GetVelocity()const
{
	return this->m_velocity;
}
float PhysicsComponent::GetMass()const
{
	return this->m_mass;
}

void PhysicsComponent::SetPos(DirectX::XMVECTOR pos)
{
	float x = DirectX::XMVectorGetX(pos);
	float y = DirectX::XMVectorGetY(pos);
	float z = DirectX::XMVectorGetZ(pos);

	this->m_pos = DirectX::XMVectorSet(x, y, z, 0.0f);

	//this->m_pos = DirectX::XMVectorEqual(this->m_pos, pos);
}
void PhysicsComponent::SetVelocity(DirectX::XMVECTOR vel)
{
	float x = DirectX::XMVectorGetX(vel);
	float y = DirectX::XMVectorGetY(vel);
	float z = DirectX::XMVectorGetZ(vel);

	this->m_velocity = DirectX::XMVectorSet(x, y, z, 0.0f);

	//this->m_velocity = DirectX::XMVectorEqual(this->m_velocity, vel);
}
void PhysicsComponent::SetMass(float mass)
{
	this->m_mass = mass;
}

void PhysicsComponent::ApplyForce(DirectX::XMVECTOR force, const float &dt)
{
	this->m_velocity = DirectX::XMVectorAdd(this->m_velocity, DirectX::XMVectorScale(force, (1 / this->m_mass) * dt));
}