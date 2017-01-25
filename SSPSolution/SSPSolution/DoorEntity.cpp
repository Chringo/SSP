#include "DoorEntity.h"

DoorEntity::DoorEntity()
{
}


DoorEntity::~DoorEntity()
{
}

int DoorEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, std::vector<ElementState> subjectStates, float rotateTime, float minRotation, float maxRotation)
{
	this->InitializeBase(entityID, pComp, gComp);

	this->m_isOpened = false;
	this->m_rotateTime = rotateTime;
	this->m_minRotation = minRotation;
	this->m_maxRotation = maxRotation;
	this->m_rotatePerSec = this->m_maxRotation / this->m_rotateTime;
	this->SyncComponents();

	this->m_subjectStates = subjectStates;

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
	//Kims stuff, "crazy but elegant" - Oscar 2017-01-23
	//this->m_isOpened = reactEvent == EVENT::BUTTON_ACTIVE;
	int i = 0;
	for (std::vector<ElementState>::iterator element = this->m_subjectStates.begin(); element != this->m_subjectStates.end(); element++)
	{
		if ((element->entityID == entityID))
		{
			if (EVENT::WHEEL_100 == reactEvent)
			{
				int b = 27;
			}
			if (EVENT::BUTTON_ACTIVE == reactEvent)
			{
				int b = 27;
			}
			element->desiredStateReached = element->desiredState == reactEvent;
		}
		i += element->desiredStateReached;
		if (i == 2)
			int b = 36;
	}
	/*for each (ElementState element in this->m_subjectStates)
	{
		if ((element.entityID == entityID))
		{
			if (EVENT::WHEEL_100 == reactEvent)
			{
				int b = 27;
			}
			if (EVENT::BUTTON_ACTIVE == reactEvent)
			{
				int b = 27;
			}
			element.desiredStateReached = element.desiredState == reactEvent;
		}
		i += element.desiredStateReached;
		if (i == 2)
			int b = 36;
	}*/
	this->m_isOpened = i == this->m_subjectStates.size();

/*	if (reactEvent == EVENT::BUTTON_ACTIVE)
	{
		this->m_isOpened = true;
		this->m_subject.Notify(this->m_entityID, EVENT::DOOR_OPENED);
	}
	else if(reactEvent == EVENT::BUTTON_DEACTIVE)
	{
		this->m_isOpened = false;
		this->m_subject.Notify(this->m_entityID, EVENT::DOOR_CLOSED);
	}
	else if (reactEvent == EVENT::WHEEL_100)
	{
		this->m_isOpened = true;
		this->m_subject.Notify(this->m_entityID, EVENT::DOOR_OPENED);
	}
	else if (reactEvent == EVENT::WHEEL_0)
	{
		this->m_isOpened = false;
		this->m_subject.Notify(this->m_entityID, EVENT::DOOR_CLOSED);
	}
	else if (reactEvent == EVENT::LEVER_ACTIVE)
	{
		this->m_isOpened = true;
		this->m_subject.Notify(this->m_entityID, EVENT::DOOR_OPENED);
	}
	else if (reactEvent == EVENT::LEVER_DEACTIVE)
	{
		this->m_isOpened = false;
		this->m_subject.Notify(this->m_entityID, EVENT::DOOR_CLOSED);
	}*/

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
