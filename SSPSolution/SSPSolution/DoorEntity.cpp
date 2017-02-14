#include "DoorEntity.h"

DoorEntity::DoorEntity()
{
}


DoorEntity::~DoorEntity()
{
}

int DoorEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, std::vector<ElementState> subjectStates, float rotateTime, float minRotation, float maxRotation)
{
	this->InitializeBase(entityID, pComp, gComp, nullptr);

	this->m_closed_pos = pComp->PC_pos;
	this->m_originalOrto = pComp->PC_OBB.ort;
	this->m_isOpened = false;
	this->m_rotateTime = rotateTime;
	this->m_minRotation = minRotation;
	this->m_maxRotation = maxRotation;
	float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);
	this->m_minRotation += currentYRotation;
	this->m_maxRotation += currentYRotation;
	this->m_rotatePerSec = (this->m_maxRotation - this->m_minRotation) / this->m_rotateTime;
	this->SyncComponents();
	//DirectX::XMVECTOR offSet = DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[1], -1.5f);
	//offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[2], -1.2f));
	//DirectX::XMMATRIX offSet_matrix = DirectX::XMMatrixTranslationFromVector(offSet);
	//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, offSet_matrix);
	this->m_subjectStates = subjectStates;

	return 0;
}

int DoorEntity::Update(float dT, InputHandler * inputHandler)
{


	if (m_animationActive)
	{
		DirectX::XMVECTOR offSet = DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[1], -1.5f);
		offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[2], -1.2f));
		PhysicsComponent* ptr = this->GetPhysicsComponent();
		DirectX::XMMATRIX rot;
		DirectX::XMMATRIX offSet_matrix = DirectX::XMMatrixTranslationFromVector(offSet);
		DirectX::XMVECTOR pivotPoint = offSet;// DirectX::XMVector3TransformCoord(this->m_pComp->PC_pos, offSet_matrix);
		//ptr->PC_pos = DirectX::XMVectorAdd(ptr->PC_pos, offSet);
		float sinRot = (pow(sin(m_animSpeed * dT) + 1, 4) / 4);


		DirectX::XMVECTOR vectorToTranslate = DirectX::XMVectorSubtract(pivotPoint, this->m_pComp->PC_pos);
		DirectX::XMMATRIX pivotTranslation = DirectX::XMMatrixTranslation(vectorToTranslate.m128_f32[0]*-1, vectorToTranslate.m128_f32[1] *-1, vectorToTranslate.m128_f32[2] * -1);

	
		

		if (m_targetRot == 0)
		{
			if (m_currRot < m_targetRot)
			{
				//this->m_pComp->PC_pos = DirectX::XMVectorAdd(offSet, this->m_pComp->PC_pos);
				m_currRot += sinRot;
				rot = DirectX::XMMatrixRotationAxis(this->m_pComp->PC_OBB.ort.r[1], DirectX::XMConvertToRadians(sinRot));
				
				//ptr->PC_pos = DirectX::XMVector3TransformCoord(ptr->PC_pos, offSet_matrix);

				//Translate back to original position (origin)
				rot = DirectX::XMMatrixMultiply(rot, pivotTranslation);
				//ptr->PC_pos = DirectX::XMVector3TransformCoord(ptr->PC_pos, rot);
				//Turn back the inverted vector and create a matrix that will translate back to origin
				pivotTranslation = DirectX::XMMatrixTranslation(vectorToTranslate.m128_f32[0], vectorToTranslate.m128_f32[1], vectorToTranslate.m128_f32[2]);
				rot = DirectX::XMMatrixMultiply(rot, pivotTranslation);
				ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, rot);
				//ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, pivotTranslation);
				//ptr->PC_pos = DirectX::XMVector3TransformCoord(ptr->PC_pos, pivotTranslation);

				//DirectX::XMVECTOR deter = DirectX::XMMatrixDeterminant(offSet_matrix);
				////ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, DirectX::XMMatrixInverse(&deter, offSet_matrix));
				//ptr->PC_pos = DirectX::XMVector3TransformCoord(ptr->PC_pos, DirectX::XMMatrixInverse(&deter, offSet_matrix));
			}
			else {
				m_animationActive = false;
				m_currRot = m_targetRot;

				DirectX::XMFLOAT3 pos;
				DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
				//SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_LEVER, pos, false, false);

				this->m_needSync = true;
			}

		}
		else
		{

			if (m_currRot > m_targetRot)
			{
				//this->m_pComp->PC_pos = DirectX::XMVectorAdd(offSet, this->m_pComp->PC_pos);
				m_currRot -= sinRot;
				
			rot = DirectX::XMMatrixRotationAxis(this->m_pComp->PC_OBB.ort.r[1], DirectX::XMConvertToRadians(-sinRot));
			//rot = rot * offSet_matrix;
			//ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, rot);
			//DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(offSet_matrix);
			//ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, DirectX::XMMatrixInverse(&det, offSet_matrix));
			//Translate back to original position (origin)
			rot = DirectX::XMMatrixMultiply(rot, pivotTranslation);
			pivotTranslation = DirectX::XMMatrixTranslation(vectorToTranslate.m128_f32[0], vectorToTranslate.m128_f32[1], vectorToTranslate.m128_f32[2]);
			rot = DirectX::XMMatrixMultiply(rot, pivotTranslation);
			ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, rot);
			//->PC_pos = DirectX::XMVector3TransformCoord(ptr->PC_pos, rot);
			//Turn back the inverted vector and create a matrix that will translate back to origin
			//ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, pivotTranslation);
			//ptr->PC_pos = DirectX::XMVector3TransformCoord(ptr->PC_pos, pivotTranslation);
			//ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, rot);
			//DirectX::XMVECTOR deter = DirectX::XMMatrixDeterminant(offSet_matrix);
			////ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, DirectX::XMMatrixInverse(&deter, offSet_matrix));
			//ptr->PC_pos = DirectX::XMVector3TransformCoord(ptr->PC_pos, DirectX::XMMatrixInverse(&deter, offSet_matrix));
			}
			else {
				m_animationActive = false;
				m_currRot = m_targetRot;
				DirectX::XMFLOAT3 pos;
				DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
				//SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_LEVER, pos, false, false);

				this->m_needSync = true;
			}

		}

			//this->m_pComp->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, DirectX::XMVectorScale(this->m_originalOrto.r[2], -1));
			//this->m_pComp->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, DirectX::XMVectorScale(this->m_originalOrto.r[0], 1));
	//	this->m_pComp->PC_pos = DirectX::XMVectorSubtract(offSet, this->m_pComp->PC_pos);
	
		//ptr->PC_rotation.m128_f32[0] += m_activatedRotation;
		this->SyncComponents();
		//offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[2], -1.2f));
		
		this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, offSet_matrix);

	//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, offSet_matrix);


	}
	//
	//if (this->m_isOpened)
	//{
	//	this->m_pComp->PC_pos = this->m_closed_pos;
	//	if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_maxRotation)
	//	{
	//		//this->m_pComp->PC_rotationVelocity = DirectX::XMVectorSet(0, -0.2, 0, 0);
	//		//DirectX::XMVECTOR rotationX = this->m_pComp->PC_OBB.ort.r[0];
	//		//DirectX::XMVECTOR rotationY = this->m_pComp->PC_OBB.ort.r[1];
	//		//DirectX::XMVECTOR rotationZ = this->m_pComp->PC_OBB.ort.r[2];

	//		//float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);

	//		//float rotationValue = this->m_rotatePerSec * dT;
	//		//if (rotationValue + currentYRotation > this->m_maxRotation)
	//		//{
	//		//	rotationValue = this->m_maxRotation - currentYRotation;
	//		//}
	//		//

	//		////We do not need to rotate the Y-axis
	//		//We need to limit the rotation so it doesn't go over the limits
	//		
	//		//this->m_gComp->modelPtr->GetOBBData().


	//		//this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), yRotationMatrix);

	//		//rotationX = DirectX::XMVector3Transform(rotationX, yRotationMatrix);
	//		//rotationZ = DirectX::XMVector3Transform(rotationZ, yRotationMatrix);
	//		//this->m_pComp->PC_OBB.ort.r[0] = rotationX;
	//		//this->m_pComp->PC_OBB.ort.r[2] = rotationZ;

	//		//this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + rotationValue);
	//		//if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_maxRotation)
	//		//{
	//		//	this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_maxRotation);
	//		//}

	//		//DirectX::XMVECTOR test = DirectX::XMVectorSet(1.2, -1.5, 0, 0);
	//		//DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslationFromVector(test);
	//		//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, matrix);
	//	}

	//	this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 3.14f / 2.0f, 1);
	//	this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(this->m_originalOrto, DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(this->m_pComp->PC_rotation)));

	//	this->m_pComp->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, DirectX::XMVectorScale(this->m_originalOrto.r[2], -1));
	//	this->m_pComp->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, DirectX::XMVectorScale(this->m_originalOrto.r[0], 1));


	//	this->SyncComponents();
	//	DirectX::XMVECTOR offSet = DirectX::XMVectorScale(this->m_originalOrto.r[1], -1.5f);
	//	offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_originalOrto.r[2], -0.2f));
	//	offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_originalOrto.r[0], -1.2f));
	//	DirectX::XMMATRIX offSet_matrix = DirectX::XMMatrixTranslationFromVector(offSet);
	//	this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, offSet_matrix);



	//}
	//else
	//{

	//	this->m_pComp->PC_pos = this->m_closed_pos;

	//	if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_minRotation)
	//	{
	//		//this->m_pComp->PC_rotationVelocity = DirectX::XMVectorSet(0, 0.2, 0, 0);
	//		//DirectX::XMVECTOR rotationX = this->m_pComp->PC_OBB.ort.r[0];
	//		//DirectX::XMVECTOR rotationY = this->m_pComp->PC_OBB.ort.r[1];
	//		////DirectX::XMVECTOR rotationZ = this->m_pComp->PC_OBB.ort.r[2];

	//		//float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);

	//		////float rotationValue = this->m_rotatePerSec * dT * -1;
	//		////if (rotationValue + currentYRotation < this->m_minRotation)
	//		////{
	//		////	rotationValue = this->m_minRotation - currentYRotation;
	//		////	this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
	//		////}


	//		//////We do not need to rotate the Y-axis
	//		//DirectX::XMMATRIX yRotationMatrix = DirectX::XMMatrixRotationY(-3.14 / 2);
	//		////We need to limit the rotation so it doesn't go over the limits

	//		//this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
	//		//this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), yRotationMatrix);
	//		//this->m_isOpened = false;
	//		//rotationX = DirectX::XMVector3Transform(rotationX, yRotationMatrix);
	//		//rotationZ = DirectX::XMVector3Transform(rotationZ, yRotationMatrix);
	//		//this->m_pComp->PC_OBB.ort.r[0] = rotationX;
	//		//this->m_pComp->PC_OBB.ort.r[2] = rotationZ;

	//		//this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + rotationValue);
	//		//if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_minRotation)
	//		//{
	//		//	this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_minRotation);
	//		//}


	//		//DirectX::XMVECTOR test = DirectX::XMVectorSet(1.2, -1.5, 0, 0);
	//		//DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslationFromVector(test);
	//		//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, matrix);
	//	}
	//	this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
	//	this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(this->m_originalOrto, DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(this->m_pComp->PC_rotation)));
	//	
	//	this->SyncComponents();

	//}
		


	return 0;
}

int DoorEntity::React(int entityID, EVENT reactEvent)
{
	//Kims stuff, "crazy but elegant" - Oscar 2017-01-23
	//I, Kim, do not remember our old crazy solution.
	int i = 0;
	for (std::vector<ElementState>::iterator element = this->m_subjectStates.begin(); element != this->m_subjectStates.end(); element++)
	{
		//Match the EntityID
		if ((element->entityID == entityID))
		{
			//There is a special case of Wheel Incrementation
			if (element->desiredState < EVENT::WHEEL_0 || element->desiredState > EVENT::WHEEL_100)
			{
				//We are not listening for incrementation events from wheels
				element->desiredStateReached = element->desiredState == reactEvent;
			}
			else
			{
				//We are listening for incrementation events from wheel so ignore state events
				if (reactEvent != EVENT::WHEEL_INCREASING && reactEvent != EVENT::WHEEL_DECREASING && reactEvent != EVENT::WHEEL_RESET)
				{
					element->desiredStateReached = element->desiredState == reactEvent;
				}
			}
		}
		i += element->desiredStateReached;
	}
	//The state of the door changed, it iseither not opened or not closed anymore
	if (this->m_isOpened != (i == this->m_subjectStates.size()))
	{
		if(m_isOpened)
			m_targetRot = 0;
		else
			m_targetRot = -m_activatedRotation;
		//Play sound
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
		m_animationActive = true;
		SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_DOOR_OPENING, pos, false, false);
	}

	this->m_isOpened = i == this->m_subjectStates.size();


	return 0;
}

bool DoorEntity::SetIsOpened(bool isOpened)
{
	bool lastValue = this->m_isOpened;
	this->m_isOpened = isOpened;
	return lastValue;
}

bool DoorEntity::GetIsOpened()
{
	return this->m_isOpened;
}

bool DoorEntity::AddSubjectState(ElementState subjectState)
{
	bool result = true;
	this->m_subjectStates.push_back(subjectState);
	return result;
}

bool DoorEntity::AddSubjectState(unsigned int entityID, EVENT requiredEvent)
{
	bool result = true;
	ElementState newElementState;
	newElementState.entityID = entityID;
	newElementState.desiredState = requiredEvent;
	newElementState.desiredStateReached = false;
	this->m_subjectStates.push_back(newElementState);
	return result;
}
