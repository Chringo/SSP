#include "WheelEntity.h"



WheelEntity::WheelEntity()
{
	this->m_rotationState = 0;
}


WheelEntity::~WheelEntity()
{
}

int WheelEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, float interactionDistance, float minRotation, float maxRotation, float rotateTime, bool resets, float resetTime, float timeUntilReset)
{
	int result = this->InitializeBase(entityID, pComp, gComp);
	//Load default values
	this->m_rotationState = 0;
	this->m_isMin = true;
	//Load modifiable settings
	this->m_range = interactionDistance;

	this->m_minRotation = minRotation * DirectX::XM_PI * 2;
	this->m_maxRotation = maxRotation * DirectX::XM_PI * 2;
	this->m_rotateTime = rotateTime;
	this->m_rotatePerSec = (this->m_maxRotation - this->m_minRotation) / this->m_rotateTime;
	
	this->m_resets = resets;
	this->m_resetTime = resetTime;
	this->m_resetRotatePerSec = (this->m_maxRotation - this->m_minRotation) / this->m_resetTime;
	this->m_timeUntilReset = this->m_resetCountdown = timeUntilReset;

	this->SyncComponents();
	return result;
}

int WheelEntity::Update(float dT, InputHandler * inputHandler)
{
	switch (this->m_rotationState)
	{
	case 0:
		//Check if the wheel can reset
		if (this->m_resets && !this->m_isMin)
		{
			//Increase the reset timer
			this->m_resetCountdown -= dT;
			//Check if it is time to reset the wheel
			if (this->m_resetCountdown < 0)
			{
				this->m_rotationState = -2;
			}
		}
		break;
	case 1:
		this->m_isMin = false;
		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_maxRotation)
		{
			//Rotation in percentage = rotationAMount / maxRotation
			float rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / (this->m_maxRotation - this->m_minRotation);
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
				rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / (this->m_maxRotation - this->m_minRotation);
				int percentIncNew = (int)(rotationAmount * 10.0f);

				//Now we have calculated the amount of 10percent incrementation of the rotation we have and can check if it has changed
				if (percentIncNew != percentIncOld)
				{
					//The increment has changed. Calculate the new percentIncrement and notify with appropriate event
					//In the very rare occurrencee that the percent increase is equal or above 20% we need to send several notifications
					int percentIncDiff = abs(percentIncNew - percentIncOld);
					if ( percentIncDiff > 1)
					{
						//Converter determines if percentInc has increased or not so we can correct the event notification
						int converter = -1;
						if (percentIncNew > percentIncOld)
							converter = 1;
						//Remember that the last event has already been sent
						for (int incIter = 1; incIter < percentIncDiff; ++incIter)
						{
							//EVENT::WHEEL_0 + percentIncOld to get the start value
							this->m_subject.Notify(this->m_entityID, EVENT(EVENT::WHEEL_0 + percentIncOld + incIter * converter));
						}
					}
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
			this->m_isMin = false;
			float rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / (this->m_maxRotation - this->m_minRotation);
			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) - (this->m_rotatePerSec * dT));
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_minRotation)
			{
				this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_minRotation);
				this->m_rotationState = 0;
				this->m_isMin = true;
				this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_0);
			}
			else
			{
				//Check if the rotation increase has exceeded a 10% increment
				//Rotation decrease
				int percentIncOld = (int)((rotationAmount * 10.0f) + FLOAT_FIX);
				rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / (this->m_maxRotation - this->m_minRotation);
				int percentIncNew = (int)((rotationAmount * 10.0f) + FLOAT_FIX);

				//Now we have calculated the amount of 10percent incrementation of the rotation we have and can check if it has changed
				if (percentIncNew != percentIncOld)
				{
					//The increment has changed. Calculate the new percentIncrement and notify with appropriate event
					//In the very rare occurrencee that the percent increase is equal or above 20% we need to send several notifications

					int percentIncDiff = abs(percentIncNew - percentIncOld);
					if (percentIncDiff > 1)
					{
						//Converter determines if percentInc has increased or not so we can correct the event notification
						int converter = -1;
						if (percentIncNew > percentIncOld)
							converter = 1;
						//Remember that the last event has already been sent
						for (int incIter = 1; incIter < percentIncDiff; ++incIter)
						{
							//EVENT::WHEEL_0 + percentIncOld to get the start value
							this->m_subject.Notify(this->m_entityID, EVENT(EVENT::WHEEL_0 + percentIncOld + incIter * converter));
						}
					}
					//The event to notify with is the WHEEL_0 event + the increment.
					this->m_subject.Notify(this->m_entityID, EVENT(EVENT::WHEEL_0 + percentIncNew));
				}
			}
			this->SyncComponents();
		}
		break;
	case -2:
		//Start resetting
		//Check if we have reached the min value
		if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) > this->m_minRotation)
		{
			float rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / (this->m_maxRotation - this->m_minRotation);
			this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, DirectX::XMVectorGetY(this->m_pComp->PC_rotation) - (this->m_resetRotatePerSec * dT));
			if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) < this->m_minRotation)
			{
				this->m_pComp->PC_rotation = DirectX::XMVectorSetY(this->m_pComp->PC_rotation, this->m_minRotation);
				this->m_rotationState = 0;
				this->m_isMin = true;
				this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_0);
			}
			else
			{
				//Check if the rotation increase has exceeded a 10% increment
				//Rotation decrease
				int percentIncOld = (int)((rotationAmount * 10.0f) + FLOAT_FIX);
				rotationAmount = DirectX::XMVectorGetY(this->m_pComp->PC_rotation) / (this->m_maxRotation - this->m_minRotation);
				int percentIncNew = (int)((rotationAmount * 10.0f) + FLOAT_FIX);

				//Now we have calculated the amount of 10% incrementation of the rotation we have and can check if it has changed
				if (percentIncNew != percentIncOld)
				{
					//The increment has changed. Calculate the new percentIncrement and notify with appropriate event

					//In the very rare occurrencee that the percent increase is equal or above 20% we need to send several notifications
					int percentIncDiff = abs(percentIncNew - percentIncOld);
					if (percentIncDiff > 1)
					{
						//Converter determines if percentInc has increased or not so we can correct the event notification
						int converter = -1;
						if (percentIncNew > percentIncOld)
							converter = 1;
						//Remember that the last event has already been sent
						for (int incIter = 1; incIter < percentIncDiff; ++incIter)
						{
							//EVENT::WHEEL_0 + percentIncOld to get the start value
							this->m_subject.Notify(this->m_entityID, EVENT(EVENT::WHEEL_0 + percentIncOld + incIter * converter));
						}
					}
					//The event to notify with is the WHEEL_0 event + the increment.
					this->m_subject.Notify(this->m_entityID, EVENT(EVENT::WHEEL_0 + percentIncNew));
				}
			}
			this->SyncComponents();
		}
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

int WheelEntity::CheckPlayerInteraction(DirectX::XMFLOAT3 playerPos, int increasing)
{
	int result = 0;
	DirectX::XMFLOAT3 myPos;
	DirectX::XMStoreFloat3(&myPos, this->m_pComp->PC_pos);
	int b = 0;
	if (increasing == 0)
	{
		this->m_rotationState = 0;
		this->m_resetCountdown = this->m_timeUntilReset;
	}
	else
	{
		if (abs(DirectX::XMVectorGetX(this->m_pComp->PC_pos) - playerPos.x) < this->m_range
			&& abs(DirectX::XMVectorGetY(this->m_pComp->PC_pos) - playerPos.y) < this->m_range
			&& abs(DirectX::XMVectorGetZ(this->m_pComp->PC_pos) - playerPos.z) < this->m_range)
		{
			if (increasing == 1)
			{
				//Check if max has been reached
				if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) >= this->m_maxRotation)
				{
					this->m_rotationState = 0;
					this->m_resetCountdown = this->m_timeUntilReset;
				}
				else
				{
					this->m_rotationState = 1;
					//this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_INCREASING);
				}
			}
			else if (increasing == -1)
			{
				//Check if min has been reached
				if (DirectX::XMVectorGetY(this->m_pComp->PC_rotation) <= this->m_minRotation)
				{
					this->m_rotationState = 0;
					this->m_resetCountdown = this->m_timeUntilReset;
				}
				else
				{
					this->m_rotationState = -1;
					//this->m_subject.Notify(this->m_entityID, EVENT::WHEEL_DECREASING);
				}
			}
		}
		else
		{
			this->m_rotationState = 0;
			this->m_resetCountdown = this->m_timeUntilReset;
		}
	}
	return result;
}

float WheelEntity::SetMinRotation(float amount)
{
	float oldValue = this->m_minRotation;
	this->m_minRotation = amount;
	return oldValue;
}

float WheelEntity::SetMaxRotation(float amount)
{
	float oldValue = this->m_maxRotation;
	this->m_maxRotation = amount;
	return oldValue;
}

float WheelEntity::SetRotateTime(float time)
{
	float oldValue = this->m_rotateTime;
	this->m_rotateTime = time;
	return oldValue;
}

float WheelEntity::SetInteractionDistance(float distance)
{
	float oldValue = this->m_range;
	this->m_range = distance;
	return oldValue;
}

float WheelEntity::GetMinRotation()
{
	return this->m_minRotation;
}

float WheelEntity::GetMaxRotation()
{
	return this->m_maxRotation;
}

float WheelEntity::GetRotateTime()
{
	return this->m_rotateTime;
}

float WheelEntity::GetInteractionDistance()
{
	return this->m_range;
}
