#include "PhysicsHandler.h"

PhysicsHandler::PhysicsHandler()
{
	this->m_nrOfComponents = 0;
	this->m_capacity = 0;
}
PhysicsHandler::~PhysicsHandler()
{
	delete[] this->m_components;
}

bool PhysicsHandler::Initialize()
{
	this->m_gravity = DirectX::XMVectorSet(0, -0.5, 0, 0);

	//this->m_components.push_back(PhysicsComponent());
	//this->m_components.at(0).SetPos(DirectX::XMVectorSet(0, 0.5, 0, 0));
	//this->m_components.at(0).SetVelocity(DirectX::XMFLOAT3(-1, 2, 0));

	//this->m_components.push_back(PhysicsComponent());
	//this->m_components.at(1).SetPos(DirectX::XMVectorSet(1, 0.5, 0, 0));
	//this->m_components.at(1).SetVelocity(DirectX::XMFLOAT3(-1, 1, 0));

	this->m_capacity = 10;
	this->m_nrOfComponents = 2;
	this->m_components = new PhysicsComponent[this->m_capacity];

	this->m_components[0].SetPos(DirectX::XMVectorSet(0, 2.5, 0, 0));
	this->m_components[1].SetPos(DirectX::XMVectorSet(1, 2.5, 0, 0));


	//this->m_walls.push_back(Plane());
	//this->m_walls.at(0).m_pos = DirectX::XMVectorSet(-5, 0, -5, 0);
	//this->m_walls.at(0).m_normal = DirectX::XMVectorSet(0, 1, 0, 0);
	//
	//this->m_walls.at(0).topLeft.m_pos1 = DirectX::XMVectorSet(-5, 0, -5, 0);
	//this->m_walls.at(0).topLeft.m_pos2 = DirectX::XMVectorSet(-5, 0, 5, 0);
	//this->m_walls.at(0).topLeft.m_pos3 = DirectX::XMVectorSet(5, 0, -5, 0);
	//
	//this->m_walls.at(0).bottomRight.m_pos1 = DirectX::XMVectorSet(5, 0, 5, 0);
	//this->m_walls.at(0).bottomRight.m_pos2 = DirectX::XMVectorSet(-5, 0, 5, 0);
	//this->m_walls.at(0).bottomRight.m_pos3 = DirectX::XMVectorSet(5, 0, -5, 0);
	//
	//this->m_walls.push_back(Plane());
	//this->m_walls.at(1).m_pos = DirectX::XMVectorSet(5, 5, 0, 0);
	//this->m_walls.at(1).m_normal = DirectX::XMVectorSet(-1, 0, 0, 0);
	//
	//this->m_walls.at(1).topLeft.m_pos1 = DirectX::XMVectorSet(5, 10, 5 , 0);
	//this->m_walls.at(1).topLeft.m_pos2 = DirectX::XMVectorSet(5, 10, -5, 0);
	//this->m_walls.at(1).topLeft.m_pos3 = DirectX::XMVectorSet(5, 0, 5, 0);
	//
	//this->m_walls.at(1).bottomRight.m_pos1 = DirectX::XMVectorSet(5, 0, -5, 0);
	//this->m_walls.at(1).bottomRight.m_pos2 = DirectX::XMVectorSet(5, 10, -5, 0);
	//this->m_walls.at(1).bottomRight.m_pos3 = DirectX::XMVectorSet(5, 0, 5, 0);

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
	//int componentSize = this->m_components.size();
	PhysicsComponent* ptr;
	for (int i = 0; i < this->m_nrOfComponents; i++)
	{
		ptr = &this->m_components[i];


		DirectX::XMVECTOR pos = ptr->GetPos();

		float y = DirectX::XMVectorGetY(pos);

		if (y > (0 + this->m_offSet))
		{
			SimpleGravity(ptr, dt);
		}
		else if (y < (0 + this->m_offSet))
		{
			ptr->SetPos(DirectX::XMVectorSet(DirectX::XMVectorGetX(pos), (0 + this->m_offSet), DirectX::XMVectorGetZ(pos), 0.0f));
			DirectX::XMVECTOR vel = ptr->GetVelocity();
			ptr->SetVelocity(DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
		}
		else if (y == (0 + this->m_offSet))
		{
			DirectX::XMVECTOR vel = ptr->GetVelocity();
			ptr->SetVelocity(DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0.0f, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
		}
		ptr->Update(dt);
	}
}
void PhysicsHandler::SimpleGravity(PhysicsComponent* componentPtr, const float &dt)
{
	DirectX::XMVECTOR test = DirectX::XMVECTOR();
	DirectX::XMFLOAT3 testRes(0, 5, 0);
	test = DirectX::XMLoadFloat3(&testRes);

	//pComponent->ApplyForce((this->m_gravity * pComponent->GetMass()), dt);

	componentPtr->ApplyForce(DirectX::XMVectorScale(this->m_gravity, componentPtr->GetMass()), dt);

	//PhysicsLibrary::Fuctions::Gravity(test, dt);
	//DirectX::XMStoreFloat3(&testRes, test);
}


int PhysicsHandler::getNrOfComponents()const
{
	return this->m_nrOfComponents;
}
PhysicsComponent* PhysicsHandler::getPComponents()const
{
	return this->m_components;
}

//bool PhysicsHandler::SpherePlaneIntersevtion(PhysicsComponent* pComponent, float radius, Plane plane, float dt)
//{
//	float m_frictionConstant = 0.99f; //this one is temporary
//	float m_bounsiness = 0.91f; // also temporary
//
//	bool result = false;
//
//	DirectX::XMVECTOR pos = pComponent->GetPos();
//	
//	DirectX::XMVECTOR toObjectVec = DirectX::XMVectorSubtract(pos, plane.m_pos);
//	
//	
//	float angle = DirectX::XMVectorGetIntX(DirectX::XMVector3Dot(toObjectVec, plane.m_normal));
//
//	
//	if (angle > 0.0f)
//	{
//
//		DirectX::XMVECTOR pParallel;
//		DirectX::XMVECTOR pPurpendicular;
//		DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPurpendicular, toObjectVec, plane.m_normal);
//
//		float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(pPurpendicular));
//
//		if (lenght <= radius)
//		{
//			DirectX::XMVECTOR pointOnPlane = DirectX::XMVectorAdd(plane.m_pos, pParallel);
//
//			result = PointInTriangleTest(pointOnPlane, plane.topLeft);
//			if (!result)
//			{
//				result = PointInTriangleTest(pointOnPlane, plane.bottomRight);
//			}
//			
//			if (result)
//			{
//				//if (lenghtToPlane < radius)
//				//{
//				//	pComponent->SetPos(plane.m_pos + (vecOnPlane)+(plane.m_normal * radius));
//				//}
//				//
//				//Vector3 vel = (pComponent->GetVelocity());
//				//
//				//
//				//
//				//Vector3 reflectedVel = vel - ((plane.m_normal * Physics::dot(vel, plane.m_normal)) * 2) * m_bounsiness;
//				//
//				//pComponent->SetVelocity(Vector3(reflectedVel.x * m_frictionConstant, reflectedVel.y * m_frictionConstant, reflectedVel.z * m_frictionConstant));
//			}
//	
//	
//			//pComponent->SetPos(plane.m_pos + (vecOnPlane) + (plane.m_normal * radius));
//			//
//			//Vector3 vel = pComponent->GetVelocity();
//			//
//			//pComponent->SetVelocity(Vector3(vel.x * m_frictionConstant, 0, vel.z * m_frictionConstant));
//			//
//			//result = true;
//	
//		}
//	}
//	
//	if (!result)
//	{
//		SimpleGravity(pComponent, dt);
//	}
//
//
//
//	return result;
//}
//bool PhysicsHandler::PointInTriangleTest(DirectX::XMVECTOR point, Triangle triangle)
//{
//	bool result = false;
//	//one example 
//	//http://math.stackexchange.com/questions/4322/check-whether-a-point-is-within-a-3d-triangle
//
//
//	//example used
//	//http://math.stackexchange.com/questions/544946/determine-if-projection-of-3d-point-onto-plane-is-within-a-triangle
//
//
//
//	DirectX::XMVECTOR u = DirectX::XMVectorSubtract(triangle.m_pos2, triangle.m_pos1);
//
//	DirectX::XMVECTOR v = DirectX::XMVectorSubtract(triangle.m_pos3, triangle.m_pos1);
//
//	DirectX::XMVECTOR n = DirectX::XMVector3Cross(u, v);
//
//	DirectX::XMVECTOR w = DirectX::XMVectorSubtract(point, triangle.m_pos1);
//
//	float y = DirectX::XMVectorGetX(DirectX::XMVectorScale(DirectX::XMVector3Dot(DirectX::XMVector3Cross(u, w), n), pow(DirectX::XMVectorGetX(DirectX::XMVector3Length(n)), 2)));
//
//	float b = DirectX::XMVectorGetX(DirectX::XMVectorScale(DirectX::XMVector3Dot(DirectX::XMVector3Cross(w, v), n), pow(DirectX::XMVectorGetX(DirectX::XMVector3Length(n)), 2)));
//
//	float c = 1 - y - b;
//
//	if (c >= 0 && c <= 1.0f && b >= 0 && b <= 1.0f && y >= 0 && y <= 1.0f)
//	{
//		result = true;
//	}
//
//
//	return result;
//}