#include "PhysicsHandler.h"
#include <iostream>
#include <chrono>
#include <ctime>


void PhysicsHandler::IntersectionTesting()
{
	PhysicsComponent sphere;
	sphere.PC_pos = DirectX::XMVectorSet(4, 10, 0, 0);
	sphere.PC_Sphere.radius = 3.0f;
	PhysicsComponent AABB;
	AABB.PC_pos = DirectX::XMVectorSet(0, 10, 0, 0);
	AABB.PC_AABB.ext[0] = 1.0f;
	AABB.PC_AABB.ext[1] = 1.0f;
	AABB.PC_AABB.ext[2] = 1.0f;

	bool result = false;
	result = this->SphereAABBIntersectionTest(&sphere, &AABB);

	PhysicsComponent sphere2;
	sphere2.PC_pos = DirectX::XMVectorSet(-1, 10, 0, 0);
	sphere2.PC_Sphere.radius = 1.0f;

	result = this->SphereSphereIntersectionTest(&sphere, &sphere2);

	PhysicsComponent plane;
	plane.PC_pos = DirectX::XMVectorSet(0, 10, 0, 0);
	plane.PC_Plane.PC_normal = DirectX::XMVectorSet(1, 0, 0, 0);

	result = this->SpherePlaneIntersectionTest(&sphere2, &plane);

	result = this->AABBPlaneIntersectionTest(&AABB, &plane);

	PhysicsComponent OBB;
	OBB.PC_pos = DirectX::XMVectorSet(-1, 10, 0, 0);
	OBB.PC_OBB.ext[0] = 1.0f;
	OBB.PC_OBB.ext[1] = 1.0f;
	OBB.PC_OBB.ext[2] = 1.0f;

	DirectX::XMVECTOR axis1 = DirectX::XMVectorSet(1, 0, 0, 0);
	DirectX::XMVECTOR axis2 = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR axis3 = DirectX::XMVectorSet(0, 0, 1, 0);

	DirectX::XMMATRIX rot;
	rot = DirectX::XMMatrixRotationZ((3.14 / 180) * 33);
	OBB.PC_OBB.ort.r[0] = DirectX::XMVector3Transform(axis1, rot);
	OBB.PC_OBB.ort.r[1] = DirectX::XMVector3Transform(axis2, rot);
	OBB.PC_OBB.ort.r[2] = DirectX::XMVector3Transform(axis3, rot);

	result = this->OBBPlaneIntersectionTest(&OBB, &plane);

	result = this->SphereOBBIntersectionTest(&sphere, &OBB);

}

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

bool PhysicsHandler::SpherePlaneIntersectionTest(PhysicsComponent * objSphere, PhysicsComponent * objPlane)
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

		DirectX::XMVECTOR resultVel = objSphere->PC_velocity;
		resultVel = DirectX::XMVector3Reflect(objSphere->PC_velocity, objPlane->PC_Plane.PC_normal);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(objSphere->PC_velocity, objPlane->PC_Plane.PC_normal)) < 0)
		{
			//DirectX::XMVECTOR resultVel = DirectX::XMVector3Reflect(objSphere->PC_velocity, objPlane->PC_Plane.PC_normal);
		}

		DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, resultVel, objPlane->PC_Plane.PC_normal);

		pParallel = DirectX::XMVectorScale(pParallel, objPlane->PC_elasticity);
		pPerpendicular = DirectX::XMVectorScale(pPerpendicular, objPlane->PC_friction);

		resultVel = DirectX::XMVectorAdd(pParallel, pPerpendicular);

		//resultVel = DirectX::XMVectorScale(resultVel, 0.3);
		//objOBB->PC_pos = DirectX::XMVectorAdd(objOBB->PC_pos, pParallel);
		objSphere->PC_velocity = resultVel;


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
		resultVel = DirectX::XMVectorScale(resultVel, 0.7);
		//objOBB->PC_pos = DirectX::XMVectorAdd(objOBB->PC_pos, pParallel);
		objOBB->PC_velocity = resultVel;
	}

	return result;
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

float PhysicsHandler::CrossProductf(const DirectX::XMVECTOR & v1, const DirectX::XMVECTOR & v2) const
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

void PhysicsHandler::UpdateAABB(PhysicsComponent* src)
{

}

DirectX::XMMATRIX PhysicsHandler::RotateBB_X(PhysicsComponent* src, const float &radian)
{
	DirectX::XMMATRIX xMatrix;
	DirectX::XMMATRIX test = src->PC_OBB.ort;

	//read the value of PC_rotation
	//DirectX::XMStoreFloat3(&rot,src->PC_rotation);
	float rotAngle = (3.14159265359 / 180.0);

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

	float rotAngle = (3.14159265359 / 180.0);

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
	src->PC_AABB.ext[0] = 1.0 + 1.0f;
	src->PC_AABB.ext[1] = 1.0 + 1.0f;
	src->PC_AABB.ext[2] = 1.0 + 1.0f;
}

void PhysicsHandler::CreateDefaultOBB(const DirectX::XMVECTOR & pos, PhysicsComponent* src)
{
	//AABB components

	src->PC_OBB.pos = pos;

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
	DirectX::XMVECTOR tempPos = DirectX::XMVectorSet(0, 5, 45, 0);
	DirectX::XMVECTOR tempPos2 = DirectX::XMVectorSet(2, 1, 45, 0);

	PhysicsComponent* ptr = nullptr;
	ptr = this->CreatePhysicsComponent(tempPos);
	ptr->PC_mass = 20;
	ptr->PC_is_Static = false;
	ptr->PC_active = 1;
	//ptr->PC_velocity = DirectX::XMVectorSet(-0.5, 0.3, 0.0, 0);

	ptr = this->CreatePhysicsComponent(tempPos);
	//ptr->PC_pos = DirectX::XMVectorSet(-2, 0, 0, 0);
	ptr->PC_mass = 5;
	ptr->PC_is_Static = false;
	ptr->PC_active = 1;
	//ptr->PC_velocity = DirectX::XMVectorSet(-0.5, 0.3, 0.0, 0);

	ptr = this->CreatePhysicsComponent(tempPos);
	ptr->PC_mass = 5.0f;
	ptr->PC_is_Static = false;
	ptr->PC_active = 1;
	//ptr->PC_velocity = DirectX::XMVectorSet(-0.5, 0.3, 0.0, 0);

	ptr = this->CreatePhysicsComponent(tempPos);
	ptr->PC_mass = 5.0f;
	ptr->PC_is_Static = false;
	ptr->PC_active = 1;
	//ptr->PC_velocity = DirectX::XMVectorSet(-0.5, 0.3, 0.0, 0);

	ptr = this->CreatePhysicsComponent(tempPos2);
	ptr->PC_velocity = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	ptr->PC_active = 1;
	ptr->PC_mass = 10;
	ptr->PC_velocity = DirectX::XMVectorSet(0.0, 0, 0.0, 0);


	int size = this->m_dynamicComponents.size();
	this->InitializeChain(0, size);

	this->m_chain.CH_linkLenght = 3.0f;


	this->m_gravity = DirectX::XMVectorSet(0, -0.06, 0, 0);

	//Axels HOUSE
	float houseFriction = 0.7;
	float houseElasticity = 0.1;
	
	this->m_floor.PC_pos = DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0);
	this->m_floor.PC_Plane.PC_normal = DirectX::XMVectorSet(0, 1.0, 0, 0);
	this->m_floor.PC_friction = houseFriction;
	this->m_floor.PC_elasticity = houseElasticity;

	//this->m_wall1.PC_pos = DirectX::XMVectorSet(25.0, 0.0, 0.0, 0.0);
	//this->m_wall1.PC_Plane.PC_normal = DirectX::XMVectorSet(-1.0, 0.0, 0, 0);
	//this->m_wall1.PC_friction = houseFriction;
	//this->m_wall1.PC_elasticity = houseElasticity;
	//
	//this->m_wall2.PC_pos = DirectX::XMVectorSet(-25.0, 0.0, 0.0, 0.0);
	//this->m_wall2.PC_Plane.PC_normal = DirectX::XMVectorSet(1.0, 0.0, 0, 0);
	//this->m_wall2.PC_friction = houseFriction;
	//this->m_wall2.PC_elasticity = houseElasticity;
	//
	//this->m_wall3.PC_pos = DirectX::XMVectorSet(0.0, 0.0, 30.0, 0.0);
	//this->m_wall3.PC_Plane.PC_normal = DirectX::XMVectorSet(0.0, 0.0, -1.0, 0);
	//this->m_wall3.PC_friction = houseFriction;
	//this->m_wall3.PC_elasticity = houseElasticity;
	//
	//this->m_wall4.PC_pos = DirectX::XMVectorSet(0.0, 0.0, -1.0, 0.0);
	//this->m_wall4.PC_Plane.PC_normal = DirectX::XMVectorSet(0.0, 0.0, 1.0, 0);
	//this->m_wall4.PC_friction = houseFriction;
	//this->m_wall4.PC_elasticity = houseElasticity;
	//
	//this->m_roof.PC_pos = DirectX::XMVectorSet(0.0, 15.0, 0.0, 0.0);
	//this->m_roof.PC_Plane.PC_normal = DirectX::XMVectorSet(0.0, -1.0, 0.0, 0);
	//this->m_roof.PC_friction = houseFriction;
	//this->m_roof.PC_elasticity = houseElasticity;



	DirectX::XMVECTOR test1 = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR test2 = DirectX::XMVectorSet(1, 1, 0, 0);
	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(test2, test1)) < 0)
	{
		test2 = DirectX::XMVector3Reflect(test2, test1);
	}


	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR pPerpendicular;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, test2, test1);

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
	float dt = (deltaTime / 30000);
	

	int nrOfChainObjects = this->m_chain.CH_links.size();
	for (int i = 0; i < nrOfChainObjects - 1; i++)
	{
		this->DoChainPhysics(this->m_chain.CH_links.at(i), this->m_chain.CH_links.at(i + 1), dt);
	}

	int nrOfObjects = this->m_dynamicComponents.size();
	for (int i = 0; i < nrOfObjects; i++)
	{
		PhysicsComponent* current = this->m_dynamicComponents.at(i);
		current->PC_normalForce = DirectX::XMVectorSet(0, 0, 0, 0);

		//Axels house intersection
		//this->SpherePlaneIntersectionTest(current, &this->m_wall1);
		//this->SpherePlaneIntersectionTest(current, &this->m_wall2);
		//this->SpherePlaneIntersectionTest(current, &this->m_wall3);
		//this->SpherePlaneIntersectionTest(current, &this->m_wall4);
		//this->SpherePlaneIntersectionTest(current, &this->m_roof);
		bool floorTest = false;
		floorTest = this->SpherePlaneIntersectionTest(current, &this->m_floor);
		if (floorTest == false)
		{
			//current->PC_velocity = DirectX::XMVectorAdd(current->PC_velocity, this->m_gravity);
		}
		else
		{
			//current->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
		}
		DirectX::XMVECTOR pParallel;
		DirectX::XMVECTOR pPerpendicular;

		DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, this->m_gravity, current->PC_normalForce);

		current->PC_velocity = DirectX::XMVectorAdd(current->PC_velocity, DirectX::XMVectorScale(DirectX::XMVectorScale(pPerpendicular, current->PC_gravityInfluence), dt));

		if (!current->PC_is_Static)
		{
			current->PC_pos = DirectX::XMVectorAdd(current->PC_pos, DirectX::XMVectorScale(current->PC_velocity, dt));
			DirectX::XMFLOAT3 temp;
			DirectX::XMStoreFloat3(&temp, current->PC_pos);
			if (temp.y < -7)
			{
				current->PC_pos = DirectX::XMVectorSet(0, 1, 7, 0);
			}
			current->PC_OBB.pos = current->PC_pos;
		}
	}

	
	this->AdjustChainLinkPosition();
	//SimpleCollition(dt);
}

void PhysicsHandler::InitializeChain(int start, int end)
{
	for (int i = start; i < end; i++)
	{
		this->m_chain.CH_links.push_back(this->m_dynamicComponents.at(i));
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

void PhysicsHandler::DoChainPhysics(PhysicsComponent * current, PhysicsComponent * next, float dt)
{
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(current->PC_pos, next->PC_pos);
	float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));

	if (lenght > this->m_chain.CH_linkLenght)
	{
		diffVec = DirectX::XMVector3Normalize(diffVec);

		DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(diffVec, lenght - this->m_chain.CH_linkLenght), 0.2);
		//force = DirectX::XMVectorAdd(force, DirectX::XMVectorScale(DirectX::XMVectorSubtract(current->m_velocity, next->m_velocity), 0.5));

		next->PC_velocity = DirectX::XMVectorAdd(next->PC_velocity, DirectX::XMVectorScale(DirectX::XMVectorScale(force, (1.0 / next->PC_mass)), 1.0));
		force = DirectX::XMVectorScale(force, -1.0);
		current->PC_velocity = DirectX::XMVectorAdd(current->PC_velocity, DirectX::XMVectorScale(DirectX::XMVectorScale(force, (1.0 / current->PC_mass)), 1.0));
	}
}

void PhysicsHandler::AdjustChainLinkPosition()
{
	int size = this->m_chain.CH_links.size();;
	PhysicsComponent* current;
	PhysicsComponent* next;
	for (int i = 0; i < size - 1; i++)
	{
		current = this->m_chain.CH_links.at(i);
		next = this->m_chain.CH_links.at(i + 1);
		DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(next->PC_pos, current->PC_pos);
		//diffVec = DirectX::XMVectorSubtract(next->PC_pos, current->PC_pos);
		float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));
		diffVec = DirectX::XMVector3Normalize(diffVec);

		if (lenght > this->m_chain.CH_linkLenght +0.5)
		{
			next->PC_pos = DirectX::XMVectorAdd(current->PC_pos, DirectX::XMVectorScale(diffVec, (this->m_chain.CH_linkLenght + 0.5)));
			next->PC_OBB.pos = next->PC_pos;
		}
	}
}

PhysicsComponent* PhysicsHandler::CreatePhysicsComponent(const DirectX::XMVECTOR &pos)
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
	newObject->PC_is_Static = false;
	newObject->PC_mass = 5.0f;
	newObject->PC_gravityInfluence = 1.0f;
	newObject->PC_Sphere.radius = 1.0f;
	newObject->PC_friction = 1.0f;
	newObject->PC_elasticity = 1.0f;

	this->CreateDefaultBB(pos, newObject);
	this->m_dynamicComponents.push_back(newObject);

	return newObject;
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

	//sideVector[0] = this->Bu; // slabs Bu
	//sideVector[1] = this->Bv; // slabs Bv
	//sideVector[2] = this->Bw; // slabs Bw

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

		e = this->CrossProductf(sideVector[i], pointVec);
		f = this->CrossProductf(sideVector[i], ray.RayDir);

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

PhysicsComponent* PhysicsHandler::getDynamicComponents(int index)const
{
	if (index >= 0 && index < this->m_dynamicComponents.size())
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

	start = std::chrono::system_clock::now();
	result = this->IntersectAABB();
	end = std::chrono::system_clock::now();

	std::chrono::duration<double>elapsed_secounds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	return result;
}

void PhysicsHandler::GetPhysicsComponentOBB(OBB*& src, int index)
{
	src = &(this->m_dynamicComponents.at(index)->PC_OBB);
}

void PhysicsHandler::GetPhysicsComponentAABB(AABB*& src, int index)
{
	src = &(this->m_dynamicComponents.at(index)->PC_AABB);
}

PhysicsComponent* PhysicsHandler::GetTempFloor()
{
	return &this->m_floor;
}