#include "WheelEntity.h"



WheelEntity::WheelEntity()
{
}


WheelEntity::~WheelEntity()
{
}

int WheelEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, float interactionDistance, float minRotation, float maxRotation, float rotateTime)
{
	this->InitializeBase(entityID, pComp, gComp);
	int result = 0;
	this->m_isDone = false;
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
	return 0;
}

int WheelEntity::React(int entityID, EVENT reactEvent)
{
	return 0;
}

int WheelEntity::CheckPlayerInteraction(DirectX::XMFLOAT3 playerPos)
{
	int result = 0;
	DirectX::XMFLOAT3 myPos;
	DirectX::XMStoreFloat3(&myPos, this->m_pComp->PC_pos);

	if (abs(DirectX::XMVectorGetX(this->m_pComp->PC_pos) - playerPos.x) < this->m_range
		&& abs(DirectX::XMVectorGetY(this->m_pComp->PC_pos) - playerPos.y) < this->m_range
		&& abs(DirectX::XMVectorGetZ(this->m_pComp->PC_pos) - playerPos.z) < this->m_range)
	{
		//this->m_subject.Notify(this->m_entityID, EVENT::BUTTON_PRESSED);

	}
	return result;
}
