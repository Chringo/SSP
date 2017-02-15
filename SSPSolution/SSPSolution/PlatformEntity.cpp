#include "PlatformEntity.h"
PlatformEntity::PlatformEntity() {}
PlatformEntity::~PlatformEntity()
{
	this->Shutdown();
}
int PlatformEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, AIComponent * aiComp)
{
	int result = 0;
	this->InitializeBase(entityID, pComp, gComp, nullptr, aiComp);
	return result;
}
int PlatformEntity::Shutdown()
{
	int result = 0;
	if (this->m_ActiveSound != nullptr)
		this->m_ActiveSound->drop();
	return result;
}

int PlatformEntity::Update(float deltaTime, InputHandler * inputHandler)
{
	int result = 0;
	this->SyncComponents();

	// Adjust misplaced graphics component - hack...
	this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_pComp->PC_OBB.ort,
		DirectX::XMMatrixTranslationFromVector(
			DirectX::XMVectorSubtract(this->m_pComp->PC_pos,
				DirectX::XMVECTOR{
		m_gComp->modelPtr->GetOBBData().position.x,
			m_gComp->modelPtr->GetOBBData().position.y,
			m_gComp->modelPtr->GetOBBData().position.z, 0})));

	if (this->GetAIComponent()->AC_triggered)
	{
		if (this->m_ActiveSound == nullptr)
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
			this->m_ActiveSound = SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_LIFT, pos, true, true);
		}
		else
		{
			if (this->m_ActiveSound->getIsPaused())
			{
				this->m_ActiveSound->setIsPaused(false);
			}
		}
	}
	else
	{
		if (this->m_ActiveSound != nullptr && !this->m_ActiveSound->getIsPaused())
		{
			this->m_ActiveSound->setPlayPosition(0);
			this->m_ActiveSound->setIsPaused(true);	//Pause the walking sound
		}
	}
	return result;
}

int PlatformEntity::React(int entityID, EVENT reactEvent)
{
	switch (reactEvent)
	{
	//case FIELD_CONTAINS:
	//	break;
	//case FIELD_ENTERED:
	//	break;
	//case FIELD_EXITED:
	//	break;
	//case FIELD_CONDITIONS_MET:
	//	break;
	//case FIELD_ENABLED:
	//	break;
	//case FIELD_DISABLED:
	//	break;
	case BUTTON_DEACTIVE:
		this->GetAIComponent()->AC_triggered = false;
		break;
	case BUTTON_ACTIVE:
		this->GetAIComponent()->AC_triggered = true;
		break;
	case LEVER_DEACTIVE:
		this->GetAIComponent()->AC_triggered = false;
		break;
	case LEVER_ACTIVE:
		this->GetAIComponent()->AC_triggered = true;
		break;
	case WHEEL_INCREASING:
		this->GetAIComponent()->AC_triggered = true;
		break;
	case WHEEL_RESET:
		this->GetAIComponent()->AC_triggered = false;
		break;
	default:
		break;
	}
	return 1;
}