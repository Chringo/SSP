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
			//Rotation in percentage = rotationAMount / maxRotation
			float rotationAmount = rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / this->m_maxRotation;
			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) + (this->m_rotatePerSec * dT));
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_maxRotation)
			{
				this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_maxRotation);
				this->m_rotationState = 0;
				this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_100);
			}
			else {
				//Check if the rotation increase has exceeded a 10% increment
				//Rotation increase
				int percentIncOld = (int)(rotationAmount * 10.0f);
				rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / this->m_maxRotation;
				int percentIncNew = (int)(rotationAmount * 10.0f);

				//Now we have calculated the amount of 10percent incrementation of the rotation we have and can check if it has changed
				if (percentIncNew != percentIncOld)
				{
					//The increment has changed. Calculate the new percentIncrement and notify with appropriate event
					//The event to notify with is the WHEEL_0 event + the increment.
					this->m_subject.Notify(this->m_entityID, EVENT(EVENT::WHEEL_0 + percentIncNew));
				}
			}

			this->SyncComponents();
		}
		break;
	case -1:
		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_minRotation)
		{
			float rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / this->m_maxRotation;
			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) - (this->m_rotatePerSec * dT));
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_minRotation)
			{
				this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_minRotation);
				this->m_rotationState = 0;
				this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_0);
			}
			else
			{
				//Check if the rotation increase has exceeded a 10% increment
				//Rotation decrease
				int percentIncOld = (int)(rotationAmount * 10.0f);
				rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / this->m_maxRotation;
				int percentIncNew = (int)(rotationAmount * 10.0f);

				//Now we have calculated the amount of 10percent incrementation of the rotation we have and can check if it has changed
				if (percentIncNew != percentIncOld)
				{
					//The increment has changed. Calculate the new percentIncrement and notify with appropriate event
					//The event to notify with is the WHEEL_0 event + the increment.
					this->m_subject.Notify(this->m_entityID, EVENT(EVENT::WHEEL_0 + percentIncNew));
				}
			}
			this->SyncComponents();
		}
		break;
	case 2:
		break;
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
			//Check if max has been reached
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) >= this->m_maxRotation)
			{
				this->m_rotationState = 0;
			}
			else
			{
				this->m_rotationState = 1;
				this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_INCREASING);
			}
		}
		else
		{
			//Check if min has been reached
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) <= this->m_maxRotation)
			{
				this->m_rotationState = 0;
			}
			else
			{
				this->m_rotationState = -1;
				this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_DECREASING);
			}
		}
	}
	else
	{
		this->m_rotationState = 0;
	}
	return result;
}
