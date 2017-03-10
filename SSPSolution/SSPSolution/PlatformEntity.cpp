#include "PlatformEntity.h"
PlatformEntity::PlatformEntity() {}
PlatformEntity::~PlatformEntity()
{
	this->Shutdown();
}
int PlatformEntity::Initialize(unsigned int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, AIComponent * aiComp)
{
	int result = 0;
	this->InitializeBase(entityID, pComp, gComp, nullptr, aiComp);
	return result;
}
int PlatformEntity::Shutdown()
{
	int result = 0;
	if (this->m_ActiveSound != nullptr)
	{
		this->m_ActiveSound->stop();
		this->m_ActiveSound->drop();
	}
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

	if (this->GetAIComponent()->AC_triggered && !this->GetAIComponent()->AC_finished)
	{
		if (this->m_ActiveSound == nullptr)
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, this->GetPhysicsComponent()->PC_pos);
			this->m_ActiveSound = SoundHandler::instance().PlaySound3D(Sounds3D::GENERAL_LIFT, pos, true, true);
		}
		/*leaving this code outcommented here in case everything breaks for some reason*/
		//if (this->GetAIComponent()->AC_finished)
		//{
		//	this->m_ActiveSound->setPlayPosition(0);
		//	this->m_ActiveSound->setIsPaused(true);	//Pause the walking sound
		//}
		else
		{
			if (this->m_ActiveSound->getIsPaused())
			{
				this->m_ActiveSound->setIsPaused(false);
			}
			/*update the position of the platform sound*/
			irrklang::vec3df newPos(
				this->GetPhysicsComponent()->PC_pos.m128_f32[0],
				this->GetPhysicsComponent()->PC_pos.m128_f32[1],
				this->GetPhysicsComponent()->PC_pos.m128_f32[2]);

			this->m_ActiveSound->setPosition(newPos);
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

int PlatformEntity::React(unsigned int entityID, EVENT reactEvent)
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
		this->GetAIComponent()->AC_triggered = !this->GetAIComponent()->AC_triggered;
		break;
	case LEVER_DEACTIVE:
		this->GetAIComponent()->AC_triggered = !this->GetAIComponent()->AC_triggered;
		this->GetAIComponent()->AC_oldReset = this->GetAIComponent()->AC_reset;
		this->GetAIComponent()->AC_reset = true;
		break;
	case LEVER_ACTIVE:		
		this->GetAIComponent()->AC_triggered = true;
		this->GetAIComponent()->AC_oldReset = this->GetAIComponent()->AC_reset;
		this->GetAIComponent()->AC_reset = false;
		break;
	case WHEEL_INCREASING:
		//printf("INCREASING\n");
		this->GetAIComponent()->AC_triggered = true;
		this->GetAIComponent()->AC_oldReset = this->GetAIComponent()->AC_reset;
		this->GetAIComponent()->AC_reset = false;
		break;
	case WHEEL_DECREASING:
		//printf("DECREASING\n");
		break;
	case WHEEL_RESET:
		//printf("RESET\n");
		this->GetAIComponent()->AC_triggered = true;
		this->GetAIComponent()->AC_oldReset = this->GetAIComponent()->AC_reset;
		this->GetAIComponent()->AC_reset = true;

		break;
	default:
		break;
	}
	return 1;
}