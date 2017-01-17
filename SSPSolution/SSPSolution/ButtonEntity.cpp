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
	return result;
}

int ButtonEntity::React(int entityID, EVENT reactEvent)
{
	int result = 0;
	return result;
}

int ButtonEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp)
{
	int result = 0;
	this->InitializeBase(entityID, pComp, gComp);
	this->m_isActive = 0;
	this->m_range = 5.0f;
	this->SyncComponents();
	return result;
}

int ButtonEntity::AddObserver(Observer * observer, int entityID)
{
	this->m_subject.AddObserver(observer, entityID);

	return 0;
}

int ButtonEntity::CheckPressed(DirectX::XMFLOAT3 playerPos)
{
	if (abs(DirectX::XMVectorGetX(this->m_pComp->PC_pos) - playerPos.x) < this->m_range
		&& abs(DirectX::XMVectorGetY(this->m_pComp->PC_pos) - playerPos.y) < this->m_range
		&& abs(DirectX::XMVectorGetZ(this->m_pComp->PC_pos) - playerPos.z) < this->m_range)
	{
		this->m_subject.Notify(this->m_entityID, EVENT::BUTTON_PRESSED);
	}

	return 0;
}
