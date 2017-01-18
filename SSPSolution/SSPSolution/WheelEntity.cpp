#include "WheelEntity.h"



WheelEntity::WheelEntity()
{
	this->m_rotationState = 0;
}


WheelEntity::~WheelEntity()
{
}

int WheelEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, float interactionDistance, float minRotation, float maxRotation, float rotateTime)
{
	this->InitializeBase(entityID, pComp, gComp);
	int result = 0;
	this->m_rotationState = 0;
	this->m_minRotation = minRotation;
	this->m_maxRotation = maxRotation;
	this->m_rotateTime = rotateTime;
	this->m_rotatePerSec = (this->m_maxRotation - this->m_minRotation) / this->m_rotateTime;
	this->m_range = interactionDistance;

	this->SyncComponents();

	return result;
}

int WheelEntity::Update(float dT, InputHandler * inputHandler)
{
	switch (this->m_rotationState)
	{
	case 0:
		break;
	case 1:
		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_maxRotation)
		{
			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + (this->m_rotatePerSec * dT));
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_maxRotation)
			{
				this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_maxRotation);
				this->m_rotationState = 0;
				this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_100);
			}
			this->SyncComponents();
		}
		break;
	case -1:
		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_minRotation)
		{
			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) - (this->m_rotatePerSec * dT));
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_minRotation)
			{
				this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_minRotation);
				this->m_rotationState = 0;
				this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_0);
			}
			this->SyncComponents();
		}
		break;
	case 2:
	case -2:
		break;
	default:
		break;
	}

		
	return 0;
}

int WheelEntity::React(int entityID, EVENT reactEvent)
{
	return 0;
}

int WheelEntity::CheckPlayerInteraction(DirectX::XMFLOAT3 playerPos, bool increasing)
{
	int result = 0;
	DirectX::XMFLOAT3 myPos;
	DirectX::XMStoreFloat3(&myPos, this->m_pComp->PC_pos);
	
	if (abs(DirectX::XMVectorGetX(this->m_pComp->PC_pos) - playerPos.x) < this->m_range
		&& abs(DirectX::XMVectorGetY(this->m_pComp->PC_pos) - playerPos.y) < this->m_range
		&& abs(DirectX::XMVectorGetZ(this->m_pComp->PC_pos) - playerPos.z) < this->m_range)
	{
		if (increasing)
		{
			this->m_rotationState = 1;
			this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_INCREASING);
		}
		else
		{
			this->m_rotationState = -1;
			this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_DECREASING);
		}
	}
	else
	{
		this->m_rotationState = 0;
	}
	return result;
}
