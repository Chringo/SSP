#include "ButtonEntity.h"



void ButtonEntity::setActiveTimerSound(bool activate)
{
	if (activate)
	{
		if (this->m_timer_sound == nullptr)
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
			this->m_timer_sound = SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_BUTTON_TIMER, pos, false, true);
		}
		else
		{
			if (this->m_timer_sound->getIsPaused())
			{
				this->m_timer_sound->setIsPaused(false);
			}
		}
	}
	else
	{
		if (this->m_timer_sound != nullptr)
		{
			if (!this->m_timer_sound->getIsPaused())
			{
				this->m_timer_sound->setIsPaused(true);
				this->m_timer_sound->setPlayPosition(0);
			}
		}
	}
}

ButtonEntity::ButtonEntity()
{
}


ButtonEntity::~ButtonEntity()
{
	if (this->m_timer_sound != nullptr)
	{
		this->m_timer_sound->stop();
		this->m_timer_sound->drop();
	}
}

int ButtonEntity::Update(float dT, InputHandler * inputHandler)
{
	int result = 0;

	if (this->m_isActive && this->m_resetTime > 0.0f)
	{
		this->m_elapsedResetTime -= dT;

		//if (this->m_timer_sound == nullptr)
		//{
		//	DirectX::XMFLOAT3 pos;
		//	DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
		//	this->m_timer_sound = SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_BUTTON_TIMER, pos, true, true);
		//}
		//else if (this->m_timer_sound->getIsPaused())
		//{
		//	this->m_timer_sound->setPlayPosition(0);
		//	this->m_timer_sound->setIsPaused(false);
		//}
		this->setActiveTimerSound(true);
		if (this->m_timer_sound != nullptr)
		{
			if (this->m_resetTime < 10.0f && this->m_timer_sound->getPlaybackSpeed() != 2.0f)
			{
				this->m_timer_sound->setPlaybackSpeed(2.0f);
			}
		}

		if (this->m_elapsedResetTime <= 0.0f)
		{
			//Reset the button
			this->m_elapsedResetTime = this->m_resetTime;
			this->m_isActive = false;
			this->m_subject.Notify(this->m_entityID, EVENT::BUTTON_DEACTIVE);
			m_targetOffset = 0;
			m_animationActive = true;
			//Play sound
			//DirectX::XMFLOAT3 pos;
			//DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
			//SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_BUTTON_CLICKED, pos, false, false);
			this->setActiveTimerSound(false);
			this->m_timer_sound->setPlaybackSpeed(1.0f);
			this->m_needSync = true;
		}
	}
	if (m_animationActive)
	{
		PhysicsComponent* ptr = this->GetPhysicsComponent();
		DirectX::XMMATRIX offsetMatrix;
		float frameOffset = m_animSpeed * dT;
		if (m_targetOffset == 0)
		{
			if (m_currOffsetValue + frameOffset < m_targetOffset)
			{
				m_currOffsetValue += frameOffset;
				
				offsetMatrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(ptr->PC_OBB.ort.r[0], m_currOffsetValue));

				this->m_lastFrameOffset = m_currOffsetValue;
			}
			else {
				m_animationActive = false;
				m_currOffsetValue = m_targetOffset;
				this->m_subject.Notify(this->m_entityID, EVENT(EVENT::BUTTON_DEACTIVE + this->m_isActive));
				DirectX::XMFLOAT3 pos;
				DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
				
				
				offsetMatrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(ptr->PC_OBB.ort.r[0], m_currOffsetValue));

			}
		}
		else
		{
			if (m_currOffsetValue - frameOffset > m_targetOffset)
			{
				m_currOffsetValue -= frameOffset;
				offsetMatrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(ptr->PC_OBB.ort.r[0], m_currOffsetValue));
				this->m_lastFrameOffset = m_currOffsetValue;
			}
			else {
				m_animationActive = false;
				m_currOffsetValue = m_targetOffset;
				this->m_subject.Notify(this->m_entityID, EVENT(EVENT::BUTTON_DEACTIVE + this->m_isActive));
				DirectX::XMFLOAT3 pos;
				DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
				offsetMatrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(ptr->PC_OBB.ort.r[0], m_currOffsetValue));
				return 0;
			}
		}
		this->SyncComponents();
		this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, offsetMatrix);
		//ptr->PC_OBB.ort = DirectX::XMMatrixMultiply(ptr->PC_OBB.ort, rot);

	}
	return result;
}

int ButtonEntity::React(unsigned int entityID, EVENT reactEvent)
{
	int result = 0;
	//If a button receives a LEVER::ACTIVATED or BUTTON::ACTIVATE event, deactivate this lever
	if (reactEvent == EVENT::LEVER_ACTIVE || reactEvent == EVENT::BUTTON_ACTIVE)
	{
		this->m_isActive = false;
		this->m_subject.Notify(this->m_entityID, EVENT::BUTTON_DEACTIVE);

		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
		SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_BUTTON_CLICKED, pos, false, false);

	}
	return result;
}


int ButtonEntity::Initialize(unsigned int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, float interactionDistance, float resetTime)
{
	int result = 0;
	this->InitializeBase(entityID, pComp, gComp, nullptr);
	this->m_isActive = false;
	this->m_needSync = false;
	this->m_range = interactionDistance;
	this->m_resetTime = resetTime;
	this->m_elapsedResetTime = 0.0f;
	this->m_lastFrameOffset = 0;
	this->m_timer_sound = nullptr;
	this->SyncComponents();
	return result;
}

int ButtonEntity::CheckPressed(DirectX::XMFLOAT3 playerPos)
{
	if (abs(DirectX::XMVectorGetX(this->m_pComp->PC_pos) - playerPos.x) < this->m_range
		&& abs(DirectX::XMVectorGetY(this->m_pComp->PC_pos) - playerPos.y) < this->m_range
		&& abs(DirectX::XMVectorGetZ(this->m_pComp->PC_pos) - playerPos.z) < this->m_range)
	{
		this->m_isActive = !this->m_isActive;
		this->m_elapsedResetTime = this->m_resetTime;
		this->m_subject.Notify(this->m_entityID, EVENT(EVENT::BUTTON_DEACTIVE + this->m_isActive));
		
		if (m_isActive) {
			m_targetOffset = m_activatedOffset;
		}
		else {
			m_targetOffset = 0;
		}
		m_animationActive = true;

		////Play sound
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
		SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_BUTTON_CLICKED, pos, false, false);
		if (!this->m_isActive)
		{
			this->setActiveTimerSound(false);
			this->m_timer_sound->setPlaybackSpeed(1.f);
		}
		else
		{
			this->setActiveTimerSound(true);
		}
		
		this->m_needSync = true;
	}

	return 0;
}

void ButtonEntity::SetSyncState(ButtonSyncState * newSyncState)
{
	if (newSyncState != nullptr)
	{
		//The player is always the cause of the state change
		this->m_isActive = newSyncState->isActive;
		this->m_elapsedResetTime = this->m_resetTime;
		this->m_subject.Notify(this->m_entityID, EVENT(EVENT::BUTTON_DEACTIVE + this->m_isActive));

		if (m_isActive) {
			this->m_targetOffset = m_activatedOffset;
		}
		else {
			this->m_targetOffset = 0;
		}
		this->m_animationActive = true;

		//Play sound
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
		SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_BUTTON_CLICKED, pos, false, false);
	}
}

ButtonSyncState * ButtonEntity::GetSyncState()
{
	ButtonSyncState* result = nullptr;
	if (this->m_needSync)
	{
		result = new ButtonSyncState{this->m_entityID, this->m_isActive};
		this->m_needSync = false;
	}
	return result;
}

bool ButtonEntity::GetIsActive()
{
	return this->m_isActive;
}
