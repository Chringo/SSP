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
			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + (this->m_rotatePerSec * dT));
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
			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) - (this->m_rotatePerSec * dT));
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
