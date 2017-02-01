#include "LeverEntity.h"



LeverEntity::LeverEntity()
{
}


LeverEntity::~LeverEntity()
{
}

int LeverEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, float interactionDistance)
{
	int result = 0;
	this->InitializeBase(entityID, pComp, gComp, nullptr);
	this->m_isActive = 0;
	this->m_needSync = false;
	this->m_range = interactionDistance;
	this->SyncComponents();
	return result;
}

int LeverEntity::Update(float dT, InputHandler * inputHandler)
{
	int result = 0;
	return result;
}

int LeverEntity::React(int entityID, EVENT reactEvent)
{
	int result = 0;
	//If a lever receives a LEVER::ACTIVATED event, deactivate this lever
	if (reactEvent == EVENT::LEVER_ACTIVE)
	{
		this->m_isActive = false;
		this->m_subject.Notify(this->m_entityID, EVENT::LEVER_DEACTIVE);
	}
	return result;
}

int LeverEntity::CheckPressed(DirectX::XMFLOAT3 playerPos)
{
	if (abs(DirectX::XMVectorGetX(this->m_pComp->PC_pos) - playerPos.x) < this->m_range
		&& abs(DirectX::XMVectorGetY(this->m_pComp->PC_pos) - playerPos.y) < this->m_range
		&& abs(DirectX::XMVectorGetZ(this->m_pComp->PC_pos) - playerPos.z) < this->m_range)
	{
		this->m_isActive = !this->m_isActive;
		this->m_subject.Notify(this->m_entityID, EVENT(EVENT::LEVER_DEACTIVE + this->m_isActive));
		this->m_needSync = true;
	}
	return 0;
}

void LeverEntity::SetSyncState(LeverSyncState * newSyncState)
{
	if (newSyncState != nullptr)
	{
		//The player is always the cause of the state change
		this->m_isActive = newSyncState->isActive;
		this->m_subject.Notify(this->m_entityID, EVENT(EVENT::LEVER_DEACTIVE + this->m_isActive));
		
	}
}

LeverSyncState * LeverEntity::GetSyncState()
{
	LeverSyncState* result = nullptr;
	if (this->m_needSync)
	{
		result = new LeverSyncState{this->m_entityID, this->m_isActive};
		this->m_needSync = false;
	}
	return result;
}
