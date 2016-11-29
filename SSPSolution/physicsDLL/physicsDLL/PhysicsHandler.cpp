#include "PhysicsHandler.h"
#include <malloc.h>

PhysicsHandler::PhysicsHandler()
{
}
PhysicsHandler::~PhysicsHandler()
{
}

bool PhysicsHandler::Initialize()
{
	this->m_gravity = DirectX::XMVectorSet(0, -0.005, 0, 0);
	

	this->m_dynamicComponents.push_back(new PhysicsComponent);
	this->m_dynamicComponents.push_back(new PhysicsComponent);

	this->m_dynamicComponents.at(0)->m_pos = DirectX::XMVectorSet(-1, 5, 0, 0);
	this->m_dynamicComponents.at(0)->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);

	this->m_dynamicComponents.at(1)->m_pos = DirectX::XMVectorSet(1, 5, 0, 0);
	this->m_dynamicComponents.at(1)->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);


	return true;
}
void PhysicsHandler::Update()
{
	float dt = 0.01f;

	SimpleCollition(dt);
}
void PhysicsHandler::SimpleCollition(float dt)
{
	float m_frictionConstant = 0.999f;
	PhysicsComponent* ptr;
	int size = this->m_dynamicComponents.size();
	for (int i = 0; i < size; i++)
	{
		ptr = this->m_dynamicComponents.at(i);


		DirectX::XMVECTOR pos = ptr->m_pos;

		float y = DirectX::XMVectorGetY(pos);

		if (y > (0 + this->m_offSet))
		{
			SimpleGravity(ptr, dt);
		}
		else if (y < (0 + this->m_offSet))
		{
			ptr->m_pos =  (DirectX::XMVectorSet(DirectX::XMVectorGetX(pos), (0 + this->m_offSet), DirectX::XMVectorGetZ(pos), 0.0f));
			DirectX::XMVECTOR vel = ptr->m_velocity;
			ptr->m_velocity = (DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
		}
		else if (y == (0 + this->m_offSet))
		{
			DirectX::XMVECTOR vel = ptr->m_velocity;
			ptr->m_velocity = (DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0.0f, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
		}

		ptr->m_pos = DirectX::XMVectorAdd(ptr->m_pos, DirectX::XMVectorScale(ptr->m_velocity, dt));
	}
}
void PhysicsHandler::SimpleGravity(PhysicsComponent* componentPtr, const float &dt)
{
	DirectX::XMVECTOR test = DirectX::XMVECTOR();
	DirectX::XMFLOAT3 testRes(0, 5, 0);
	test = DirectX::XMLoadFloat3(&testRes);


	componentPtr->m_velocity = DirectX::XMVectorAdd(componentPtr->m_velocity, this->m_gravity);

}


int PhysicsHandler::getNrOfComponents()const
{
	return this->m_dynamicComponents.size();
}
PhysicsComponent* PhysicsHandler::getDynamicComponents(int index)const
{
	if (index >= 0 && index < this->m_dynamicComponents.size())
	{
		return this->m_dynamicComponents.at(index);
	}
	return nullptr;

}
