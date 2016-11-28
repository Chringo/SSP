#include "PhysicsHandler.h"
#include <malloc.h>

bool PhysicsHandler::IntersectAABB()
{
	bool possibleCollitionX = false;
	bool possibleCollitionY = false;
	bool possibleCollitionZ = false;
	PhysicsComponent* PC_ptr = nullptr;


	int nrOfComponents = this->m_dynamicComponents.size();
	float vecToObj[3];


	for (int i = 0; i < nrOfComponents; i++)
	{
		PC_ptr = this->m_dynamicComponents.at(i);

		//iterate through all physicscomponents 
		for (int axis = 0; axis < 3; axis++)
		{
			vecToObj[axis] = 0; //remove clutter values, or old values
			vecToObj[axis] = toCheck.pos[axis] - PC_ptr->m_AABB.pos[axis];
		}

		//Fraps return the absolute value
		//http://www.cplusplus.com/reference/cmath/fabs/

		//if the extensions from objA and objB together is smaller than the vector to b, then no collition
		possibleCollitionX = (fabs(vecToObj[0]) <= (toCheck.ext[0] + PC_ptr->m_AABB.ext[0]));
		if (possibleCollitionX == true)
		{
			possibleCollitionY = (fabs(vecToObj[1]) <= (toCheck.ext[0] + PC_ptr->m_AABB.ext[0]));
			if (possibleCollitionY == true)
			{
				possibleCollitionZ = (fabs(vecToObj[2]) <= (toCheck.ext[0] + PC_ptr->m_AABB.ext[0]));
				if (possibleCollitionZ == true)
				{
					// apply OOB check for more precisition
					return true;
				}
			}
		}
	}
	//collition not possible
	return false;
}

PhysicsHandler::PhysicsHandler()
{
}
PhysicsHandler::~PhysicsHandler()
{
}

bool PhysicsHandler::Initialize()
{
	PhysicsComponent* tempPtr;

	this->m_gravity = DirectX::XMVectorSet(0, -0.000005, 0, 0);

	//first dummy obj
	tempPtr = new PhysicsComponent;
	tempPtr->m_pos = DirectX::XMVectorSet(-1, 5, 0, 0);
	tempPtr->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);

	tempPtr->m_AABB.pos[0] = 0;
	tempPtr->m_AABB.pos[1] = 0;
	tempPtr->m_AABB.pos[2] = 0;

	tempPtr->m_AABB.ext[0] = 1;
	tempPtr->m_AABB.ext[1] = 1;
	tempPtr->m_AABB.ext[2] = 1;

	this->m_dynamicComponents.push_back(tempPtr);

	//secound obj
	tempPtr = new PhysicsComponent;
	tempPtr->m_pos = DirectX::XMVectorSet(1, 5, 0, 0);
	tempPtr->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);

	tempPtr->m_AABB.pos[0] = 2;
	tempPtr->m_AABB.pos[1] = 0;
	tempPtr->m_AABB.pos[2] = 0;

	tempPtr->m_AABB.ext[0] = 1;
	tempPtr->m_AABB.ext[1] = 1;
	tempPtr->m_AABB.ext[2] = 1;

	this->m_dynamicComponents.push_back(tempPtr);

	this->checkCollition();

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

bool PhysicsHandler::checkCollition()
{
	bool result = false;
	result = this->IntersectAABB();

	return result;
}