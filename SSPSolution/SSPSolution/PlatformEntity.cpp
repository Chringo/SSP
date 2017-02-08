#include "PlatformEntity.h"
PlatformEntity::PlatformEntity(){}
PlatformEntity::~PlatformEntity(){}

int PlatformEntity::Update(float deltaTime, InputHandler * inputHandler)
{
	this->SyncComponents();
	//this->m_gComp->worldMatrix = DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos);
	//if (m_entityID == 922)
	//{
	this->m_gComp->worldMatrix =
		DirectX::XMMatrixMultiply(
			DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation),
			DirectX::XMMatrixTranslationFromVector(
				DirectX::XMVectorSubtract(this->m_pComp->PC_pos,
					DirectX::XMVECTOR{
		m_gComp->modelPtr->GetOBBData().position.x,
			m_gComp->modelPtr->GetOBBData().position.y,
			m_gComp->modelPtr->GetOBBData().position.z, 0})));
	//this->m_gComp->worldMatrix =
	//	DirectX::XMMatrixMultiply(
	//		DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation),
	//		DirectX::XMMatrixTranslationFromVector(
	//			DirectX::XMVectorSubtract(this->m_pComp->PC_pos,
	//				this->m_pComp->PC_OBB.ort.r[3])));
	//m_gComp->modelPtr->GetOBBData().position
	//}
	//else
	//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation), DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));

	return 1;
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
	//case LEVER_DEACTIVE:
	//	break;
	//case LEVER_ACTIVE:
	//	break;
	//case LEVER_ENABLED:
	//	break;
	//case LEVER_DISABLED:
	//	break;
	default:
		break;
	}
	return 1;
}

int PlatformEntity::Initialize(int entityID, PhysicsComponent * pComp, GraphicsComponent * gComp, AIComponent * aiComp)
{
	this->InitializeBase(entityID, pComp, gComp, nullptr, aiComp);
	return 1;
}
