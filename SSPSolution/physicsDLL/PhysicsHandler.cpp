#include "PhysicsHandler.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>


bool PhysicsHandler::IntersectAABB()
{
	bool possibleCollitionX = false;
	bool possibleCollitionY = false;
	bool possibleCollitionZ = false;
	PhysicsComponent* PC_ptr = nullptr;
	PhysicsComponent* PC_toCheck = nullptr;
	bool result = false;

	DirectX::XMFLOAT3 temp;
	DirectX::XMFLOAT3 temp2;



	int nrOfComponents = this->m_dynamicComponents.size();
	float vecToObj[3];

	for (int i = 0; i < (nrOfComponents - this->m_nrOfStaticObjects); i++)
	{
		PC_toCheck = this->m_dynamicComponents.at(i);
		DirectX::XMStoreFloat3(&temp, PC_toCheck->PC_pos);

		for (int j = i + 1; j < nrOfComponents; j++)
		{
			PC_ptr = this->m_dynamicComponents.at(j);
			DirectX::XMStoreFloat3(&temp2, PC_ptr->PC_pos);

			vecToObj[0] = 0; //remove clutter values, or old values
			vecToObj[0] = temp.x - temp2.x;

			vecToObj[1] = 0; //remove clutter values, or old values
			vecToObj[1] = temp.y - temp2.y;

			vecToObj[2] = 0; //remove clutter values, or old values
			vecToObj[2] = temp.z - temp2.z;

			//Fraps return the absolute value
			//http://www.cplusplus.com/reference/cmath/fabs/

			//if the extensions from objA and objB together is smaller than the vector to b, then no collition
			possibleCollitionX = (fabs(vecToObj[0]) <= (PC_toCheck->PC_AABB.ext[0] + PC_ptr->PC_AABB.ext[0]));
			if (possibleCollitionX == true)
			{
				possibleCollitionY = (fabs(vecToObj[1]) <= (PC_toCheck->PC_AABB.ext[1] + PC_ptr->PC_AABB.ext[1]));
				if (possibleCollitionY == true)
				{
					possibleCollitionZ = (fabs(vecToObj[2]) <= (PC_toCheck->PC_AABB.ext[2] + PC_ptr->PC_AABB.ext[2]));
					if (possibleCollitionZ == true)
					{
						// apply OOB check for more precisition
						result = true;
						//result = this->DoIntersectionTestOBB(PC_toCheck, PC_ptr);
					}
				}
			}
		}
	}
	return result;
}

bool PhysicsHandler::AABBAABBIntersectionTest(PhysicsComponent *obj1, PhysicsComponent *obj2, float dt)
{
	bool possibleCollitionX = false;
	bool possibleCollitionY = false;
	bool possibleCollitionZ = false;
	PhysicsComponent* PC_ptr = nullptr;
	PhysicsComponent* PC_toCheck = nullptr;
	bool result = false;

	DirectX::XMFLOAT3 temp;
	DirectX::XMFLOAT3 temp2;

	float xOverlap;
	float yOverlap;
	float zOverlap;


	int nrOfComponents = this->m_dynamicComponents.size();
	float vecToObj[3];

	PC_toCheck = obj1;
	DirectX::XMStoreFloat3(&temp, PC_toCheck->PC_pos);

	PC_ptr = obj2;
	DirectX::XMStoreFloat3(&temp2, PC_ptr->PC_pos);

	vecToObj[0] = 0; //remove clutter values, or old values
	vecToObj[0] = temp.x - temp2.x;

	vecToObj[1] = 0; //remove clutter values, or old values
	vecToObj[1] = temp.y - temp2.y;

	vecToObj[2] = 0; //remove clutter values, or old values
	vecToObj[2] = temp.z - temp2.z;

			//Fraps return the absolute value
			//http://www.cplusplus.com/reference/cmath/fabs/

			//if the extensions from objA and objB together is smaller than the vector to b, then no collition
		xOverlap = vecToObj[0];
		float x_total_ext = PC_toCheck->PC_AABB.ext[0] + PC_ptr->PC_AABB.ext[0];
		float y_total_ext = PC_toCheck->PC_AABB.ext[1] + PC_ptr->PC_AABB.ext[1];
		float z_total_ext = PC_toCheck->PC_AABB.ext[2] + PC_ptr->PC_AABB.ext[2];




		possibleCollitionX = (fabs(vecToObj[0]) <= PC_toCheck->PC_AABB.ext[0] + PC_ptr->PC_AABB.ext[0]);
		if (possibleCollitionX == true)
		{
			yOverlap = vecToObj[1];
			possibleCollitionY = (fabs(vecToObj[1]) <= PC_toCheck->PC_AABB.ext[1] + PC_ptr->PC_AABB.ext[1]);
			if (possibleCollitionY == true)
			{
				zOverlap = vecToObj[2];
				possibleCollitionZ = (fabs(vecToObj[2]) <= PC_toCheck->PC_AABB.ext[2] + PC_ptr->PC_AABB.ext[2]);
				if (possibleCollitionZ == true)
				{
					DirectX::XMVECTOR normal;
					// apply OOB check for more precisition
					result = true;
					if (
						fabs(xOverlap  / x_total_ext) > fabs(yOverlap / y_total_ext)
						&&
						fabs(xOverlap / x_total_ext) > fabs(zOverlap / z_total_ext)
						)
					{
						//overlapX
						float distanceToMove = obj1->PC_AABB.ext[0] + obj2->PC_AABB.ext[0];
						distanceToMove = distanceToMove - fabs(vecToObj[0]);
						normal = DirectX::XMVectorSet(1, 0, 0, 0);
						if (vecToObj[0] < 0)
						{
							distanceToMove *= -1;
							normal = DirectX::XMVectorSet(-1, 0, 0, 0);
						}
						obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, DirectX::XMVectorSet(distanceToMove, 0, 0, 0));
					}

					if (
						fabs(yOverlap / y_total_ext) > fabs(xOverlap / x_total_ext)
						&&
						fabs(yOverlap / y_total_ext) > fabs(zOverlap / z_total_ext)
						)
					{
						float distanceToMove = obj1->PC_AABB.ext[1] + obj2->PC_AABB.ext[1];
						distanceToMove = distanceToMove - fabs(vecToObj[1]);
						normal = DirectX::XMVectorSet(0, 1, 0, 0);
						if (vecToObj[1] < 0)
						{
							distanceToMove *= -1;
							normal = DirectX::XMVectorSet(0, -1, 0, 0);
						}
						obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, DirectX::XMVectorSet(0, distanceToMove, 0, 0));
					}

					if (
						fabs(zOverlap / z_total_ext) > fabs(xOverlap / x_total_ext)
						&&
						fabs(zOverlap / z_total_ext) > fabs(yOverlap / y_total_ext)
						)
					{
						float distanceToMove = obj1->PC_AABB.ext[2] + obj2->PC_AABB.ext[2];
						distanceToMove = distanceToMove - fabs(vecToObj[2]);
						normal = DirectX::XMVectorSet(0, 0, 1, 0);
						if (vecToObj[2] < 0)
						{
							distanceToMove *= -1;
							normal = DirectX::XMVectorSet(0, 0, -1, 0);
						}
						obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, DirectX::XMVectorSet(0, 0, distanceToMove, 0));
					}
					this->CollitionDynamics(obj1, obj2, normal, dt);
					obj1->PC_normalForce = normal;

					//overlappY

					//overlappZ



				}
			}
		}
	return result;
}

bool PhysicsHandler::ObbObbIntersectionTest(PhysicsComponent* objA, PhysicsComponent* objB)
{
	DirectX::XMFLOAT3 transPF_v;
	DirectX::XMFLOAT3 transPF_t;

	DirectX::XMFLOAT3 orthA[3];
	DirectX::XMFLOAT3 orthB[3];

	DirectX::XMFLOAT3 posA;
	DirectX::XMFLOAT3 posB;



	DirectX::XMStoreFloat3(&posA, objA->PC_pos);
	DirectX::XMStoreFloat3(&posB, objB->PC_pos);


	//not very clever way, but I need to know if shit work, for debug purpuses
	for (int i = 0; i < 3; i++)
	{
		DirectX::XMStoreFloat3(&orthA[i], objA->PC_OBB.ort.r[i]);
		DirectX::XMStoreFloat3(&orthA[i], objA->PC_OBB.ort.r[i]);

		DirectX::XMStoreFloat3(&orthB[i], objB->PC_OBB.ort.r[i]);
		DirectX::XMStoreFloat3(&orthB[i], objB->PC_OBB.ort.r[i]);
	}

	OBB* a = nullptr;
	OBB* b = nullptr;

	a = &objA->PC_OBB;
	b = &objB->PC_OBB;

	float T[3];

	transPF_v = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	//scalar values
	float rA = 0.0f;
	float rB = 0.0f;
	float t = 0.0f;

	DirectX::XMFLOAT3 L = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	//B's basis with respect to A's local frame
	DirectX::XMFLOAT3X3 R;

	//this holds the translation vector in parent frame
	transPF_v = this->VectorSubstract(posA, posB);


	//translation in A's frame (START)

	//really tedious to do this, if time is given, we should make a better dot product
	//really directX?!?!?
	for (int i = 0; i < 3; i++)
	{
		T[i] = this->DotProduct(transPF_v, orthA[i]);

	}
	//T[1] = -2.0f;
	//T[2] = 0.0;
	//translation in A's frame (END)

	//calculate the rotation matrix
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			R.m[i][k] = this->DotProduct(orthA[i], orthB[k]);
		}
	}

	/*ALGORITHM: Use the separating axis test for all 15 potential
	separating axes. If a separating axis could not be found, the two
	boxes overlap. */

	//A's basis vectors
	for (int i = 0; i < 3; i++)
	{
		rA = a->ext[i];

		rB = b->ext[0] * fabs(R.m[i][0]) + b->ext[1] * fabs(R.m[i][1]) + b->ext[2] * fabs(R.m[i][2]);

		t = fabs(T[i]);

		if (t >(rA + rB))
		{
			return false;
		}
	}

	//B's basis vectors
	for (int i = 0; i < 3; i++)
	{
		rA = a->ext[0] * fabs(R.m[0][i]) + a->ext[1] * fabs(R.m[1][i]) + a->ext[2] * fabs(R.m[2][i]);

		rB = a->ext[i];

		t = fabs(T[i]);

		if (t >(rA + rB))
		{
			return false;
		}
	}

	//9 cross products??
	/*
	I have no clue what happening here mathwise, if time is with us, I will try to understand what is happening
	here.

	//sorce
	http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=5
	*/

	// L = A0 x B0
	rA = a->ext[1] * fabs(R.m[2][0]) + a->ext[2] * fabs(R.m[1][0]);
	rB = b->ext[1] * fabs(R.m[0][2]) + b->ext[2] * fabs(R.m[0][1]);

	t = fabs(T[2] * R.m[1][0] - T[1] * R.m[2][0]);

	if (t > (rA + rB))
	{
		return false;
	}

	//L = A0 x B1
	rA = a->ext[1] * fabs(R.m[2][1]) + a->ext[2] * fabs(R.m[1][1]);
	rB = b->ext[0] * fabs(R.m[0][2]) + b->ext[2] * fabs(R.m[0][0]);

	t = fabs(T[2] * R.m[1][1] - T[1] * R.m[2][1]);

	if (t > (rA + rB))
	{
		return false;
	}

	//L = A0 x B2
	rA = a->ext[1] * fabs(R.m[2][2]) + a->ext[2] * fabs(R.m[1][2]);
	rB = b->ext[0] * fabs(R.m[0][1]) + b->ext[1] * fabs(R.m[0][0]);

	t = fabs(T[2] * R.m[1][2] - T[1] * R.m[2][2]);

	if (t > (rA + rB))
	{
		return false;
	}

	// L = A1 x B0
	rA = a->ext[0] * fabs(R.m[2][0]) + a->ext[2] * fabs(R.m[0][0]);
	rB = b->ext[1] * fabs(R.m[1][2]) + b->ext[2] * fabs(R.m[1][1]);

	t = fabs(T[0] * R.m[2][0] - T[2] * R.m[0][0]);

	if (t > (rA + rB))
	{
		return false;
	}

	//L = A1 x B1
	rA = a->ext[0] * fabs(R.m[2][1]) + a->ext[2] * fabs(R.m[0][1]);
	rB = b->ext[0] * fabs(R.m[1][2]) + b->ext[2] * fabs(R.m[1][0]);

	t = fabs(T[0] * R.m[2][1] - T[2] * R.m[0][1]);

	if (t > (rA + rB))
	{
		return false;
	}

	//L = A1 x B2
	rA = a->ext[0] * fabs(R.m[2][2]) + a->ext[2] * fabs(R.m[0][2]);
	rB = b->ext[0] * fabs(R.m[1][1]) + b->ext[1] * fabs(R.m[1][0]);

	t = fabs(T[0] * R.m[2][2] - T[2] * R.m[0][2]);

	if (t > (rA + rB))
	{
		return false;
	}

	// L = A2 x B0
	rA = a->ext[0] * fabs(R.m[1][0]) + a->ext[1] * fabs(R.m[0][0]);

	rB = b->ext[1] * fabs(R.m[2][2]) + b->ext[2] * fabs(R.m[2][1]);

	t = fabs(T[1] * R.m[0][0] - T[0] * R.m[1][0]);

	if (t > (rA + rB))
	{
		return false;
	}

	//L = A2 x B1
	rA = a->ext[0] * fabs(R.m[1][1]) + a->ext[1] * fabs(R.m[0][1]);

	rB = b->ext[0] * fabs(R.m[2][2]) + b->ext[2] * fabs(R.m[2][0]);

	t = fabs(T[2] * R.m[1][1] - T[1] * R.m[2][1]);
	float test = rA + rB;

	if (t > (rA + rB))
	{
		return false;
	}

	//L = A2 x B2
	rA = a->ext[0] * fabs(R.m[1][2]) + a->ext[1] * fabs(R.m[0][2]);

	rB = b->ext[0] * fabs(R.m[2][1]) + b->ext[1] * fabs(R.m[2][0]);

	t = fabs(T[1] * R.m[0][2] - T[0] * R.m[1][2]);

	if (t > (rA + rB))
	{
		return false;
	}
	/*no separating axis found,
	the two boxes overlap */

	//printf("Collition has been detected\n");
	return true;
}

bool PhysicsHandler::SphereAABBIntersectionTest(PhysicsComponent * objSphere, PhysicsComponent * objAABB)
{

	DirectX::XMVECTOR sphere_local = DirectX::XMVectorSubtract(objSphere->PC_pos, objAABB->PC_pos);
	float sphere_pos[3];
	sphere_pos[0] = DirectX::XMVectorGetX(sphere_local);
	sphere_pos[1] = DirectX::XMVectorGetY(sphere_local);
	sphere_pos[2] = DirectX::XMVectorGetZ(sphere_local);

	float box_axis[3];
	box_axis[0] = objAABB->PC_AABB.ext[0];
	box_axis[1] = objAABB->PC_AABB.ext[1];
	box_axis[2] = objAABB->PC_AABB.ext[2];

	float s = 0;
	float d = 0;

	for (int i = 0; i < 3; i++)
	{
		if (sphere_pos[i] < -box_axis[i])
		{
			s = sphere_pos[i] - (-box_axis[i]);
			d += s*s;
		}
		else if (sphere_pos[i] > box_axis[i])
		{
			s = sphere_pos[i] - box_axis[i];
			d += s*s;
		}
	}

	return d <= (objSphere->PC_Sphere.radius * objSphere->PC_Sphere.radius);
}

bool PhysicsHandler::SphereOBBIntersectionTest(PhysicsComponent * objSphere, PhysicsComponent * objOBB)
{
	DirectX::XMVECTOR sphere_local = DirectX::XMVectorSubtract(objSphere->PC_pos, objOBB->PC_pos);
	float sphere_pos[3];
	sphere_pos[0] = DirectX::XMVectorGetX(sphere_local);
	sphere_pos[1] = DirectX::XMVectorGetY(sphere_local);
	sphere_pos[2] = DirectX::XMVectorGetZ(sphere_local);

	DirectX::XMVECTOR min = DirectX::XMVectorSet(0, 0, 0, 0);
	min = DirectX::XMVectorSubtract(min, objOBB->PC_OBB.ort.r[0]);
	min = DirectX::XMVectorSubtract(min, objOBB->PC_OBB.ort.r[1]);
	min = DirectX::XMVectorSubtract(min, objOBB->PC_OBB.ort.r[2]);
	DirectX::XMVECTOR max = DirectX::XMVectorSet(0, 0, 0, 0);
	max = DirectX::XMVectorAdd(max, objOBB->PC_OBB.ort.r[0]);
	max = DirectX::XMVectorAdd(max, objOBB->PC_OBB.ort.r[1]);
	max = DirectX::XMVectorAdd(max, objOBB->PC_OBB.ort.r[2]);


	float box_MIN[3];
	box_MIN[0] = DirectX::XMVectorGetX(min);
	box_MIN[1] = DirectX::XMVectorGetY(min);
	box_MIN[2] = DirectX::XMVectorGetZ(min);

	float box_MAX[3];
	box_MAX[0] = DirectX::XMVectorGetX(max);
	box_MAX[1] = DirectX::XMVectorGetY(max);
	box_MAX[2] = DirectX::XMVectorGetZ(max);

	float s = 0;
	float d = 0;

	for (int i = 0; i < 3; i++)
	{
		if (sphere_pos[i] < box_MIN[i])
		{
			s = sphere_pos[i] - box_MIN[i];
			d += s*s;
		}
		else if (sphere_pos[i] > box_MAX[i])
		{
			s = sphere_pos[i] - box_MAX[i];
			d += s*s;
		}
	}

	return d <= (objSphere->PC_Sphere.radius * objSphere->PC_Sphere.radius);
}

bool PhysicsHandler::SphereSphereIntersectionTest(PhysicsComponent * objSphere1, PhysicsComponent * objSphere2)
{
	float d = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(objSphere1->PC_pos, objSphere2->PC_pos)));


	return d <= (objSphere1->PC_Sphere.radius + objSphere2->PC_Sphere.radius);
}

bool PhysicsHandler::SpherePlaneIntersectionTest(PhysicsComponent * objSphere, PhysicsComponent * objPlane, float dt)
{
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(objSphere->PC_pos, objPlane->PC_pos);
	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR pPerpendicular;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, diffVec, objPlane->PC_Plane.PC_normal);

	float d = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));

	bool result = d <= objSphere->PC_Sphere.radius;

	if (result)
	{
		float lenght = (objSphere->PC_Sphere.radius - d);
		objSphere->PC_pos = DirectX::XMVectorAdd(objSphere->PC_pos, DirectX::XMVectorScale(pParallel, lenght));

		this->CollitionDynamics(objSphere, objPlane, objPlane->PC_Plane.PC_normal, dt);


		objSphere->PC_normalForce = objPlane->PC_Plane.PC_normal;
	}

	return result;
}

bool PhysicsHandler::AABBPlaneIntersectionTest(PhysicsComponent * objAABB, PhysicsComponent * objPlane)
{
	float d = 0;

	float a1 = objAABB->PC_AABB.ext[0];
	float a2 = objAABB->PC_AABB.ext[1];
	float a3 = objAABB->PC_AABB.ext[2];

	float sum = 0;
	sum += a1 * abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, DirectX::XMVectorSet(a1, 0, 0, 0))));
	sum += a2 * abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, DirectX::XMVectorSet(0, a2, 0, 0))));
	sum += a3 * abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, DirectX::XMVectorSet(0, 0, a3, 0))));

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(objAABB->PC_pos, objPlane->PC_pos);
	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR dump;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &dump, diffVec, objPlane->PC_Plane.PC_normal);

	d = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));

	return d < sum;
}

bool PhysicsHandler::OBBPlaneIntersectionTest(PhysicsComponent * objOBB, PhysicsComponent * objPlane)
{
	DirectX::XMVECTOR axis1 = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR axis2 = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR axis3 = DirectX::XMVectorSet(0, 0, 0, 0);

	axis1 = objOBB->PC_OBB.ort.r[0];
	axis2 = objOBB->PC_OBB.ort.r[1];
	axis3 = objOBB->PC_OBB.ort.r[2];

	float a1 = DirectX::XMVectorGetX(DirectX::XMVector3Length(axis1));
	float a2 = DirectX::XMVectorGetX(DirectX::XMVector3Length(axis2));
	float a3 = DirectX::XMVectorGetX(DirectX::XMVector3Length(axis3));

	float sum = 0;
	sum += a1 * abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, axis1)));
	sum += a2 * abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, axis2)));
	sum += a3 * abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, axis3)));

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(objOBB->PC_pos, objPlane->PC_pos);
	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR pPerpendicular;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, diffVec, objPlane->PC_Plane.PC_normal);

	float d = 0;

	d = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));

	bool result = d < sum;
	if (result)
	{
		DirectX::XMVECTOR resultVel = DirectX::XMVector3Reflect(objOBB->PC_velocity, objPlane->PC_Plane.PC_normal);
		resultVel = DirectX::XMVectorScale(resultVel, 0.7f);
		//objOBB->PC_pos = DirectX::XMVectorAdd(objOBB->PC_pos, pParallel);
		objOBB->PC_velocity = resultVel;
	}

	return result;
}

void PhysicsHandler::CollitionDynamics(PhysicsComponent* obj1, PhysicsComponent* obj2, DirectX::XMVECTOR normal, float dt)
{
	if (obj2->PC_is_Static)
	{
		DirectX::XMVECTOR pParallel;
		DirectX::XMVECTOR pPerpendicular;
		DirectX::XMVECTOR velDir = DirectX::XMVector3Normalize(obj1->PC_velocity);
		DirectX::XMVECTOR oldVel = obj1->PC_velocity;
		DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, oldVel, normal);
		float old_parallelVel = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));
		float old_perpendicularVel = DirectX::XMVectorGetX(DirectX::XMVector3Length(pPerpendicular));

		float newPerpendicularVel = old_perpendicularVel * obj2->PC_friction;
		float newParallelVel = old_parallelVel * obj2->PC_elasticity;

		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(velDir, normal)) < 0)
		{
			velDir = DirectX::XMVector3Reflect(velDir, normal);
			//for direction when new force is applied
			pParallel = DirectX::XMVectorScale(pParallel, -1);
			old_parallelVel *= -1;
		}
		pParallel = DirectX::XMVector3Normalize(pParallel);
		pPerpendicular = DirectX::XMVector3Normalize(pPerpendicular);

		float parallelImpuls = obj1->PC_mass * newParallelVel - obj1->PC_mass * old_parallelVel;
		float perpendicularImpuls = obj1->PC_mass * newPerpendicularVel - obj1->PC_mass * old_perpendicularVel;

		float parallelForce = parallelImpuls / dt;
		float perpendicularForce = perpendicularImpuls / dt;

		this->ApplyForceToComponent(obj1, DirectX::XMVectorScale(pParallel, parallelForce), dt);
		this->ApplyForceToComponent(obj1, DirectX::XMVectorScale(pPerpendicular, perpendicularForce), dt);

	}
	else
	{
		DirectX::XMVECTOR pParallel;
		DirectX::XMVECTOR pPerpendicular1;
		DirectX::XMVECTOR pPerpendicular2;

		float forceVec1[3];
		float forceVec2[3];

		float v1_old[3];
		float v1_new[3];
		float m1 = obj1->PC_mass;
		DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular1, obj1->PC_velocity, normal);
		v1_old[0] = DirectX::XMVectorGetX(pParallel);
		v1_old[1] = DirectX::XMVectorGetY(pParallel);
		v1_old[2] = DirectX::XMVectorGetZ(pParallel);

		float v2_old[3];
		float v2_new[3];
		float m2 = obj2->PC_mass;
		DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular2, obj2->PC_velocity, normal);
		v2_old[0] = DirectX::XMVectorGetX(pParallel);
		v2_old[1] = DirectX::XMVectorGetY(pParallel);
		v2_old[2] = DirectX::XMVectorGetZ(pParallel);

		float e = 0.0;

		for (int i = 0; i < 3; i++)
		{

			v1_new[i] = (v1_old[i] * (m1 - m2*e) + m2*v2_old[i] * (1.0f + e)) / (m1 + m2);

			v2_new[i] = (v1_old[i] * m1*(1.0f + e) + (m2 - e*m1)*v2_old[i]) / (m1 + m2);

		}
		v1_new[0] += DirectX::XMVectorGetX(pPerpendicular1);
		v1_new[1] += DirectX::XMVectorGetY(pPerpendicular1);
		v1_new[2] += DirectX::XMVectorGetZ(pPerpendicular1);

		v2_new[0] += DirectX::XMVectorGetX(pPerpendicular2);
		v2_new[1] += DirectX::XMVectorGetY(pPerpendicular2);
		v2_new[2] += DirectX::XMVectorGetZ(pPerpendicular2);
		if (!obj1->PC_is_Static)
		{
			obj1->PC_velocity = DirectX::XMVectorSet(v1_new[0], v1_new[1], v1_new[2], 0);
		}
		if (!obj2->PC_is_Static)
		{
			obj2->PC_velocity = DirectX::XMVectorSet(v2_new[0], v2_new[1], v2_new[2], 0);
		}
	}


}

float PhysicsHandler::DotProduct(const DirectX::XMFLOAT3 & v1, const DirectX::XMFLOAT3 & v2) const
{
	DirectX::XMVECTOR temp1;
	DirectX::XMVECTOR temp2;
	float result = 0;
	
	temp1 = DirectX::XMLoadFloat3(&v1);
	temp2 = DirectX::XMLoadFloat3(&v2);

	DirectX::XMStoreFloat(&result, DirectX::XMVector3Dot(temp1, temp2));

	return result;
}

DirectX::XMFLOAT3 PhysicsHandler::CrossProduct(const DirectX::XMFLOAT3 & v1, const DirectX::XMFLOAT3 & v2) const
{
	DirectX::XMFLOAT3 result = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR temp1;
	DirectX::XMVECTOR temp2;

	temp1 = DirectX::XMLoadFloat3(&v1);
	temp2 = DirectX::XMLoadFloat3(&v2);

	temp1 = DirectX::XMVector2Cross(temp1, temp2);
	DirectX::XMStoreFloat3(&result, temp1);
	
	return result;
}

float PhysicsHandler::DotProduct(const DirectX::XMVECTOR & v1, const DirectX::XMVECTOR & v2) const
{
	DirectX::XMVECTOR cont;
	float result = 0.0f;

	cont = DirectX::XMVector3Dot(v1, v2);
	DirectX::XMStoreFloat(&result, cont);

	return result;
}

DirectX::XMFLOAT3 PhysicsHandler::VectorSubstract(const DirectX::XMFLOAT3 & v1, const DirectX::XMFLOAT3 & v2) const
{
	DirectX::XMFLOAT3 result;
	
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	
	return result;
}

DirectX::XMMATRIX PhysicsHandler::RotateBB_X(PhysicsComponent* src, const float &radian)
{
	DirectX::XMMATRIX xMatrix;
	DirectX::XMMATRIX test = src->PC_OBB.ort;

	//read the value of PC_rotation
	//DirectX::XMStoreFloat3(&rot,src->PC_rotation);
	float rotAngle = (3.14159265359f / 180.0f);

	xMatrix = DirectX::XMMatrixRotationX(radian);
	test = DirectX::XMMatrixMultiply(test, xMatrix);


	//xMatrix = DirectX::XMMatrixRotationX((3.14159265359 / 4.0));
	//test = DirectX::XMMatrixMultiply(test, xMatrix);
	//xMatrix = DirectX::XMMatrixRotationZ((3.14159265359 / 4.0));
	//test = DirectX::XMMatrixMultiply(test, xMatrix);

	src->PC_OBB.ort = test;

	return xMatrix;
}

DirectX::XMMATRIX PhysicsHandler::RotateBB_Y(PhysicsComponent* src, const float &radian)
{
	DirectX::XMMATRIX xMatrix;
	DirectX::XMMATRIX test = src->PC_OBB.ort;

	xMatrix = DirectX::XMMatrixRotationY(radian);
	test = DirectX::XMMatrixMultiply(test, xMatrix);

	src->PC_OBB.ort = test;

	return xMatrix;
}

DirectX::XMMATRIX PhysicsHandler::RotateBB_Z(PhysicsComponent* src, const float &radian)
{
	DirectX::XMMATRIX xMatrix;
	DirectX::XMMATRIX test = src->PC_OBB.ort;

	float rotAngle = (3.14159265359f / 180.0f);

	xMatrix = DirectX::XMMatrixRotationZ(radian);
	test = DirectX::XMMatrixMultiply(test, xMatrix);

	src->PC_OBB.ort = test;

	return xMatrix;
}

void PhysicsHandler::CreateDefaultBB(const DirectX::XMVECTOR & pos, PhysicsComponent * src)
{
	this->CreateDefaultAABB(pos, src);
	this->CreateDefaultOBB(pos, src);
}

void PhysicsHandler::CreateDefaultAABB(const DirectX::XMVECTOR & pos, PhysicsComponent* src)
{
	DirectX::XMFLOAT3 temp;
	DirectX::XMStoreFloat3(&temp, pos);

	OBB* temp2 = &src->PC_OBB;

	//AABB components
	src->PC_AABB.ext[0] = 1.0;
	src->PC_AABB.ext[1] = 1.0;
	src->PC_AABB.ext[2] = 1.0;
}

void PhysicsHandler::CreateDefaultOBB(const DirectX::XMVECTOR & pos, PhysicsComponent* src)
{
	//AABB components

	src->PC_pos = pos;

	src->PC_OBB.ext[0] = 1.0f;
	src->PC_OBB.ext[1] = 1.0f;
	src->PC_OBB.ext[2] = 1.0f;

	src->PC_OBB.ort = DirectX::XMMatrixIdentity();
}

PhysicsHandler::PhysicsHandler()
{
}

PhysicsHandler::~PhysicsHandler()
{
}

bool PhysicsHandler::Initialize()
{
	this->m_gravity = DirectX::XMVectorSet(0.0f, -0.05f, 0.0f, 0.0f);


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

void PhysicsHandler::Update(float deltaTime)
{
	float dt = (deltaTime / 50000);

#pragma region ChainPhysics
	int nrOfChainLinks = this->m_links.size();
	for (int i = 0; i < nrOfChainLinks; i++)
	{
		this->DoChainPhysics(&this->m_links.at(i), dt);
	}
#pragma endregion

#pragma region collitioncheck
	int nrOfObjects = this->m_dynamicComponents.size();
	for (int i = 0; i < (nrOfObjects - this->m_nrOfStaticObjects); i++)
	{
		PhysicsComponent* current = this->m_dynamicComponents.at(i);
		current->PC_normalForce = DirectX::XMVectorSet(0, 0, 0, 0);

		if (current->PC_BVtype == BoundingVolumeType::BV_AABB)
		{
			//only collide with static environment for starters
			for (int j = (nrOfObjects - this->m_nrOfStaticObjects); j < nrOfObjects; j++)
			{
				PhysicsComponent* toCompare = nullptr;
				toCompare = this->m_dynamicComponents.at(j);
				if (toCompare->PC_BVtype == BoundingVolumeType::BV_AABB)
				{
					this->AABBAABBIntersectionTest(current, toCompare, dt);
				}

				if (toCompare->PC_BVtype == BoundingVolumeType::BV_Plane)
				{
					this->SpherePlaneIntersectionTest(current, toCompare, dt);
				}

				if (toCompare->PC_BVtype == BoundingVolumeType::BV_OBB)
				{
					//to be continued

				}

				if (toCompare->PC_BVtype == BoundingVolumeType::BV_Sphere)
				{
					//
				}

			}
		}
	

		DirectX::XMVECTOR pParallel;
		DirectX::XMVECTOR pPerpendicular;

		DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, this->m_gravity, current->PC_normalForce);

		DirectX::XMVECTOR gravityForce = DirectX::XMVectorScale(DirectX::XMVectorScale(DirectX::XMVectorScale(pPerpendicular, (float)current->PC_gravityInfluence),current->PC_mass), 1.0f);
		
		this->ApplyForceToComponent(current, gravityForce, dt);

		if (!current->PC_is_Static)
		{
			float windResistance = 1.0;
			current->PC_pos = DirectX::XMVectorAdd(current->PC_pos, DirectX::XMVectorScale(current->PC_velocity, dt));
			DirectX::XMFLOAT3 temp;
			DirectX::XMStoreFloat3(&temp, current->PC_pos);
			if (temp.y < -7)
			{
				current->PC_pos = DirectX::XMVectorSet(temp.x, 2, temp.z, 0);
				current->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
			}
		}
		else
		{
			current->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
		}
	}
#pragma endregion

	if (this->fileDone == false)
		this->checkCollition();

	for (int i = 0; i < nrOfChainLinks; i++)
	{
		this->AdjustChainLinkPosition(&this->m_links.at(i));
	}
}

void PhysicsHandler::TranslateBB(const DirectX::XMVECTOR &newPos, PhysicsComponent* src)
{
	DirectX::XMFLOAT3 temp;
	DirectX::XMStoreFloat3(&temp, newPos);

	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(temp.x, temp.y, temp.z);
	
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	src->PC_pos = DirectX::XMVector3Transform(src->PC_pos, worldMatrix);
}

void PhysicsHandler::Add_toRotateVec(PhysicsComponent* src)
{
	src->PC_rotation = DirectX::XMVectorAdd(src->PC_rotation, src->PC_rotationVelocity);
}

void PhysicsHandler::DoChainPhysics(ChainLink * link, float dt)
{
	int type = 1;
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(link->CL_previous->PC_pos, link->CL_next->PC_pos);
	float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));
	diffVec = DirectX::XMVector3Normalize(diffVec);

	if (lenght > link->CL_lenght)
	{
		//spring force
		if (type == 0)
		{
			DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(diffVec, lenght - link->CL_lenght), 0.2f);
			//force = DirectX::XMVectorAdd(force, DirectX::XMVectorScale(DirectX::XMVectorSubtract(link->CL_previous->PC_velocity, link->CL_next->PC_velocity), 0.5));

			this->ApplyForceToComponent(link->CL_next, force, dt);
			force = DirectX::XMVectorScale(force, -1.0);
			this->ApplyForceToComponent(link->CL_previous, force, dt);
		}

		//this one seems to work good
		if (type == 1)
		{
			DirectX::XMVECTOR pParallel;
			DirectX::XMVECTOR pPerpendicular1;
			DirectX::XMVECTOR pPerpendicular2;

			float forceVec1[3];
			float forceVec2[3];

			float v1_old[3];
			float v1_new[3];
			float m1 = link->CL_previous->PC_mass;
			DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular1, link->CL_previous->PC_velocity, diffVec);
			v1_old[0] = DirectX::XMVectorGetX(pParallel);
			v1_old[1] = DirectX::XMVectorGetY(pParallel);
			v1_old[2] = DirectX::XMVectorGetZ(pParallel);

			float v2_old[3];
			float v2_new[3];
			float m2 = link->CL_next->PC_mass;
			DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular2, link->CL_next->PC_velocity, diffVec);
			v2_old[0] = DirectX::XMVectorGetX(pParallel);
			v2_old[1] = DirectX::XMVectorGetY(pParallel);
			v2_old[2] = DirectX::XMVectorGetZ(pParallel);

			float e = 0.5;

			for (int i = 0; i < 3; i++)
			{

				v1_new[i] = (v1_old[i] * (m1 - m2*e) + m2*v2_old[i] * (1 + e)) / (m1 + m2);

				v2_new[i] = (v1_old[i] * m1*(1 + e) + (m2 - e*m1)*v2_old[i]) / (m1 + m2);

			}
			v1_new[0] += DirectX::XMVectorGetX(pPerpendicular1);
			v1_new[1] += DirectX::XMVectorGetY(pPerpendicular1);
			v1_new[2] += DirectX::XMVectorGetZ(pPerpendicular1);

			v2_new[0] += DirectX::XMVectorGetX(pPerpendicular2);
			v2_new[1] += DirectX::XMVectorGetY(pPerpendicular2);
			v2_new[2] += DirectX::XMVectorGetZ(pPerpendicular2);
			if (!link->CL_previous->PC_is_Static)
			{
				link->CL_previous->PC_velocity = DirectX::XMVectorSet(v1_new[0], v1_new[1], v1_new[2], 0);
			}
			if (!link->CL_next->PC_is_Static)
			{
				link->CL_next->PC_velocity = DirectX::XMVectorSet(v2_new[0], v2_new[1], v2_new[2], 0);
			}
		}
	}



}

void PhysicsHandler::AdjustChainLinkPosition(ChainLink * link)
{
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(link->CL_next->PC_pos, link->CL_previous->PC_pos);

	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));

	if (distance > link->CL_lenght + 0.1)
	{
		diffVec = DirectX::XMVector3Normalize(diffVec);
		link->CL_next->PC_pos = DirectX::XMVectorAdd(link->CL_previous->PC_pos, DirectX::XMVectorScale(diffVec, (link->CL_lenght + 0.1f)));
		link->CL_next->PC_pos = link->CL_next->PC_pos;
	}

}

void PhysicsHandler::ApplyForceToComponent(PhysicsComponent * componentPtr, DirectX::XMVECTOR force, float dt)
{
	if (!componentPtr->PC_is_Static)
	{
		componentPtr->PC_velocity = DirectX::XMVectorAdd(componentPtr->PC_velocity, DirectX::XMVectorScale(DirectX::XMVectorScale(force, (1 / componentPtr->PC_mass)), dt));
	}
}

PhysicsComponent* PhysicsHandler::CreatePhysicsComponent(const DirectX::XMVECTOR &pos, const bool &isStatic)
{
	PhysicsComponent* newObject = nullptr;
	newObject = new PhysicsComponent;


	newObject->PC_pos = pos;
	newObject->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	newObject->PC_rotation = DirectX::XMVectorSet(0, 0, 0, 0);
	newObject->PC_rotationVelocity = DirectX::XMVectorSet(0, 0, 0, 0);
	newObject->PC_normalForce = DirectX::XMVectorSet(0, 0, 0, 0);
	newObject->PC_active = 1;
	newObject->PC_coolides = true;
	newObject->PC_entityID = 0;
	newObject->PC_is_Static = isStatic;
	newObject->PC_mass = 1.0f;
	newObject->PC_gravityInfluence = 1.0f;
	newObject->PC_Sphere.radius = 1.0f;
	newObject->PC_friction = 1.0f;
	newObject->PC_elasticity = 1.0f;
	newObject->PC_BVtype = BV_AABB;

	this->CreateDefaultBB(pos, newObject);
	this->m_dynamicComponents.push_back(newObject);

	PhysicsComponent* toSwap;
	if (isStatic == false)
	{
		toSwap = this->m_dynamicComponents.at((this->m_dynamicComponents.size() - this->m_nrOfStaticObjects) - 1);
		this->m_dynamicComponents.at((this->m_dynamicComponents.size() - this->m_nrOfStaticObjects) - 1) = newObject;
		this->m_dynamicComponents.at(this->m_dynamicComponents.size() - 1) = toSwap;
	}
	else
	{
		this->m_nrOfStaticObjects++;
	}

	return newObject;
}

void PhysicsHandler::CreateChainLink(int index1, int index2, int nrOfLinks, float linkLenght)
{

	PhysicsComponent* ptr = this->m_dynamicComponents.at(index1);
	PhysicsComponent* previous = this->m_dynamicComponents.at(index1);
	PhysicsComponent* next = nullptr;

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(this->m_dynamicComponents.at(index2)->PC_pos,this->m_dynamicComponents.at(index1)->PC_pos);
	diffVec = DirectX::XMVectorDivide(diffVec, DirectX::XMVectorSet((float)nrOfLinks, (float)nrOfLinks, (float)nrOfLinks, (float)nrOfLinks));

	for (int i = 1; i <= nrOfLinks; i++)
	{
		ChainLink link;
		link.CL_lenght = linkLenght;

		//next = this->CreatePhysicsComponent(DirectX::XMVectorAdd(ptr->PC_pos, DirectX::XMVectorScale(diffVec, i)));
		next = this->CreatePhysicsComponent(DirectX::XMVectorSet(3, 10, 60, 0), false);
		
		next->PC_AABB.ext[0] = 0.1f;
		next->PC_AABB.ext[1] = 0.1f;
		next->PC_AABB.ext[2] = 0.1f;

		link.CL_previous = previous;
		link.CL_next = next;
		this->m_links.push_back(link);
		previous = next;
	}
	ChainLink link;
	link.CL_lenght = linkLenght;

	next = this->m_dynamicComponents.at(index2);
	link.CL_previous = previous;
	link.CL_next = next;
	this->m_links.push_back(link);
}

bool PhysicsHandler::IntersectRayOBB(const DirectX::XMVECTOR & rayOrigin, const DirectX::XMVECTOR & rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos)
{
	Ray ray;
	ray.Origin = rayOrigin;
	ray.RayDir = rayDir;


	float t1, t2 = 0.0;
	const int NR_OF_NORMALS = 3;
	
	//Vec rayD = ray.d;
	DirectX::XMVECTOR radD = ray.RayDir;

	DirectX::XMVECTOR sideVector[NR_OF_NORMALS];

	sideVector[0] = obj.ort.r[0];
	sideVector[1] = obj.ort.r[1];
	sideVector[2] = obj.ort.r[2];

	float tMin;
	float tMax;

	tMin = -INFINITY;
	tMax = INFINITY;

	//Vec pointVec = this->Bcenter - ray.o;
	DirectX::XMVECTOR pointVec = DirectX::XMVectorSubtract(obbPos, ray.Origin);

	//rayD.Normalize();
	ray.RayDir = DirectX::XMVector3Normalize(radD);

	float temp;
	float length[NR_OF_NORMALS];

	length[0] = obj.ext[0];
	length[1] = obj.ext[1];
	length[2] = obj.ext[2];

	float e = 0.0f;
	float f = 0.0f;

	for (int i = 0; i < NR_OF_NORMALS; i++)
	{

		e = this->DotProduct(sideVector[i], pointVec);
		f = this->DotProduct(sideVector[i], ray.RayDir);

		if (abs(f) > 1e-20f)
		{
			t1 = (e + length[i]) / f;
			t2 = (e - length[i]) / f;

			if (t1 > t2)
			{
				//swap
				temp = t2;
				t2 = t1;
				t1 = temp;
			}
			if (t1 > tMin)
			{
				tMin = t1;
			}
			if (t2 < tMax)
			{
				tMax = t2;
			}
			if (tMin > tMax)
			{
				return false;
			}
			if (tMax < 0)
			{
				return false;
			}
		}
		else if ((-e - length[i]) > 0 || (-e + length[i] < 0))
		{
			return false;
		}
	}

	DirectX::XMVECTOR totalRay;
	if (tMin > 0)
	{
		//min intersect
		//return tMin
	}
	else
	{
		//max intersect
		//return tMax;
	}

	/*
	if (tMin > 0)
	{
		// min intersect
		if (tMin < hit.t || hit.t == -1)
		{
			hit.t = tMin;
			hit.lastShape = this;
			pointVec = ray.o + ray.d*tMin; //calc point were it hits
			hit.lastNormal = this->normal(pointVec);
			//hit.color = this->c;
		}
	}
	else
	{
		//max intersect
		if (tMax < hit.t || hit.t == -1)
		{
			hit.t = tMax;
			hit.lastShape = this;
			pointVec = ray.o + ray.d*tMax; //calc point were it hits
			hit.lastNormal = this->normal(pointVec);
			//hit.color = this->c;
		}
	}
	*/
	return true;
}

bool PhysicsHandler::IntersectRayOBB(const DirectX::XMVECTOR & rayOrigin, const DirectX::XMVECTOR & rayDir, const OBB & obj, const DirectX::XMVECTOR & obbPos, float & distanceToOBB)
{
	Ray ray;
	ray.Origin = rayOrigin;
	ray.RayDir = rayDir;


	float t1, t2 = 0.0;
	const int NR_OF_NORMALS = 3;

	//Vec rayD = ray.d;
	DirectX::XMVECTOR radD = ray.RayDir;

	DirectX::XMVECTOR sideVector[NR_OF_NORMALS];

	sideVector[0] = obj.ort.r[0];
	sideVector[1] = obj.ort.r[1];
	sideVector[2] = obj.ort.r[2];

	float tMin;
	float tMax;

	tMin = -INFINITY;
	tMax = INFINITY;

	//Vec pointVec = this->Bcenter - ray.o;
	DirectX::XMVECTOR pointVec = DirectX::XMVectorSubtract(obbPos, ray.Origin);

	//rayD.Normalize();
	ray.RayDir = DirectX::XMVector3Normalize(radD);

	float temp;
	float length[NR_OF_NORMALS];

	length[0] = obj.ext[0];
	length[1] = obj.ext[1];
	length[2] = obj.ext[2];

	float e = 0.0f;
	float f = 0.0f;

	distanceToOBB = 0;
	for (int i = 0; i < NR_OF_NORMALS; i++)
	{

		e = this->DotProduct(sideVector[i], pointVec);
		f = this->DotProduct(sideVector[i], ray.RayDir);

		if (abs(f) > 1e-20f)
		{
			t1 = (e + length[i]) / f;
			t2 = (e - length[i]) / f;

			if (t1 > t2)
			{
				//swap
				temp = t2;
				t2 = t1;
				t1 = temp;
			}
			if (t1 > tMin)
			{
				tMin = t1;
			}
			if (t2 < tMax)
			{
				tMax = t2;
			}
			if (tMin > tMax)
			{
				return false;
			}
			if (tMax < 0)
			{
				return false;
			}
		}
		else if ((-e - length[i]) > 0 || (-e + length[i] < 0))
		{
			return false;
		}
	}

	if (tMin > 0)
	{
		//min intersect
		distanceToOBB = tMin;
	}
	else
	{
		//max intersect
		distanceToOBB = tMax;
	}

	return true;
}

void PhysicsHandler::SimpleCollition(float dt)
{
	float m_frictionConstant = 0.999f;
	PhysicsComponent* ptr;
	int size = this->m_dynamicComponents.size();
	for (int i = 0; i < size; i++)
	{
		ptr = this->m_dynamicComponents.at(i);


		DirectX::XMVECTOR pos = ptr->PC_pos;

		float y = DirectX::XMVectorGetY(pos);

		if (y > (0 + this->m_offSet))
		{
			SimpleGravity(ptr, dt);
		}
		else if (y < (0 + this->m_offSet))
		{
			ptr->PC_pos =  (DirectX::XMVectorSet(DirectX::XMVectorGetX(pos), (0 + this->m_offSet), DirectX::XMVectorGetZ(pos), 0.0f));
			DirectX::XMVECTOR vel = ptr->PC_velocity;
			ptr->PC_velocity = (DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
		}
		else if (y == (0 + this->m_offSet))
		{
			DirectX::XMVECTOR vel = ptr->PC_velocity;
			ptr->PC_velocity = (DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0.0f, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
		}

		ptr->PC_pos = DirectX::XMVectorAdd(ptr->PC_pos, DirectX::XMVectorScale(ptr->PC_velocity, dt));
	}
}

void PhysicsHandler::SimpleGravity(PhysicsComponent* componentPtr, const float &dt)
{
	DirectX::XMVECTOR test = DirectX::XMVECTOR();
	DirectX::XMFLOAT3 testRes(0, 5, 0);
	test = DirectX::XMLoadFloat3(&testRes);


	componentPtr->PC_velocity = DirectX::XMVectorAdd(componentPtr->PC_velocity, this->m_gravity);

}

int PhysicsHandler::getNrOfComponents()const
{
	return this->m_dynamicComponents.size();
}

PhysicsComponent* PhysicsHandler::getDynamicComponentAt(int index)const
{
	if (index >= 0 && index < (signed int)this->m_dynamicComponents.size())
	{
		return this->m_dynamicComponents.at(index);
	}
	return nullptr;

}

void PhysicsHandler::SetBB_Rotation(const DirectX::XMVECTOR &rotVec, PhysicsComponent* toRotate)
{
	toRotate->PC_rotation = rotVec;
}

bool PhysicsHandler::checkCollition()
{
	bool result = false;

	std::chrono::time_point<std::chrono::system_clock>start;
	std::chrono::time_point<std::chrono::system_clock>end;

	

	std::chrono::duration<double>elapsed_time;
	std::time_t end_time;
	
	std::ofstream myFile;
	myFile.open("performanceSweepTest.txt");

	int i = 0;

	while (i++ < 2000)
	{
		start = std::chrono::system_clock::now();
		result = this->IntersectAABB();
		end = std::chrono::system_clock::now();

		elapsed_time = end - start;
		end_time = std::chrono::system_clock::to_time_t(end);
		elapsed_time *= 1000; //millisecounds

		float test = elapsed_time.count();
		myFile << test << std::endl;
	}
	myFile.close();
	this->fileDone = true;

	return result;
}
#ifdef _DEBUG
void PhysicsHandler::GetPhysicsComponentOBB(OBB*& src, int index)
{
	src = &(this->m_dynamicComponents.at(index)->PC_OBB);
}

void PhysicsHandler::GetPhysicsComponentAABB(AABB*& src, int index)
{
	src = &(this->m_dynamicComponents.at(index)->PC_AABB);
}

void PhysicsHandler::GetPhysicsComponentPlane(Plane*& src, int index)
{
	src = &(this->m_dynamicComponents.at(index)->PC_Plane);
}
#endif // _DEBUG
