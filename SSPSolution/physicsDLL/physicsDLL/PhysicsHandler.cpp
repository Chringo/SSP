#include "PhysicsHandler.h"
#include <malloc.h>
#include <iostream>
#include <chrono>
#include <ctime>

bool PhysicsHandler::IntersectAABB()
{
	bool possibleCollitionX = false;
	bool possibleCollitionY = false;
	bool possibleCollitionZ = false;
	PhysicsComponent* PC_ptr = nullptr;
	PhysicsComponent* PC_toCheck = nullptr;


	int nrOfComponents = this->m_dynamicComponents.size();
	float vecToObj[3];

	for (int i = 0; i < (nrOfComponents - this->m_nrOfStaticObjects); i++)
	{
		PC_toCheck = this->m_dynamicComponents.at(i);

		for (int j = i + 1; j < nrOfComponents; j++)
		{
			PC_ptr = this->m_dynamicComponents.at(j);

			for (int axis = 0; axis < 3; axis++)
			{
				vecToObj[axis] = 0; //remove clutter values, or old values
				vecToObj[axis] = PC_toCheck->m_AABB.pos[axis] - PC_ptr->m_AABB.pos[axis];
			}
			//Fraps return the absolute value
			//http://www.cplusplus.com/reference/cmath/fabs/

			//if the extensions from objA and objB together is smaller than the vector to b, then no collition
			possibleCollitionX = (fabs(vecToObj[0]) <= (PC_toCheck->m_AABB.ext[0] + PC_ptr->m_AABB.ext[0]));
			if (possibleCollitionX == true)
			{
				possibleCollitionY = (fabs(vecToObj[1]) <= (PC_toCheck->m_AABB.ext[0] + PC_ptr->m_AABB.ext[0]));
				if (possibleCollitionY == true)
				{
					possibleCollitionZ = (fabs(vecToObj[2]) <= (PC_toCheck->m_AABB.ext[0] + PC_ptr->m_AABB.ext[0]));
					if (possibleCollitionZ == true)
					{
						// apply OOB check for more precisition
					}
				}
			}
		}
	}
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
	PhysicsComponent* tempObj = nullptr;
	PhysicsComponent* tempObj2 = nullptr;

	tempObj = new PhysicsComponent;
	tempObj2 = new PhysicsComponent;

	//Obj nr 1 is at position (0,0,0) with a with of 1
	for (int i = 0; i < 3; i++)
	{
		tempObj->m_AABB.pos[i] = 0.0f;
		tempObj->m_OBB.pos[i] = 0.0f;

		tempObj->m_AABB.ext[i] = 1.0f;
		tempObj->m_OBB.ext[i] = 1.0f;

		tempObj->m_OBB.orth.x_axis[i] = 0.0f;
		tempObj->m_OBB.orth.y_axis[i] = 0.0f;
	}
	//local axis in world space sets here x = (1,0,0) and y-axis = (0, 1, 0)
	tempObj->m_OBB.orth.x_axis[0] = 1.0f;
	tempObj->m_OBB.orth.y_axis[1] = 1.0f;

	//Obj nr 2 is at position (3,0,0) with a width of 1
	for (int i = 0; i < 3; i++)
	{
		tempObj2->m_AABB.pos[i] = 0.0f;
		tempObj2->m_OBB.pos[i] = 0.0f;

		tempObj2->m_AABB.ext[i] = 1.0f;
		tempObj2->m_OBB.ext[i] = 1.0f;

		tempObj2->m_OBB.orth.x_axis[i] = 0.0f;
		tempObj2->m_OBB.orth.y_axis[i] = 0.0f;
	}
	//local axis in world space sets here x = (1,0,0) and y-axis = (0, 1, 0)
	tempObj2->m_OBB.orth.x_axis[0] = 1.0f;
	tempObj2->m_OBB.orth.y_axis[1] = 1.0f;


	this->m_nrOfStaticObjects = 0;
	this->m_gravity = DirectX::XMVectorSet(0, -0.000005, 0, 0);

	this->m_dynamicComponents.push_back(tempObj);
	this->m_dynamicComponents.push_back(tempObj2);

	return true;
}
void PhysicsHandler::ShutDown()
{
	int size = this->m_dynamicComponents.size();
	for (int i = 0; i < size; i++)
	{
		delete this->m_dynamicComponents.at(i);
	}
}
void PhysicsHandler::Update()
{
	float dt = 0.01f;
	this->checkCollition();

	//SimpleCollition(dt);
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

	std::chrono::time_point<std::chrono::system_clock>start;
	std::chrono::time_point<std::chrono::system_clock>end;

	start = std::chrono::system_clock::now();
	result = this->IntersectAABB();
	end = std::chrono::system_clock::now();

	std::chrono::duration<double>elapsed_secounds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	return result;
}

