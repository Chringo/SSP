#include "ButtonEntity.h"



ButtonEntity::ButtonEntity()
{
}


ButtonEntity::~ButtonEntity()
{
}

int ButtonEntity::Update(float dT, InputHandler * inputHandler)
{
	int result = 0;

	if (this->m_isActive)
	{
		this->m_elapsedResetTime -= dT;
		if (this->m_elapsedResetTime <= 0.0f)
		{
			//Reset the button
			this->m_elapsedResetTime = this->m_resetTime;
			this->m_isActive = false;
		}
	}

	return result;
}

int ButtonEntity::React(int entityID, EVENT reactEvent)
{
	int result = 0;
	return result;
}


int ButtonEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, float interactionDistance, float resetTime)
{
	int result = 0;
	this->InitializeBase(entityID, pComp, gComp);
	this->m_isActive = false;
	this->m_range = interactionDistance;
	this->m_resetTime = resetTime;
	this->m_elapsedResetTime = 0.0f;
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
		this->m_subject.Notify(this->m_entityID, EVENT(EVENT::BUTTON_DEACTIVE + this->m_isActive));
	}

	return 0;
}
