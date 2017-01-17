#include "DoorEntity.h"

DoorEntity::DoorEntity()
{
}


DoorEntity::~DoorEntity()
{
}

int DoorEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp)
{
	this->InitializeBase(entityID, pComp, gComp);

	this->m_isOpened = false;

	return 0;
}

int DoorEntity::Update(float dT, InputHandler * inputHandler)
{
	this->SyncComponents();

	return 0;
}

int DoorEntity::React(int entityID, EVENT reactEvent)
{
	if (reactEvent == EVENT::BUTTON_PRESSED)
	{
		DirectX::XMVECTOR rotation;
		if (!this->m_isOpened)
		{
			this->m_isOpened = true;
			this->m_pComp->PC_rotation = DirectX::XMVectorSet(0.f, 3.f, 0.f, 0.f);
		} 
		else
		{
			this->m_isOpened = false;
			this->m_pComp->PC_rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);
		}
	}

	return 0;
}
