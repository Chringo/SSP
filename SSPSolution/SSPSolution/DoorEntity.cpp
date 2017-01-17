#include "DoorEntity.h"

DoorEntity::DoorEntity()
{
}


DoorEntity::~DoorEntity()
{
}

int DoorEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, float minRotation, float maxRotation, float rotateTime)
{
	this->InitializeBase(entityID, pComp, gComp);

	this->m_isOpened = false;
	this->m_minRotation = minRotation;
	this->m_maxRotation = maxRotation;
	this->m_rotateTime = rotateTime;
	this->m_rotatePerSec = (this->m_maxRotation - this->m_minRotation) / this->m_rotateTime;
	this->SyncComponents();

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
	if (reactEvent == EVENT::BUTTON_PRESSED)
	{
		if (!this->m_isOpened)
		{
			this->m_isOpened = true;
			this->m_subject.Notify(this->m_entityID, EVENT::DOOR_OPENED);
		} 
		else
		{
			this->m_isOpened = false;
			this->m_subject.Notify(this->m_entityID, EVENT::DOOR_CLOSED);
		}
	}

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
