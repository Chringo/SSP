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

	this->m_isOpened = false;
	this->m_rotateTime = rotateTime;
	this->m_minRotation = minRotation;
	this->m_maxRotation = maxRotation;
	this->m_rotatePerSec = this->m_maxRotation / this->m_rotateTime;
	this->SyncComponents();

	this->m_subjectStates = subjectStates;

	return 0;
}

int DoorEntity::Update(float dT, InputHandler * inputHandler)
{
	if (this->m_isOpened)
	{
		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_maxRotation)
		{
			DirectX::XMVECTOR rotationX = this->m_pComp->PC_OBB.ort.r[0];
			DirectX::XMVECTOR rotationY = this->m_pComp->PC_OBB.ort.r[1];
			DirectX::XMVECTOR rotationZ = this->m_pComp->PC_OBB.ort.r[2];

			float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);

			float rotationValue = this->m_rotatePerSec * dT;
			if (rotationValue + currentYRotation > this->m_maxRotation)
			{
				rotationValue = this->m_maxRotation - currentYRotation;
			}
			

			//We do not need to rotate the Y-axis
			DirectX::XMMATRIX yRotationMatrix = DirectX::XMMatrixRotationY(rotationValue);
			//We need to limit the rotation so it doesn't go over the limits

			rotationX = DirectX::XMVector3Transform(rotationX, yRotationMatrix);
			rotationZ = DirectX::XMVector3Transform(rotationZ, yRotationMatrix);
			this->m_pComp->PC_OBB.ort.r[0] = rotationX;
			this->m_pComp->PC_OBB.ort.r[2] = rotationZ;

			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + rotationValue);
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_maxRotation)
			{
				this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_maxRotation);
			}
			this->SyncComponents();
		}
	}
	else
	{
		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_minRotation)
		{
			DirectX::XMVECTOR rotationX = this->m_pComp->PC_OBB.ort.r[0];
			DirectX::XMVECTOR rotationY = this->m_pComp->PC_OBB.ort.r[1];
			DirectX::XMVECTOR rotationZ = this->m_pComp->PC_OBB.ort.r[2];

			float currentYRotation = DirectX::XMVectorGetY(this->m_pComp->PC_rotation);

			float rotationValue = this->m_rotatePerSec * dT * -1;
			if (rotationValue + currentYRotation < this->m_minRotation)
			{
				rotationValue = this->m_minRotation - currentYRotation;
			}


			//We do not need to rotate the Y-axis
			DirectX::XMMATRIX yRotationMatrix = DirectX::XMMatrixRotationY(rotationValue);
			//We need to limit the rotation so it doesn't go over the limits

			rotationX = DirectX::XMVector3Transform(rotationX, yRotationMatrix);
			rotationZ = DirectX::XMVector3Transform(rotationZ, yRotationMatrix);
			this->m_pComp->PC_OBB.ort.r[0] = rotationX;
			this->m_pComp->PC_OBB.ort.r[2] = rotationZ;

			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + rotationValue);
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_minRotation)
			{
				this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_minRotation);
			}
			this->SyncComponents();
		}
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
