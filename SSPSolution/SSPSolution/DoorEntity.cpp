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

	this->m_subjectStates = subjectStates;

	return 0;
}

int DoorEntity::Update(float dT, InputHandler * inputHandler)
{
	if (this->m_isOpened)
	{
		this->m_pComp->PC_pos = this->m_closed_pos;
		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_maxRotation)
		{
			//this->m_pComp->PC_rotationVelocity = DirectX::XMVectorSet(0, -0.2, 0, 0);
			//DirectX::XMVECTOR rotationX = this->m_pComp->PC_OBB.ort.r[0];
			//DirectX::XMVECTOR rotationY = this->m_pComp->PC_OBB.ort.r[1];
			//DirectX::XMVECTOR rotationZ = this->m_pComp->PC_OBB.ort.r[2];

			//float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);

			//float rotationValue = this->m_rotatePerSec * dT;
			//if (rotationValue + currentYRotation > this->m_maxRotation)
			//{
			//	rotationValue = this->m_maxRotation - currentYRotation;
			//}
			//

			////We do not need to rotate the Y-axis
			//We need to limit the rotation so it doesn't go over the limits
			
			//this->m_gComp->modelPtr->GetOBBData().


			//this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), yRotationMatrix);

			//rotationX = DirectX::XMVector3Transform(rotationX, yRotationMatrix);
			//rotationZ = DirectX::XMVector3Transform(rotationZ, yRotationMatrix);
			//this->m_pComp->PC_OBB.ort.r[0] = rotationX;
			//this->m_pComp->PC_OBB.ort.r[2] = rotationZ;

			//this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + rotationValue);
			//if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_maxRotation)
			//{
			//	this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_maxRotation);
			//}

			//DirectX::XMVECTOR test = DirectX::XMVectorSet(1.2, -1.5, 0, 0);
			//DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslationFromVector(test);
			//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, matrix);
		}

		this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 3.14 /2, 1);
		this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(this->m_originalOrto, DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(this->m_pComp->PC_rotation)));

		this->m_pComp->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, DirectX::XMVectorScale(this->m_originalOrto.r[2], -1));
		this->m_pComp->PC_pos = DirectX::XMVectorAdd(this->m_pComp->PC_pos, DirectX::XMVectorScale(this->m_originalOrto.r[0], 1));


		this->SyncComponents();
		DirectX::XMVECTOR offSet = DirectX::XMVectorScale(this->m_originalOrto.r[1], -1.5);
		offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_originalOrto.r[2], -0.2));
		offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_originalOrto.r[0], -1.2));
		DirectX::XMMATRIX offSet_matrix = DirectX::XMMatrixTranslationFromVector(offSet);
		this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, offSet_matrix);



	}
	else
	{

		this->m_pComp->PC_pos = this->m_closed_pos;

		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_minRotation)
		{
			//this->m_pComp->PC_rotationVelocity = DirectX::XMVectorSet(0, 0.2, 0, 0);
			//DirectX::XMVECTOR rotationX = this->m_pComp->PC_OBB.ort.r[0];
			//DirectX::XMVECTOR rotationY = this->m_pComp->PC_OBB.ort.r[1];
			////DirectX::XMVECTOR rotationZ = this->m_pComp->PC_OBB.ort.r[2];

			//float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);

			////float rotationValue = this->m_rotatePerSec * dT * -1;
			////if (rotationValue + currentYRotation < this->m_minRotation)
			////{
			////	rotationValue = this->m_minRotation - currentYRotation;
			////	this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
			////}


			//////We do not need to rotate the Y-axis
			//DirectX::XMMATRIX yRotationMatrix = DirectX::XMMatrixRotationY(-3.14 / 2);
			////We need to limit the rotation so it doesn't go over the limits

			//this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
			//this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), yRotationMatrix);
			//this->m_isOpened = false;
			//rotationX = DirectX::XMVector3Transform(rotationX, yRotationMatrix);
			//rotationZ = DirectX::XMVector3Transform(rotationZ, yRotationMatrix);
			//this->m_pComp->PC_OBB.ort.r[0] = rotationX;
			//this->m_pComp->PC_OBB.ort.r[2] = rotationZ;

			//this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + rotationValue);
			//if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_minRotation)
			//{
			//	this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_minRotation);
			//}


			//DirectX::XMVECTOR test = DirectX::XMVectorSet(1.2, -1.5, 0, 0);
			//DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslationFromVector(test);
			//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, matrix);
		}
		this->m_pComp->PC_rotation = DirectX::XMVectorSetByIndex(this->m_pComp->PC_rotation, 0, 1);
		this->m_pComp->PC_OBB.ort = DirectX::XMMatrixMultiply(this->m_originalOrto, DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(this->m_pComp->PC_rotation)));
		
		this->SyncComponents();
		DirectX::XMVECTOR offSet = DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[1], -1.5);
		offSet = DirectX::XMVectorAdd(offSet, DirectX::XMVectorScale(this->m_pComp->PC_OBB.ort.r[2], -1.2));
		DirectX::XMMATRIX offSet_matrix = DirectX::XMMatrixTranslationFromVector(offSet);
		this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, offSet_matrix);

	}


	return 0;
}

int DoorEntity::React(int entityID, EVENT reactEvent)
{
	//Kims stuff, "crazy but elegant" - Oscar 2017-01-23
	//this->m_isOpened = reactEvent == EVENT::BUTTON_ACTIVE;
	int i = 0;
	for (std::vector<ElementState>::iterator element = this->m_subjectStates.begin(); element != this->m_subjectStates.end(); element++)
	{
		if ((element->entityID == entityID))
		{
			element->desiredStateReached = element->desiredState == reactEvent;
		}
		i += element->desiredStateReached;
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
