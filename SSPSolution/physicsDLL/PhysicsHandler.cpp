#include "PhysicsHandler.h"
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
	bool result = false;

	DirectX::XMFLOAT3 temp;
	DirectX::XMFLOAT3 temp2;



	int nrOfComponents = this->m_physicsComponents.size();
	float vecToObj[3];

	for (int i = 0; i < (nrOfComponents - this->m_nrOfStaticObjects); i++)
	{
		PC_toCheck = this->m_physicsComponents.at(i);
		DirectX::XMStoreFloat3(&temp, PC_toCheck->PC_pos);

		for (int j = i + 1; j < nrOfComponents; j++)
		{
			PC_ptr = this->m_physicsComponents.at(j);
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


	int nrOfComponents = this->m_physicsComponents.size();
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

		DirectX::XMVECTOR correction = DirectX::XMVectorAdd(obj1->PC_pos, obj2->PC_pos);
		correction = DirectX::XMVector4Normalize(correction);

		float procentMargin = 0.997;

		float yCorrection = 0;
		float correctionMargin = 0.4;

		bool noCollision = false;

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
					DirectX::XMVECTOR normal = DirectX::XMVectorSet(0, 0, 0, 0);
					float xProcent = fabs(xOverlap / x_total_ext);
					float yProcent = fabs(yOverlap / y_total_ext);
					float zProcent = fabs(zOverlap / z_total_ext);

					float highLimit = 0.1;
					float diff = 0;

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
						if (!obj1->PC_steadfast)
						{

							//obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, DirectX::XMVectorSet(distanceToMove, 0, 0, 0)); // old

							float obj1YMin = DirectX::XMVectorGetY(obj1->PC_pos) - obj1->PC_AABB.ext[1];
							float obj2YMax = DirectX::XMVectorGetY(obj2->PC_pos) + obj2->PC_AABB.ext[1];
							diff = fabs(obj1YMin - obj2YMax);

							if (diff < highLimit)
							{
								normal = DirectX::XMVectorSet(0, 1, 0, 0);
								yCorrection = diff;
								distanceToMove = 0;
							}

							obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, DirectX::XMVectorSet(distanceToMove, yCorrection, 0, 0));
						}
						else if (!obj2->PC_steadfast && !obj2->PC_is_Static)
						{

							//obj2->PC_pos = DirectX::XMVectorAdd(obj2->PC_pos, DirectX::XMVectorSet(-distanceToMove, 0, 0, 0)); // old

							float obj2YMin = DirectX::XMVectorGetY(obj2->PC_pos) - obj2->PC_AABB.ext[1];
							float obj1YMax = DirectX::XMVectorGetY(obj1->PC_pos) + obj1->PC_AABB.ext[1];
							diff = fabs(obj2YMin - obj1YMax);

							if (diff < highLimit)
							{
								yCorrection = diff;
								distanceToMove = 0;
							}

							obj2->PC_pos = DirectX::XMVectorAdd(obj2->PC_pos, DirectX::XMVectorSet(distanceToMove, yCorrection, 0, 0));
						}
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
						if (!obj1->PC_steadfast)
						{
							obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, DirectX::XMVectorSet(0, distanceToMove, 0, 0));
						}
						else if (!obj2->PC_steadfast && !obj2->PC_is_Static)
						{
							obj2->PC_pos = DirectX::XMVectorAdd(obj2->PC_pos, DirectX::XMVectorSet(0, -distanceToMove, 0, 0));
						}
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
						if (!obj1->PC_steadfast)
						{


							//obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, DirectX::XMVectorSet(0, 0, distanceToMove, 0)); // old

							float obj1YMin = DirectX::XMVectorGetY(obj1->PC_pos) - obj1->PC_AABB.ext[1];
							float obj2YMax = DirectX::XMVectorGetY(obj2->PC_pos) + obj2->PC_AABB.ext[1];

							diff = fabs(obj1YMin - obj2YMax);

							if (diff < highLimit)
							{
								yCorrection = diff;
								distanceToMove = 0;
								normal = DirectX::XMVectorSet(0, 1, 0, 0);
							}

							obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, DirectX::XMVectorSet(0, yCorrection, distanceToMove, 0));

						}
						else if (!obj2->PC_steadfast && !obj2->PC_is_Static)
						{

							
							//obj2->PC_pos = DirectX::XMVectorAdd(obj2->PC_pos, DirectX::XMVectorSet(0, 0, -distanceToMove, 0)); // old

							float obj2YMin = DirectX::XMVectorGetY(obj2->PC_pos) - obj2->PC_AABB.ext[1];
							float obj1YMax = DirectX::XMVectorGetY(obj1->PC_pos) + obj1->PC_AABB.ext[1];

							diff = fabs(obj2YMin - obj1YMax);

							if (diff < highLimit)
							{
								yCorrection = diff;
								distanceToMove = 0;
							}

							obj2->PC_pos = DirectX::XMVectorAdd(obj2->PC_pos, DirectX::XMVectorSet(0, yCorrection, distanceToMove, 0));
						}
					}
					//obj1->PC_pos = DirectX::XMVectorAdd(obj1->PC_pos, correction);
					this->CollitionDynamics(obj1, obj2, normal, dt);
					obj1->PC_normalForce = normal;

					//overlappY

					//overlappZ



				}
			}
		}
	return result;
}

bool PhysicsHandler::ObbObbIntersectionTest(PhysicsComponent* objA, PhysicsComponent* objB, bool doPhysics, float dt)
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
	transPF_v = this->VectorSubstract(posB, posA);


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

		rB = b->ext[i];

		t = fabs(T[0] * R.m[0][i] + T[1] * R.m[1][i] + T[2] * R.m[2][i]);

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
	t = fabs(T[1] * R.m[0][1] - T[0] * R.m[1][1]);
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
	if (doPhysics)
	{
		this->ObbObbCollitionCorrection(objA, objB, dt);
	}

	return true;
}

bool PhysicsHandler::OBBAABBIntersectionTest(PhysicsComponent * objOBB, PhysicsComponent * objAABB, float dt)
{
	//this function just convertes the AABB in current and makes a temporary OBB that is the same size 
	//and then does OBBVSOBB intersection test.
	//IMPORTANT the second PhysicsComponent has to be the AABB
	bool result = false;



	PhysicsComponent OBBconverted;
	OBBconverted.PC_BVtype = BV_OBB;
	OBBconverted.PC_is_Static = objAABB->PC_is_Static;
	OBBconverted.PC_pos = objAABB->PC_pos;
	OBBconverted.PC_velocity = objAABB->PC_velocity;
	OBBconverted.PC_OBB.ort = DirectX::XMMatrixIdentity();
	OBBconverted.PC_OBB.ext[0] = objAABB->PC_AABB.ext[0];
	OBBconverted.PC_OBB.ext[1] = objAABB->PC_AABB.ext[1];
	OBBconverted.PC_OBB.ext[2] = objAABB->PC_AABB.ext[2];
	OBBconverted.PC_elasticity = objAABB->PC_elasticity;
	OBBconverted.PC_friction = objAABB->PC_friction;

	DirectX::XMVECTOR originalPos = objOBB->PC_pos;
	DirectX::XMVECTOR originalVel = objOBB->PC_velocity;

	result = this->ObbObbIntersectionTest(objOBB, &OBBconverted, true, dt);

	if (result)
	{
		int a = 0;
		//DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(objAABB->PC_pos, objOBB->PC_pos);
		//float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(diffVec, DirectX::XMVectorSet(0, 1, 0, 0)));
		//DirectX::XMVECTOR downVec = DirectX::XMVectorScale(DirectX::XMVectorSet(0, 1, 0, 0), dot);
		//float yMin = DirectX::XMVectorGetY(DirectX::XMVectorSubtract(objOBB->PC_pos, downVec));
		//float yMax = DirectX::XMVectorGetY(objAABB->PC_pos) + objAABB->PC_AABB.ext[1];

		//float diff = fabs(yMin - yMax);
		//if (diff < 0.499)
		//{
		//	objOBB->PC_velocity = originalVel;
		//	objOBB->PC_pos = DirectX::XMVectorAdd(originalPos, DirectX::XMVectorSet(0, diff, 0, 0));
		//}

	}

	objAABB->PC_pos = OBBconverted.PC_pos;
	objAABB->PC_velocity = OBBconverted.PC_velocity;

	return result;
}

void PhysicsHandler::ObbObbCollitionCorrection(PhysicsComponent * obj1, PhysicsComponent * obj2, float dt)
{
	DirectX::XMVECTOR collitionPoint;
	collitionPoint = this->FindCollitionPoint(obj1, obj2, dt);
}

DirectX::XMVECTOR PhysicsHandler::FindCollitionPoint(PhysicsComponent * obj1, PhysicsComponent * obj2, float dt)
{
	OBB* A = &obj1->PC_OBB;
	OBB* B = &obj2->PC_OBB;

	//get the position of the objects
	DirectX::XMVECTOR Apos = obj1->PC_pos;
	DirectX::XMVECTOR Bpos = obj2->PC_pos;

	//vector holding  the vector to obj1 to obj2
	DirectX::XMVECTOR BtoAvector;
	BtoAvector = DirectX::XMVectorSubtract(Apos, Bpos);

	//vector holding the vector to obj2 to obj1
	DirectX::XMVECTOR AtoBvector;
	AtoBvector = DirectX::XMVectorSubtract(Bpos, Apos);

	DirectX::XMFLOAT3 scalarA(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 scalarB(0.0f, 0.0f, 0.0f);

	//project this vector onto the axises of the obb of A
	DirectX::XMStoreFloat(&scalarA.x, DirectX::XMVector3Dot(AtoBvector, A->ort.r[0]));
	DirectX::XMStoreFloat(&scalarA.y, DirectX::XMVector3Dot(AtoBvector, A->ort.r[1]));
	DirectX::XMStoreFloat(&scalarA.z, DirectX::XMVector3Dot(AtoBvector, A->ort.r[2]));

	//check the projected scalar against extended
	if (scalarA.x > A->ext[0])
	{
		scalarA.x = A->ext[0];
	}
	else if (scalarA.x < -A->ext[0])
	{
		scalarA.x = -A->ext[0];
	}

	if (scalarA.y > A->ext[1])
	{
		scalarA.y = A->ext[1];
	}

	else if (scalarA.y < -A->ext[1])
	{
		scalarA.y = -A->ext[1];
	}

	if (scalarA.z > A->ext[2])
	{
		scalarA.z = A->ext[2];
	}
	else if (scalarA.z < -A->ext[2])
	{
		scalarA.z = -A->ext[2];
	}


	//project this vector onto the axises of the obb of B
	DirectX::XMStoreFloat(&scalarB.x, DirectX::XMVector3Dot(BtoAvector, B->ort.r[0]));
	DirectX::XMStoreFloat(&scalarB.y, DirectX::XMVector3Dot(BtoAvector, B->ort.r[1]));
	DirectX::XMStoreFloat(&scalarB.z, DirectX::XMVector3Dot(BtoAvector, B->ort.r[2]));

	if (scalarB.x > B->ext[0])
	{
		scalarB.x = B->ext[0];
	}
	else if (scalarB.x < -B->ext[0])
	{
		scalarB.x = -B->ext[0];
	}

	if (scalarB.y > B->ext[1])
	{
		scalarB.y = B->ext[1];
	}

	else if (scalarB.y < -B->ext[1])
	{
		scalarB.y = -B->ext[1];
	}

	if (scalarB.z > B->ext[2])
	{
		scalarB.z = B->ext[2];
	}
	else if (scalarB.z < -B->ext[2])
	{
		scalarB.z = -B->ext[2];
	}

	DirectX::XMVECTOR pointOfIntersectA;
	pointOfIntersectA = Apos;

	DirectX::XMVECTOR pointOfIntersectB;
	pointOfIntersectB = Bpos;

	pointOfIntersectA = DirectX::XMVectorAdd(pointOfIntersectA, DirectX::XMVectorScale(A->ort.r[0], scalarA.x));
	pointOfIntersectA = DirectX::XMVectorAdd(pointOfIntersectA, DirectX::XMVectorScale(A->ort.r[1], scalarA.y));
	pointOfIntersectA = DirectX::XMVectorAdd(pointOfIntersectA, DirectX::XMVectorScale(A->ort.r[2], scalarA.z));

	pointOfIntersectB = DirectX::XMVectorAdd(pointOfIntersectB, DirectX::XMVectorScale(B->ort.r[0], scalarB.x));
	pointOfIntersectB = DirectX::XMVectorAdd(pointOfIntersectB, DirectX::XMVectorScale(B->ort.r[1], scalarB.y));
	pointOfIntersectB = DirectX::XMVectorAdd(pointOfIntersectB, DirectX::XMVectorScale(B->ort.r[2], scalarB.z));

	bool pointOfIntersectionBInA = true;
	pointOfIntersectionBInA = this->IsPointInBox(pointOfIntersectB, A, Apos);

	bool pointOfIntersectionAInB = true;
	pointOfIntersectionAInB = this->IsPointInBox(pointOfIntersectA, B, Bpos);

	DirectX::XMVECTOR result = DirectX::XMVectorSet(0, 0, 0, 0);
	if (pointOfIntersectionAInB == true && pointOfIntersectionBInA == true)
	{
		/*
		case 1
		both points are in both OBBs
		*/

		result = pointOfIntersectB;
	}

	else if (pointOfIntersectionAInB == true || pointOfIntersectionBInA == true)
	{
		/*
		case 2/3 only one point of intersection exists in both OBBs
		*/
		int a = -1;
		fabs(a);
		DirectX::XMVECTOR pointOfInterest;
		PhysicsComponent* componentOfInterst = nullptr;
		PhysicsComponent* otherComponent = nullptr;
		if (pointOfIntersectionAInB == true)
		{
			pointOfInterest = pointOfIntersectA;
			componentOfInterst = obj2;
			otherComponent = obj1;
		}
		else if (pointOfIntersectionBInA == true)
		{
			pointOfInterest = pointOfIntersectB;
			componentOfInterst = obj1;
			otherComponent = obj2;
		}


		result = pointOfInterest;
	}
	else if (pointOfIntersectionAInB == false && pointOfIntersectionBInA == false)
	{
		/*
		case 4 when neither of the points can be found in both OBBs
		*/

		DirectX::XMVECTOR corner1 = DirectX::XMVectorSet(0, 0, 0, 0);
		DirectX::XMVECTOR corner2 = DirectX::XMVectorSet(0, 0, 0, 0);
		DirectX::XMVECTOR corner3 = DirectX::XMVectorSet(0, 0, 0, 0);
		DirectX::XMVECTOR corner4 = DirectX::XMVectorSet(0, 0, 0, 0);

		DirectX::XMVECTOR corners[2];
		corners[0] = DirectX::XMVectorSet(0, 0, 0, 0);
		corners[1] = DirectX::XMVectorSet(0, 0, 0, 0);

		corner1 = DirectX::XMVectorAdd(corner1, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[0], obj1->PC_OBB.ext[0]));
		corner1 = DirectX::XMVectorAdd(corner1, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[1], obj1->PC_OBB.ext[1]));
		corner1 = DirectX::XMVectorAdd(corner1, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[2], obj1->PC_OBB.ext[2]));

		corner2 = DirectX::XMVectorAdd(corner2, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[0], -obj1->PC_OBB.ext[0]));
		corner2 = DirectX::XMVectorAdd(corner2, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[1], obj1->PC_OBB.ext[1]));
		corner2 = DirectX::XMVectorAdd(corner2, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[2], obj1->PC_OBB.ext[2]));

		corner3 = DirectX::XMVectorAdd(corner3, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[0], obj1->PC_OBB.ext[0]));
		corner3 = DirectX::XMVectorAdd(corner3, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[1], obj1->PC_OBB.ext[1]));
		corner3 = DirectX::XMVectorAdd(corner3, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[2], -obj1->PC_OBB.ext[2]));

		corner4 = DirectX::XMVectorAdd(corner4, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[0], -obj1->PC_OBB.ext[0]));
		corner4 = DirectX::XMVectorAdd(corner4, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[1], obj1->PC_OBB.ext[1]));
		corner4 = DirectX::XMVectorAdd(corner4, DirectX::XMVectorScale(obj1->PC_OBB.ort.r[2], -obj1->PC_OBB.ext[2]));

		int nrOfCornersFound = 0;

		if (this->IsPointInBox(DirectX::XMVectorAdd(Apos, corner1), B, Bpos))
		{
			corners[nrOfCornersFound] = DirectX::XMVectorAdd(Apos, corner1);
			nrOfCornersFound++;
		}
		if (this->IsPointInBox(DirectX::XMVectorAdd(Apos, corner2), B, Bpos))
		{
			corners[nrOfCornersFound] = DirectX::XMVectorAdd(Apos, corner2);
			nrOfCornersFound++;
		}
		if (this->IsPointInBox(DirectX::XMVectorAdd(Apos, corner3), B, Bpos) && nrOfCornersFound < 2)
		{
			corners[nrOfCornersFound] = DirectX::XMVectorAdd(Apos, corner3);
			nrOfCornersFound++;
		}
		if (this->IsPointInBox(DirectX::XMVectorAdd(Apos, corner4), B, Bpos) && nrOfCornersFound < 2)
		{
			corners[nrOfCornersFound] = DirectX::XMVectorAdd(Apos, corner4);
			nrOfCornersFound++;
		}
		if (nrOfCornersFound < 2)
		{
			corner1 = DirectX::XMVectorScale(corner1, -1);
			corner2 = DirectX::XMVectorScale(corner2, -1);
			corner3 = DirectX::XMVectorScale(corner3, -1);
			corner4 = DirectX::XMVectorScale(corner4, -1);

			if (this->IsPointInBox(DirectX::XMVectorAdd(Apos, corner1), B, Bpos) && nrOfCornersFound < 2)
			{
				corners[nrOfCornersFound] = DirectX::XMVectorAdd(Apos, corner1);
				nrOfCornersFound++;
			}
			if (this->IsPointInBox(DirectX::XMVectorAdd(Apos, corner2), B, Bpos) && nrOfCornersFound < 2)
			{
				corners[nrOfCornersFound] = DirectX::XMVectorAdd(Apos, corner2);
				nrOfCornersFound++;
			}
			if (this->IsPointInBox(DirectX::XMVectorAdd(Apos, corner3), B, Bpos) && nrOfCornersFound < 2)
			{
				corners[nrOfCornersFound] = DirectX::XMVectorAdd(Apos, corner3);
				nrOfCornersFound++;
			}
			if (this->IsPointInBox(DirectX::XMVectorAdd(Apos, corner4), B, Bpos) && nrOfCornersFound < 2)
			{
				corners[nrOfCornersFound] = DirectX::XMVectorAdd(Apos, corner4);
				nrOfCornersFound++;
			}

		}
		else if (nrOfCornersFound == 0)
		{
			corner1 = DirectX::XMVectorSet(0, 0, 0, 0);
			corner2 = DirectX::XMVectorSet(0, 0, 0, 0);
			corner3 = DirectX::XMVectorSet(0, 0, 0, 0);
			corner4 = DirectX::XMVectorSet(0, 0, 0, 0);


			corner1 = DirectX::XMVectorAdd(corner1, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[0], obj2->PC_OBB.ext[0]));
			corner1 = DirectX::XMVectorAdd(corner1, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[1], obj2->PC_OBB.ext[1]));
			corner1 = DirectX::XMVectorAdd(corner1, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[2], obj2->PC_OBB.ext[2]));

			corner2 = DirectX::XMVectorAdd(corner2, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[0], -obj2->PC_OBB.ext[0]));
			corner2 = DirectX::XMVectorAdd(corner2, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[1], obj2->PC_OBB.ext[1]));
			corner2 = DirectX::XMVectorAdd(corner2, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[2], obj2->PC_OBB.ext[2]));

			corner3 = DirectX::XMVectorAdd(corner3, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[0], obj2->PC_OBB.ext[0]));
			corner3 = DirectX::XMVectorAdd(corner3, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[1], obj2->PC_OBB.ext[1]));
			corner3 = DirectX::XMVectorAdd(corner3, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[2], -obj2->PC_OBB.ext[2]));

			corner4 = DirectX::XMVectorAdd(corner4, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[0], -obj2->PC_OBB.ext[0]));
			corner4 = DirectX::XMVectorAdd(corner4, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[1], obj2->PC_OBB.ext[1]));
			corner4 = DirectX::XMVectorAdd(corner4, DirectX::XMVectorScale(obj2->PC_OBB.ort.r[2], -obj2->PC_OBB.ext[2]));

			if (this->IsPointInBox(DirectX::XMVectorAdd(Bpos, corner1), A, Apos))
			{
				corners[nrOfCornersFound] = DirectX::XMVectorAdd(Bpos, corner1);
				nrOfCornersFound++;
			}
			if (this->IsPointInBox(DirectX::XMVectorAdd(Bpos, corner2), A, Apos))
			{
				corners[nrOfCornersFound] = DirectX::XMVectorAdd(Bpos, corner2);
				nrOfCornersFound++;
			}
			if (this->IsPointInBox(DirectX::XMVectorAdd(Bpos, corner3), A, Apos) && nrOfCornersFound < 2)
			{
				corners[nrOfCornersFound] = DirectX::XMVectorAdd(Bpos, corner3);
				nrOfCornersFound++;
			}
			if (this->IsPointInBox(DirectX::XMVectorAdd(Bpos, corner4), A, Apos) && nrOfCornersFound < 2)
			{
				corners[nrOfCornersFound] = DirectX::XMVectorAdd(Bpos, corner4);
				nrOfCornersFound++;
			}
			if (nrOfCornersFound < 2)
			{
				corner1 = DirectX::XMVectorScale(corner1, -1);
				corner2 = DirectX::XMVectorScale(corner2, -1);
				corner3 = DirectX::XMVectorScale(corner3, -1);
				corner4 = DirectX::XMVectorScale(corner4, -1);

				if (this->IsPointInBox(DirectX::XMVectorAdd(Bpos, corner1), A, Apos) && nrOfCornersFound < 2)
				{
					corners[nrOfCornersFound] = DirectX::XMVectorAdd(Bpos, corner1);
					nrOfCornersFound++;
				}
				if (this->IsPointInBox(DirectX::XMVectorAdd(Bpos, corner2), A, Apos) && nrOfCornersFound < 2)
				{
					corners[nrOfCornersFound] = DirectX::XMVectorAdd(Bpos, corner2);
					nrOfCornersFound++;
				}
				if (this->IsPointInBox(DirectX::XMVectorAdd(Bpos, corner3), A, Apos) && nrOfCornersFound < 2)
				{
					corners[nrOfCornersFound] = DirectX::XMVectorAdd(Bpos, corner3);
					nrOfCornersFound++;
				}
				if (this->IsPointInBox(DirectX::XMVectorAdd(Bpos, corner4), A, Apos) && nrOfCornersFound < 2)
				{
					corners[nrOfCornersFound] = DirectX::XMVectorAdd(Bpos, corner4);
					nrOfCornersFound++;
				}

			}
		}
		DirectX::XMVECTOR FINALPOINT;
		DirectX::XMVECTOR diffVec;
		if (nrOfCornersFound == 1)
		{
			FINALPOINT = corners[0];
		}
		else if (nrOfCornersFound == 2)
		{
			diffVec = DirectX::XMVectorSubtract(corners[0], corners[1]);
			diffVec = DirectX::XMVectorDivide(diffVec, DirectX::XMVectorSet(2, 2, 2, 2));

			FINALPOINT = DirectX::XMVectorAdd(corners[1], diffVec);

		}
		result = FINALPOINT;

		int a = 0;
	}


	//the corner intersecting the static OBB
	DirectX::XMVECTOR pointOfIntersection;
	pointOfIntersection = result;

	PhysicsComponent* componentToMove = nullptr;
	PhysicsComponent* staticComponent = nullptr;
	if (!obj1->PC_is_Static)
	{
		componentToMove = obj1;
		staticComponent = obj2;
	}
	else
	{
		componentToMove = obj2;
		staticComponent = obj1;
	}


	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(pointOfIntersection, staticComponent->PC_pos);
	DirectX::XMVECTOR normal = DirectX::XMVectorSet(0, 0, 0, 0);


	float dot[3];

	dot[0] = DirectX::XMVectorGetX(DirectX::XMVector3Dot(diffVec, staticComponent->PC_OBB.ort.r[0]));
	dot[1] = DirectX::XMVectorGetX(DirectX::XMVector3Dot(diffVec, staticComponent->PC_OBB.ort.r[1]));
	dot[2] = DirectX::XMVectorGetX(DirectX::XMVector3Dot(diffVec, staticComponent->PC_OBB.ort.r[2]));

	if (dot[0] > staticComponent->PC_OBB.ext[0])
	{
		dot[0] = staticComponent->PC_OBB.ext[0];
	}
	else if (dot[0] < -staticComponent->PC_OBB.ext[0])
	{
		dot[0] = -staticComponent->PC_OBB.ext[0];
	}

	if (dot[1] > staticComponent->PC_OBB.ext[1])
	{
		dot[1] = staticComponent->PC_OBB.ext[1];
	}

	else if (dot[1] < -staticComponent->PC_OBB.ext[1])
	{
		dot[1] = -staticComponent->PC_OBB.ext[1];
	}

	if (dot[2] > staticComponent->PC_OBB.ext[2])
	{
		dot[2] = staticComponent->PC_OBB.ext[2];
	}
	else if (dot[2] < -staticComponent->PC_OBB.ext[2])
	{
		dot[2] = -staticComponent->PC_OBB.ext[2];
	}

	float test2[3];

	test2[0] = dot[0] / staticComponent->PC_OBB.ext[0];
	test2[1] = dot[1] / staticComponent->PC_OBB.ext[1];
	test2[2] = dot[2] / staticComponent->PC_OBB.ext[2];

	float axises[3];
	if (test2[0] >= 0.9)
	{
		axises[0] = 1;
	}
	else if (test2[0] <= -0.9)
	{
		axises[0] = -1;
	}
	else
	{
		axises[0] = 0;
	}

	if (test2[1] >= 0.9)
	{
		axises[1] = 1;
	}
	else if (test2[1] <= -0.9)
	{
		axises[1] = -1;
	}
	else
	{
		axises[1] = 0;
	}

	if (test2[2] >= 0.9)
	{
		axises[2] = 1;
	}
	else if (test2[2] <= -0.9)
	{
		axises[2] = -1;
	}
	else
	{
		axises[2] = 0;
	}

	//OBB* ptr = &staticComponent->PC_OBB;
	//this->FindNormalFromPointOfIntersection(ptr, diffVec, axises);

	for (int i = 0; i < 3; i++)
	{
		normal = DirectX::XMVectorAdd(normal, DirectX::XMVectorScale(staticComponent->PC_OBB.ort.r[i], axises[i]));
	}
	normal = DirectX::XMVector3Normalize(normal);
	//DirectX::XMVECTOR toCorrect = DirectX::XMVectorSubtract(normal, DirectX::XMVectorMultiply(normal, DirectX::XMVectorSet(test2[0], test2[1], test2[2], 0)));
	DirectX::XMVECTOR POItoA = DirectX::XMVectorSubtract(componentToMove->PC_pos, pointOfIntersection);
	

	//componentToMove->PC_pos = DirectX::XMVectorAdd(componentToMove->PC_pos, toCorrect);
	componentToMove->PC_normalForce = normal;
	DirectX::XMVECTOR originalPos = componentToMove->PC_pos;
	//if (DirectX::XMVector3NotEqual(normal, DirectX::XMVectorSet(0, 0, 0, 0)))
	//{
	//	DirectX::XMVECTOR oldpos = componentToMove->PC_pos;
	//	componentToMove->PC_pos = DirectX::XMVectorAdd(componentToMove->PC_pos, DirectX::XMVectorScale(normal, 0.25));
	//	bool test = this->ObbObbIntersectionTest(componentToMove, staticComponent, false, dt);
	//	DirectX::XMVECTOR diff;
	//	if (test == false)
	//	{
	//		for (int i = 0; i < 10 ; i++)
	//		{
	//			if (!test)
	//			{
	//				diff = DirectX::XMVectorScale(DirectX::XMVectorSubtract(oldpos, componentToMove->PC_pos), 0.5f);
	//				componentToMove->PC_pos = DirectX::XMVectorAdd(componentToMove->PC_pos, diff);

	//			}
	//			else if (test)
	//			{
	//				diff = DirectX::XMVectorScale(DirectX::XMVectorSubtract(componentToMove->PC_pos, oldpos), 0.5f);
	//				oldpos = componentToMove->PC_pos;
	//				componentToMove->PC_pos = DirectX::XMVectorAdd(componentToMove->PC_pos, diff);
	//			}
	//			test = this->ObbObbIntersectionTest(componentToMove, staticComponent, false, dt);
	//		}
	//	}

	//}
	this->CollitionDynamics(componentToMove, staticComponent, normal, dt);
	componentToMove->PC_pos = DirectX::XMVectorAdd(componentToMove->PC_pos, DirectX::XMVectorScale(componentToMove->PC_velocity, dt));

	return pointOfIntersection;

}

bool PhysicsHandler::IsPointInBox(DirectX::XMVECTOR point, OBB *& src, DirectX::XMVECTOR BoxPos)
{
	bool result = true;

	float acceptanceRange = 0.09f;
	DirectX::XMVECTOR BoxToPoint;
	BoxToPoint = DirectX::XMVectorSubtract(point, BoxPos);

	DirectX::XMFLOAT3 scalar;

	//does pointOfIntersectA exist in the OBB B
	DirectX::XMStoreFloat(&scalar.x, DirectX::XMVector3Dot(BoxToPoint, src->ort.r[0]));
	DirectX::XMStoreFloat(&scalar.y, DirectX::XMVector3Dot(BoxToPoint, src->ort.r[1]));
	DirectX::XMStoreFloat(&scalar.z, DirectX::XMVector3Dot(BoxToPoint, src->ort.r[2]));

	result = scalar.x <= src->ext[0] + acceptanceRange && scalar.x >= -src->ext[0] - acceptanceRange;

	if (result == true)
	{
		result = scalar.y <= src->ext[1] + acceptanceRange && scalar.y >= -src->ext[1] - acceptanceRange;

		if (result == true)
		{
			result = scalar.z <= src->ext[2] + acceptanceRange && scalar.z >= -src->ext[2] - acceptanceRange;
		}
	}
	return result;
}

void PhysicsHandler::FindNormalFromPointOfIntersection(OBB *& src, DirectX::XMVECTOR vecToPoint, float * arr)
{
	float dot[3];

	dot[0] = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToPoint, src->ort.r[0]));
	dot[1] = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToPoint, src->ort.r[1]));
	dot[2] = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToPoint, src->ort.r[2]));

	if (dot[0] > src->ext[0])
	{
		dot[0] = src->ext[0];
	}
	else if (dot[0] < -src->ext[0])
	{
		dot[0] = -src->ext[0];
	}

	if (dot[1] > src->ext[1])
	{
		dot[1] = src->ext[1];
	}

	else if (dot[1] < -src->ext[1])
	{
		dot[1] = -src->ext[1];
	}

	if (dot[2] > src->ext[2])
	{
		dot[2] = src->ext[2];
	}
	else if (dot[2] < -src->ext[2])
	{
		dot[2] = -src->ext[2];
	}

	float test2[3];

	test2[0] = dot[0] / src->ext[0];
	test2[1] = dot[1] / src->ext[1];
	test2[2] = dot[2] / src->ext[2];

	//float axises[3];
	if (test2[0] >= 0.9)
	{
		arr[0] = 1;
	}
	else if (test2[0] <= -0.9)
	{
		arr[0] = -1;
	}
	else
	{
		arr[0] = 0;
	}

	if (test2[1] >= 0.9)
	{
		arr[1] = 1;
	}
	else if (test2[1] <= -0.9)
	{
		arr[1] = -1;
	}
	else
	{
		arr[1] = 0;
	}

	if (test2[2] >= 0.9)
	{
		arr[2] = 1;
	}
	else if (test2[2] <= -0.9)
	{
		arr[2] = -1;
	}
	else
	{
		arr[2] = 0;
	}
}

bool PhysicsHandler::SphereAABBIntersectionTest(PhysicsComponent * objSphere, PhysicsComponent * objAABB, float dt)
{
	//this section of the code found on http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=4
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

	bool result = d <= (objSphere->PC_Sphere.radius * objSphere->PC_Sphere.radius);
	//-------------

	if (result)
	{
		float axises[3];
		axises[0] = 0.0f;
		axises[1] = 0.0f;
		axises[2] = 0.0f;

		int correction_axis_index = -1;
		int correction_axis_dir = 0;
		DirectX::XMVECTOR vec;
		for (int i = 0; i < 3; i++)
		{
			vec = DirectX::XMVectorSet(0, 0, 0, 0);
			vec = DirectX::XMVectorSetByIndex(vec, objAABB->PC_AABB.ext[i], i);
			float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(sphere_local, DirectX::XMVector3Normalize(vec)));
			axises[i] = dot;
			if (dot != 0.0f)
			{
				if (dot > objAABB->PC_AABB.ext[i])
				{
					axises[i] = objAABB->PC_AABB.ext[i];
				}
				if (dot < -objAABB->PC_AABB.ext[i])
				{
					axises[i] = -objAABB->PC_AABB.ext[i];
				}
			}
		}
		DirectX::XMVECTOR pointOfIntersection = DirectX::XMVectorSet(0, 0, 0, 0);
		DirectX::XMVECTOR toAdd = DirectX::XMVectorSet(axises[0], axises[1], axises[2], 0);
		pointOfIntersection = DirectX::XMVectorAdd(pointOfIntersection, toAdd);

		DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(sphere_local, pointOfIntersection);

		float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));

		diffVec = DirectX::XMVector3Normalize(diffVec);



		DirectX::XMVECTOR toMove = DirectX::XMVectorSubtract(DirectX::XMVectorScale(diffVec, objSphere->PC_Sphere.radius), DirectX::XMVectorScale(diffVec, lenght));

		objSphere->PC_pos = DirectX::XMVectorAdd(objSphere->PC_pos, toMove);
		this->CollitionDynamics(objSphere, objAABB, diffVec, dt);
		objSphere->PC_normalForce = diffVec;
		if (!objAABB->PC_is_Static)
		{
			objAABB->PC_normalForce = DirectX::XMVectorScale(diffVec, -1);
		}
	}

	return result;
}

bool PhysicsHandler::SphereOBBIntersectionTest(PhysicsComponent * objSphere, PhysicsComponent * objOBB, float dt)
{
	float radX = DirectX::XMVectorGetX(objOBB->PC_rotation);
	float radY = DirectX::XMVectorGetY(objOBB->PC_rotation);
	float radZ = DirectX::XMVectorGetZ(objOBB->PC_rotation);

	//create rotation matrix to translate the sphere into OBBs local space and do SphereVSAABB test
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationX(radX) * DirectX::XMMatrixRotationY(radY) * DirectX::XMMatrixRotationZ(radZ);
	DirectX::XMVECTOR t;
	DirectX::XMVECTOR toSphere;
	DirectX::XMVECTOR originalToSphere;

	DirectX::XMVECTOR originalVel;

	DirectX::XMMATRIX rotInv = DirectX::XMMatrixInverse(&t, rot);

	toSphere = DirectX::XMVectorSubtract(objSphere->PC_pos, objOBB->PC_pos);
	originalToSphere = toSphere;
	originalVel = objSphere->PC_velocity;
	float vel = DirectX::XMVectorGetX(DirectX::XMVector3Length(objSphere->PC_velocity));

	//rotate the toSphere vector to get sphere in OBBs locan space
	toSphere = DirectX::XMVector3Transform(toSphere, rotInv);
	objSphere->PC_pos = DirectX::XMVectorAdd(objOBB->PC_pos, toSphere);
	//rotate sphere velocity to be in OBBs local space
	objSphere->PC_velocity = DirectX::XMVector3Transform(objSphere->PC_velocity, rotInv);
	//convert the OBB to AABB for sphereVSAABB test
	PhysicsComponent testAABB;
	testAABB.PC_BVtype = BV_AABB;
	testAABB.PC_elasticity = objOBB->PC_elasticity;
	testAABB.PC_friction = objOBB->PC_friction;
	testAABB.PC_is_Static = objOBB->PC_is_Static;
	testAABB.PC_pos = objOBB->PC_pos;
	testAABB.PC_AABB.ext[0] = objOBB->PC_OBB.ext[0];
	testAABB.PC_AABB.ext[1] = objOBB->PC_OBB.ext[1];
	testAABB.PC_AABB.ext[2] = objOBB->PC_OBB.ext[2];
	
	//do sphereVSAABB test
	bool result = this->SphereAABBIntersectionTest(objSphere, &testAABB, dt);
	if (result)
	{
		//if result is true get the new toSphere vector
		toSphere = DirectX::XMVectorSubtract(objSphere->PC_pos, objOBB->PC_pos);
		objSphere->PC_pos = DirectX::XMVectorSetByIndex(objSphere->PC_pos, 0, 3);
		//rotate sphere pos/velocity out of OBBs local space
		toSphere = DirectX::XMVector3Transform(toSphere, rot);
		toSphere = DirectX::XMVectorSetByIndex(toSphere, 0, 3);
		//objSphere->PC_velocity = DirectX::XMVector3Transform(objSphere->PC_velocity, rot);
		objSphere->PC_velocity = originalVel;
		this->CollitionDynamics(objSphere, objOBB, toSphere, dt);

		objSphere->PC_pos = DirectX::XMVectorAdd(objOBB->PC_pos, toSphere);
	}
	else
	{
		//if no intersection was found reset sphere pos/velocity to original values
		objSphere->PC_pos = DirectX::XMVectorAdd(objOBB->PC_pos, originalToSphere);
		objSphere->PC_velocity = originalVel;
	}

	return result;

}

bool PhysicsHandler::SphereSphereIntersectionTest(PhysicsComponent * objSphere1, PhysicsComponent * objSphere2, float dt)
{
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(objSphere1->PC_pos, objSphere2->PC_pos);
	float d = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));

	float totalLenght = (objSphere1->PC_Sphere.radius + objSphere2->PC_Sphere.radius);
	bool result = d <= totalLenght;

	if (result)
	{
		if (d < totalLenght)
		{
			diffVec = DirectX::XMVector3Normalize(diffVec);
			DirectX::XMVECTOR toMove = DirectX::XMVectorScale(diffVec, totalLenght - d);
			objSphere1->PC_pos = DirectX::XMVectorAdd(objSphere1->PC_pos, toMove);
			this->CollitionDynamics(objSphere1, objSphere2, diffVec, dt);
			objSphere1->PC_normalForce = diffVec;
			if (!objSphere2->PC_is_Static)
			{
				objSphere2->PC_normalForce = DirectX::XMVectorScale(diffVec, -1);
			}
		}
	}

	return result;
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

bool PhysicsHandler::AABBPlaneIntersectionTest(PhysicsComponent * objAABB, PhysicsComponent * objPlane, float dt)
{
	//this section of the code taken from http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=7
	float d = 0;
	float a1 = objAABB->PC_AABB.ext[0];
	float a2 = objAABB->PC_AABB.ext[1];
	float a3 = objAABB->PC_AABB.ext[2];

	float sum = 0;
	float dot1 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, DirectX::XMVectorSet(a1 / a1, 0, 0, 0)));
	float dot2 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, DirectX::XMVectorSet(0, a2 / a2, 0, 0)));
	float dot3 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, DirectX::XMVectorSet(0, 0, a3 / a3, 0)));

	sum += a1 * fabs(dot1);
	sum += a2 * fabs(dot2);
	sum += a3 * fabs(dot3);

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(objAABB->PC_pos, objPlane->PC_pos);
	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR dump;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &dump, diffVec, objPlane->PC_Plane.PC_normal);

	DirectX::XMVECTOR temp_pos = DirectX::XMVectorSubtract(objAABB->PC_pos, pParallel);

	diffVec = DirectX::XMVectorSubtract(objAABB->PC_pos, temp_pos);
	pParallel = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVector3ComponentsFromNormal(&pParallel, &dump, diffVec, objPlane->PC_Plane.PC_normal);

	d = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));

	bool result = d < sum;
	//---------------------------
	if (result)
	{
		float t = sum - d;

		DirectX::XMVECTOR toMove = DirectX::XMVectorScale(objPlane->PC_Plane.PC_normal, t);

		objAABB->PC_pos = DirectX::XMVectorAdd(objAABB->PC_pos, toMove);
		this->CollitionDynamics(objAABB, objPlane, objPlane->PC_Plane.PC_normal, dt);

		objAABB->PC_normalForce = objPlane->PC_normalForce;

	}

	return result;
}

bool PhysicsHandler::OBBPlaneIntersectionTest(PhysicsComponent * objOBB, PhysicsComponent * objPlane, float dt)
{
	//this section of the code taken from http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=7
	float a1 = objOBB->PC_OBB.ext[0];
	float a2 = objOBB->PC_OBB.ext[1];
	float a3 = objOBB->PC_OBB.ext[2];

	float sum = 0;
	float dot1 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, objOBB->PC_OBB.ort.r[0]));
	float dot2 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, objOBB->PC_OBB.ort.r[1]));
	float dot3 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(objPlane->PC_Plane.PC_normal, objOBB->PC_OBB.ort.r[2]));

	sum += a1 * abs(dot1);
	sum += a2 * abs(dot2);
	sum += a3 * abs(dot3);

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(objOBB->PC_pos, objPlane->PC_pos);
	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR pPerpendicular;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, diffVec, objPlane->PC_Plane.PC_normal);

	float d = 0;

	d = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));

	bool result = d < sum;
	//-----------------------------------
	if (result)
	{
		float t = sum - d;
		DirectX::XMVECTOR toMove = DirectX::XMVectorScale(objPlane->PC_Plane.PC_normal, t);

		objOBB->PC_pos = DirectX::XMVectorAdd(objOBB->PC_pos, toMove);
		this->CollitionDynamics(objOBB, objPlane, objPlane->PC_Plane.PC_normal, dt);

		objOBB->PC_normalForce = objPlane->PC_normalForce;

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

		float e = (obj1->PC_elasticity + obj2->PC_elasticity) / 2;
		float frictionConstant = (obj1->PC_friction + obj2->PC_friction);


		float newParallelVel = (old_parallelVel * e);

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
		float parallelForce = parallelImpuls / dt;

		//float newPerpendicularVel = old_perpendicularVel * obj2->PC_friction;
		//float perpendicularImpuls = obj1->PC_mass * newPerpendicularVel - obj1->PC_mass * old_perpendicularVel;
		//float perpendicularForce = perpendicularImpuls / dt;
		//this->ApplyForceToComponent(obj1, DirectX::XMVectorScale(pPerpendicular, perpendicularForce), dt);


		float frictionForce = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorScale(DirectX::XMVectorScale(pParallel, parallelForce), frictionConstant)));
		float maxFriction = fabs(DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorScale(this->m_gravity, frictionConstant))));
		if (frictionForce > maxFriction)
		{
			frictionForce = maxFriction;
		}
		DirectX::XMVECTOR frictionFORCE = DirectX::XMVectorScale(DirectX::XMVectorScale(pPerpendicular, -1), frictionForce);
		//frictionFORCE = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMVectorScale(pPerpendicular, -1)), frictionForce);

		this->ApplyForceToComponent(obj1, DirectX::XMVectorScale(pParallel, parallelForce), dt);
		this->ApplyForceToComponent(obj1, frictionFORCE, dt);

	}
	else
	{
		DirectX::XMVECTOR pParallel;
		DirectX::XMVECTOR pPerpendicular1;
		DirectX::XMVECTOR pPerpendicular2;

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

		float e = obj1->PC_elasticity + obj2->PC_elasticity;

		for (int i = 0; i < 3; i++)
		{
			//not sure what value e should be

			v1_new[i] = (v1_old[i] * (m1 - m2*e) + m2*v2_old[i] * (1.0f + e)) / (m1 + m2);

			v2_new[i] = (v1_old[i] * m1*(1.0f + e) + (m2 - e*m1)*v2_old[i]) / (m1 + m2);

		}

		DirectX::XMVECTOR pParallelForce1;
		pParallelForce1 = DirectX::XMVectorSubtract(DirectX::XMVectorScale(DirectX::XMVectorSet(v1_new[0], v1_new[1], v1_new[2], 0), obj1->PC_mass), DirectX::XMVectorScale(DirectX::XMVectorSet(v1_old[0], v1_old[1], v1_old[2], 0), obj1->PC_mass));
		pParallelForce1 = DirectX::XMVectorScale(pParallelForce1, 1 / dt);

		DirectX::XMVECTOR pParallelForce2;
		pParallelForce2 = DirectX::XMVectorSubtract(DirectX::XMVectorScale(DirectX::XMVectorSet(v2_new[0], v2_new[1], v2_new[2], 0), obj2->PC_mass), DirectX::XMVectorScale(DirectX::XMVectorSet(v2_old[0], v2_old[1], v2_old[2], 0), obj2->PC_mass));
		pParallelForce2 = DirectX::XMVectorScale(pParallelForce2, 1 / dt);

		float frictionConstant = (obj1->PC_friction + obj2->PC_friction);


		DirectX::XMVECTOR frictionForce1 = DirectX::XMVectorScale(DirectX::XMVectorScale(pPerpendicular1, -1), frictionConstant);
		frictionForce1 = DirectX::XMVectorAdd(frictionForce1, DirectX::XMVectorScale(pPerpendicular2, frictionConstant));

		DirectX::XMVECTOR frictionForce2 = DirectX::XMVectorScale(DirectX::XMVectorScale(pPerpendicular2, -1), frictionConstant);
		frictionForce2 = DirectX::XMVectorAdd(frictionForce2, DirectX::XMVectorScale(pPerpendicular1, frictionConstant));

		this->ApplyForceToComponent(obj1, pParallelForce1, dt);
		this->ApplyForceToComponent(obj1, frictionForce1, dt);

		this->ApplyForceToComponent(obj2, pParallelForce2, dt);
		this->ApplyForceToComponent(obj2, frictionForce2, dt);

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
	src->PC_rotation = DirectX::XMVectorSetByIndex(src->PC_rotation, radian, 0);
	//read the value of PC_rotation
	//DirectX::XMStoreFloat3(&rot,src->PC_rotation);
	float rotAngle = (radian);

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
	src->PC_rotation = DirectX::XMVectorSetByIndex(src->PC_rotation, radian, 1);

	xMatrix = DirectX::XMMatrixRotationY(radian);
	test = DirectX::XMMatrixMultiply(test, xMatrix);

	src->PC_OBB.ort = test;

	return xMatrix;
}

DirectX::XMMATRIX PhysicsHandler::RotateBB_Z(PhysicsComponent* src, const float &radian)
{
	DirectX::XMMATRIX xMatrix;
	DirectX::XMMATRIX test = src->PC_OBB.ort;
	src->PC_rotation = DirectX::XMVectorSetByIndex(src->PC_rotation, radian, 2);

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

void PhysicsHandler::SetStartIndex(unsigned int newStartIndex)
{
	this->m_startIndex = newStartIndex;
}

void PhysicsHandler::SetNumberOfDynamics(unsigned int newNumberOfDynamics)
{
	this->m_numberOfDynamics = newNumberOfDynamics;
}

void PhysicsHandler::SetIsHost(bool newIsHost)
{
	this->m_isHost = newIsHost;
}

PhysicsHandler::PhysicsHandler()
{
}

PhysicsHandler::~PhysicsHandler()
{
}

bool PhysicsHandler::Initialize()
{
	this->m_gravity = DirectX::XMVectorSet(0.0f, -0.009f, 0.0f, 0.0f);

	this->m_startIndex = 0;
	this->m_nrOfStaticObjects = this->m_physicsComponents.size();
	this->m_isHost = true;


	return true;
}

void PhysicsHandler::ShutDown()
{
	int size = this->m_physicsComponents.size();
	for (int i = 0; i < size; i++)
	{
		delete this->m_physicsComponents.at(i);
	}
}

void PhysicsHandler::Update(float deltaTime)
{
	float dt = (deltaTime / 50000);

	int nrOfChainLinks = this->m_links.size();
	for (int i = 0; i < nrOfChainLinks; i++)
	{
		this->DoChainPhysics(&this->m_links.at(i), dt);
	}
	for (int i = 0; i < nrOfChainLinks; i++)
	{
		this->AdjustChainLinkPosition(&this->m_links.at(i));
	}
	//this->m_numberOfDynamics = this->m_physicsComponents.size() - this->m_nrOfStaticObjects;	// SHOULD BE REMOVED SINCE WE GET THE NUMBER FROM THE NETWORK MODULE (NOT IMPLETED YET) //
	
	// DYNAMIC VS DYNAMIC
	if (this->m_isHost)
	{
		this->m_numberOfDynamics = this->m_dynamicComponents.size();
		// Do dynamic vs dynamic checks
		for (int i = 0; i < this->m_numberOfDynamics; i++)	// 
		{
			PhysicsComponent* current = this->m_dynamicComponents.at(i);
			if (current->PC_collides)
			{
				for (int j = i + 1; j < this->m_numberOfDynamics; j++)
				{
					PhysicsComponent* toCompare = this->m_dynamicComponents.at(j);
					if (toCompare->PC_collides)
					{
						if (current->PC_BVtype == BV_AABB)
						{
							if (toCompare->PC_BVtype == BV_AABB)
							{
								this->AABBAABBIntersectionTest(current, toCompare, dt);
							}
						}
						if (current->PC_BVtype == BV_Sphere)
						{
							if (toCompare->PC_BVtype == BV_AABB)
							{
								this->SphereAABBIntersectionTest(current, toCompare, dt);
							}
							if (toCompare->PC_BVtype == BV_OBB)
							{
								this->SphereOBBIntersectionTest(current, toCompare, dt);
							}
							if (toCompare->PC_BVtype == BV_Sphere)
							{
								this->SphereSphereIntersectionTest(current, toCompare, dt);
							}
						}
						if (current->PC_BVtype == BV_OBB)
						{
							if (toCompare->PC_BVtype == BV_OBB)
							{
								this->ObbObbIntersectionTest(current, toCompare, true, dt);
							}
						}
					}

				}
			}
		}
	}

	// DYNAMIC VS STATIC
	this->m_numberOfDynamics = this->m_dynamicComponents.size();
	for (int i = this->m_startIndex; i < this->m_numberOfDynamics; i++)	// 
	{
		PhysicsComponent* current = this->m_dynamicComponents.at(i);
		current->PC_normalForce = DirectX::XMVectorSet(0, 0, 0, 0);


		int loopsNeeded = 1;
		float speed = DirectX::XMVectorGetX(DirectX::XMVector3Length(current->PC_velocity));
		float speedLimit = 0.3f; // speed limit that detemines when a component needs more checks to ensure it doesn't go through walls
		if (speed > speedLimit)
		{
			//if speed is to great increase number of loops to insure 
			//hit detection/correction is done correctly
			loopsNeeded = 3; // 3 or 4 seems to work fine when speed is above 0.3f
		}
		this->m_nrOfStaticObjects = this->m_staticComponents.size();
		for (int i = 0; i < loopsNeeded; i++)
		{
			float newDT = dt / loopsNeeded;
			if (!current->PC_steadfast)
			{
				if (current->PC_BVtype == BoundingVolumeType::BV_AABB)
					{
						//only collide with static environment for starters
						for (int j = 0; j < this->m_nrOfStaticObjects; j++)
						{
							PhysicsComponent* toCompare = nullptr;
							toCompare = this->m_staticComponents.at(j);
							if (toCompare->PC_BVtype == BoundingVolumeType::BV_AABB)
							{
								this->AABBAABBIntersectionTest(current, toCompare, newDT);
							}

							if (toCompare->PC_BVtype == BoundingVolumeType::BV_Plane)
							{
								this->AABBPlaneIntersectionTest(current, toCompare, newDT);
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
				if (current->PC_BVtype == BoundingVolumeType::BV_Sphere)
					{
						//only collide with static environment for starters
						for (int j = 0; j < this->m_nrOfStaticObjects; j++)
						{
							PhysicsComponent* toCompare = nullptr;
							toCompare = this->m_staticComponents.at(j);
							if (toCompare->PC_BVtype == BoundingVolumeType::BV_AABB)
							{
								this->SphereAABBIntersectionTest(current, toCompare, newDT);
							}

							if (toCompare->PC_BVtype == BoundingVolumeType::BV_Plane)
							{
								this->SpherePlaneIntersectionTest(current, toCompare, newDT);
							}

							if (toCompare->PC_BVtype == BoundingVolumeType::BV_OBB)
							{
								this->SphereOBBIntersectionTest(current, toCompare, newDT);
							}

							if (toCompare->PC_BVtype == BoundingVolumeType::BV_Sphere)
							{
								this->SphereSphereIntersectionTest(current, toCompare, newDT);
							}
						}
					}
				if (current->PC_BVtype == BoundingVolumeType::BV_OBB)
					{
						//only collide with static environment for starters
						for (int j = 0; j < this->m_nrOfStaticObjects; j++)
						{
							PhysicsComponent* toCompare = nullptr;
							toCompare = this->m_staticComponents.at(j);
							if (toCompare->PC_BVtype == BoundingVolumeType::BV_AABB)
							{
								//toCompare has to be AABB or bad peaople will take you in the night
								this->OBBAABBIntersectionTest(current, toCompare, newDT);
							}

							if (toCompare->PC_BVtype == BoundingVolumeType::BV_Plane)
							{
								this->OBBPlaneIntersectionTest(current, toCompare, newDT);
							}

							if (toCompare->PC_BVtype == BoundingVolumeType::BV_OBB)
							{
								//to be continued
								this->ObbObbIntersectionTest(current, toCompare, true, newDT);
							}

							if (toCompare->PC_BVtype == BoundingVolumeType::BV_Sphere)
							{
								this->SphereSphereIntersectionTest(current, toCompare, newDT);
							}
						}
					}

			}
			DirectX::XMVECTOR pParallel;
			DirectX::XMVECTOR pPerpendicular;

			DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, this->m_gravity, current->PC_normalForce);

			DirectX::XMVECTOR gravityForce = DirectX::XMVectorScale(DirectX::XMVectorScale(DirectX::XMVectorScale(pPerpendicular, (float)current->PC_gravityInfluence), current->PC_mass), 1.0f);

			this->ApplyForceToComponent(current, gravityForce, newDT);

			if (!current->PC_is_Static)
			{
				current->PC_pos = DirectX::XMVectorAdd(current->PC_pos, DirectX::XMVectorScale(current->PC_velocity, newDT));
				DirectX::XMFLOAT3 temp;
				DirectX::XMStoreFloat3(&temp, current->PC_pos);
				if (temp.y < -20)
				{
					current->PC_pos = DirectX::XMVectorSet(temp.x, -5, temp.z, 0);
					current->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
				}
			}
			else
			{
				current->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
			}
		}

	}


}

void PhysicsHandler::CheckFieldIntersection()
{
	PhysicsComponent* current = nullptr;
	int size = this->m_dynamicComponents.size();
	for (int i = 0; i < size; i++)
	{
		current = this->m_dynamicComponents.at(i);
		int nrOfFields = this->m_fields.size();
		for (int y = 0; y < nrOfFields; y++)
		{
			if (current->PC_BVtype == BV_OBB)
			{

			}
			if (current->PC_BVtype == BV_AABB)
			{

			}
			if(current->PC_BVtype == BV_Sphere)
		}
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

	if (distance > link->CL_lenght)
	{
		DirectX::XMVECTOR toMove = DirectX::XMVectorSubtract(DirectX::XMVectorScale(DirectX::XMVector3Normalize(diffVec), link->CL_lenght), diffVec);
		toMove = DirectX::XMVectorScale(toMove, 0.5f);

		link->CL_previous->PC_pos = DirectX::XMVectorAdd(link->CL_previous->PC_pos, DirectX::XMVectorScale(toMove, -1));
		link->CL_next->PC_pos = DirectX::XMVectorAdd(link->CL_next->PC_pos, toMove);


		//diffVec = DirectX::XMVector3Normalize(diffVec);
		//link->CL_next->PC_pos = DirectX::XMVectorAdd(link->CL_previous->PC_pos, DirectX::XMVectorScale(diffVec, (link->CL_lenght)));
		//link->CL_next->PC_pos = link->CL_next->PC_pos;

		
	}

}

void PhysicsHandler::ApplyForceToComponent(PhysicsComponent * componentPtr, DirectX::XMVECTOR force, float dt)
{
	if (!componentPtr->PC_is_Static && !componentPtr->PC_steadfast)
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
	newObject->PC_collides = true;
	newObject->PC_entityID = 0;
	newObject->PC_is_Static = isStatic;
	newObject->PC_steadfast = false;
	newObject->PC_mass = 1.0f;
	newObject->PC_gravityInfluence = 1.0f;
	newObject->PC_Sphere.radius = 1.0f;
	newObject->PC_friction = 0.9f;
	newObject->PC_elasticity = 0.5f;
	newObject->PC_BVtype = BV_AABB;

	this->CreateDefaultBB(pos, newObject);
	this->m_physicsComponents.push_back(newObject);

	PhysicsComponent* toSwap;
	if (isStatic == false)
	{
		toSwap = this->m_physicsComponents.at((this->m_physicsComponents.size() - this->m_nrOfStaticObjects) - 1);
		this->m_physicsComponents.at((this->m_physicsComponents.size() - this->m_nrOfStaticObjects) - 1) = newObject;
		this->m_physicsComponents.at(this->m_physicsComponents.size() - 1) = toSwap;
	}
	else
	{
		this->m_nrOfStaticObjects++;
	}

	return newObject;
}


void PhysicsHandler::CreateChainLink(PhysicsComponent* playerComponent, PhysicsComponent* ballComponent, int nrOfLinks, float linkLenght)
{
	//important function will link from index1 to index2 and change the position of the PhysicsComponent at index2

	PhysicsComponent* ptr = playerComponent;
	PhysicsComponent* previous = playerComponent;
	PhysicsComponent* next = nullptr;

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(ballComponent->PC_pos, playerComponent->PC_pos);
	
	if (!DirectX::XMVector3NotEqual(diffVec, DirectX::XMVectorSet(0, 0, 0, 0)))
	{
		diffVec = DirectX::XMVectorSet((float)nrOfLinks, 0, 0, 0);
	}
	diffVec = DirectX::XMVectorDivide(diffVec, DirectX::XMVectorSet((float)nrOfLinks, (float)nrOfLinks, (float)nrOfLinks, (float)nrOfLinks));

	DirectX::XMVECTOR nextPos = DirectX::XMVectorAdd(previous->PC_pos, diffVec);

	for (int i = 1; i <= nrOfLinks; i++)
	{
		ChainLink link;
		link.CL_lenght = linkLenght;

		//next = this->CreatePhysicsComponent(DirectX::XMVectorAdd(ptr->PC_pos, DirectX::XMVectorScale(diffVec, i)));
		next = this->CreatePhysicsComponent(nextPos, false);
		
		next->PC_BVtype = BV_AABB;
		next->PC_collides = false;
		next->PC_Sphere.radius = 0.5;
		//next->PC_friction = 0;

		next->PC_AABB.ext[0] = 0.25f;
		next->PC_AABB.ext[1] = 0.25f;
		next->PC_AABB.ext[2] = 0.25f;
		next->PC_gravityInfluence = 1.0f;

		link.CL_previous = previous;
		link.CL_next = next;
		this->m_links.push_back(link);
		previous = next;
		nextPos = DirectX::XMVectorAdd(previous->PC_pos, diffVec);
	}
	ChainLink link;
	link.CL_lenght = linkLenght;

	next = ballComponent;
	next->PC_pos = nextPos;
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

Field * PhysicsHandler::CreateField(DirectX::XMVECTOR & pos, unsigned int entityID1, unsigned int entityID2, OBB & obb)
{
	this->m_fields.push_back(Field());
	Field* field = &this->m_fields.at(this->m_fields.size());
	field->F_BV.ext[0]	= obb.ext[0];
	field->F_BV.ext[1]	= obb.ext[1];
	field->F_BV.ext[2]	= obb.ext[2];
	field->F_BV.ort		= obb.ort;
	field->F_entitityID1 = entityID1;
	field->F_entitityID2 = entityID2;
	field->F_first_inide = false;
	field->F_second_inside = false;

	return nullptr;
}

void PhysicsHandler::SimpleCollition(float dt)
{
	float m_frictionConstant = 0.999f;
	PhysicsComponent* ptr;
	int size = this->m_physicsComponents.size();
	for (int i = 0; i < size; i++)
	{
		ptr = this->m_physicsComponents.at(i);


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

int PhysicsHandler::GetNrOfComponents()const
{
	return this->m_physicsComponents.size();
}

PhysicsComponent* PhysicsHandler::GetDynamicComponentAt(int index)const
{
	if (index >= 0 && index < (signed int)this->m_physicsComponents.size())
	{
		return this->m_physicsComponents.at(index);
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

void PhysicsHandler::SortComponents()
{
	//this->m_nrOfStaticObjects = 20;
	//this->m_nrOfStaticObjects = 0;
	//int nrOfComponents = this->m_physicsComponents.size();
	//PhysicsComponent* current;
	//for (int i = 0; i < nrOfComponents; i++)
	//{
	//	current = this->m_physicsComponents.at(i);
	//	if (current->PC_is_Static)
	//	{
	//		this->m_nrOfStaticObjects++;
	//	}
	//}
	//int pivot = nrOfComponents - this->m_nrOfStaticObjects;
	//int lastKnownStatic = pivot;
	//PhysicsComponent* dynamicToSwap = nullptr;
	//PhysicsComponent* staticToSwap = nullptr;
	//for (int i = pivot; i < nrOfComponents; i++)
	//{
	//	current = this->m_physicsComponents.at(i);
	//	if(!current->PC_is_Static)
	//	{ 
	//		dynamicToSwap = this->m_physicsComponents.at(i);
	//		for (int x = lastKnownStatic; x >= 0 && dynamicToSwap != nullptr; x--)
	//		{
	//			staticToSwap = this->m_physicsComponents.at(x);
	//			if (staticToSwap->PC_is_Static)
	//			{
	//				lastKnownStatic = x;
	//				this->m_physicsComponents.at(i) = staticToSwap;
	//				this->m_physicsComponents.at(x) = dynamicToSwap;
	//				dynamicToSwap = nullptr;
	//			}

	//		}
	//	}
	//}
	//int a = this->m_physicsComponents.size();

	this->m_dynamicComponents.clear();
	this->m_staticComponents.clear();

	int size = this->m_physicsComponents.size();

	PhysicsComponent* ptr = nullptr;
	for (int i = 0; i < size; i++)
	{
		ptr = this->m_physicsComponents.at(i);
		if (ptr->PC_is_Static)
		{
			this->m_staticComponents.push_back(ptr);
		}
		else
		{
			this->m_dynamicComponents.push_back(ptr);
		}
	}

	int nrOfDynamic = this->m_dynamicComponents.size();
	int nrOfStatic = this->m_staticComponents.size();
	ptr = nullptr;
	for (int i = 0; i < nrOfDynamic; i++)
	{
		ptr = this->m_dynamicComponents.at(i);
		if (ptr->PC_is_Static)
		{
			int wrong = 1;
		}
	}
	for (int i = 0; i < nrOfStatic; i++)
	{
		ptr = this->m_staticComponents.at(i);
		if (!ptr->PC_is_Static)
		{
			int wrong = 1;
		}
	}
}

PhysicsComponent * PhysicsHandler::GetClosestComponent(PhysicsComponent * component, int minDistance)
{
	PhysicsComponent* pp = nullptr;
	PhysicsComponent* closest = nullptr;
	float distance = 0;
	float closestDistance = 999999999;	//Gotta be big
	DirectX::XMVECTOR vec;

	for(int i = 0; i < this->m_dynamicComponents.size(); i++)	//We know the dynamics are in the front of the array
	{
		pp = this->m_dynamicComponents.at(i);

		if (pp->PC_entityID != 0 && pp->PC_entityID != 1 && pp->PC_entityID != 2)	//Check so we sont find our own component we compare to
		{
			
			//Calc the distance
			vec = DirectX::XMVectorSubtract(pp->PC_pos, component->PC_pos);
			distance = DirectX::XMVectorGetX( DirectX::XMVector3Length(vec) );

			if (distance <= minDistance)	// Check its close enoughe 
			{

				if (closest != nullptr)		// If we already found a component
				{

					if (distance < closestDistance)	//The new one is closer
					{
						closest = pp;
						closestDistance = distance;
					}

				}
				else
				{
					closest = pp;
					closestDistance = distance;
				}
				
			}

		}

	}

	return closest;
}

#ifdef _DEBUG
void PhysicsHandler::GetPhysicsComponentOBB(OBB*& src, int index)
{
	src = &(this->m_physicsComponents.at(index)->PC_OBB);
}

void PhysicsHandler::GetPhysicsComponentAABB(AABB*& src, int index)
{
	src = &(this->m_physicsComponents.at(index)->PC_AABB);
}

void PhysicsHandler::GetPhysicsComponentPlane(Plane*& src, int index)
{
	src = &(this->m_physicsComponents.at(index)->PC_Plane);
}

void PhysicsHandler::GetPhysicsComponentSphere(Sphere *& src, int index)
{
	src = &(this->m_physicsComponents.at(index)->PC_Sphere);
}

#endif 

