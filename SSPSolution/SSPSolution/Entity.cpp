#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

int Entity::SyncComponents()
{
	int result = 1;

	if (this->m_pComp != nullptr)
	{
		if (this->m_aiComp != nullptr)
		{
			// Works for now since we're only handling platforms
			if (this->m_aiComp->AC_triggered)
				this->m_pComp->PC_velocity = DirectX::XMVectorScale(this->m_aiComp->AC_dir, this->m_aiComp->AC_speed);
			else
				this->m_pComp->PC_velocity = { 0 };
			this->m_aiComp->AC_position = this->m_pComp->PC_pos;
			//TODO: test physcomp vs aicomp positions are still updated after they are released from duty
		}
		if (this->m_gComp != nullptr)
		{
			//rotate and translate the obb in the game
			if (this->m_pComp->PC_BVtype == BV_OBB)
			{
				if (this->m_entityID == 1 || this->m_entityID == 2) // 1 or 2 == player
				{
					this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_pComp->PC_OBB.ort, DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));
					this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(0, -this->m_pComp->PC_OBB.ext[1], 0, 0)));
				}
				else
				{
					this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_pComp->PC_OBB.ort, DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));

				}
			}
			else
			{
				this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation), DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));
			}

			result = 1;
		}
		else
		{
			result = -2;
		}
	}
	else
	{
		if (this->m_gComp == nullptr)
		{
			result = -3;
		}
		result = -1;
	}

	return result;
}

int Entity::AddObserver(Observer * observer, int entityID)
{
	this->m_subject.AddObserver(observer, entityID);

	return 0;
}

void Entity::UnsafeSyncComponents()
{
	//rotate and translate the obb in the game
	if (this->m_pComp->PC_BVtype == BV_OBB)
	{
		//this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationQuaternion(this->m_pComp->PC_OBB.quat), DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));
		if (this->m_entityID == 1 || this->m_entityID == 2) // 1 or 2 == player
		{
			this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_pComp->PC_OBB.ort, DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));
			this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_gComp->worldMatrix, DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(0, -this->m_pComp->PC_OBB.ext[1], 0, 0)));
		}
		else
		{
			this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(this->m_pComp->PC_OBB.ort, DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));

		}
	}
	else
	{
		this->m_gComp->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_pComp->PC_rotation), DirectX::XMMatrixTranslationFromVector(this->m_pComp->PC_pos));
	}
}

PhysicsComponent* Entity::SetPhysicsComponent(PhysicsComponent * pComp)
{
	PhysicsComponent* tempReturn = this->m_pComp;
	this->m_pComp = pComp;
	return tempReturn;
}

GraphicsComponent* Entity::SetGraphicsComponent(GraphicsComponent * gComp)
{
	GraphicsComponent* tempReturn = this->m_gComp;
	this->m_gComp = gComp;
	return tempReturn;
}

AIComponent * Entity::SetAIComponent(AIComponent * aiComp)
{
	AIComponent* tempReturn = this->m_aiComp;
	this->m_aiComp = aiComp;
	return tempReturn;
}

AnimationComponent * Entity::SetAnimationComponent(AnimationComponent * aComp)
{
	AnimationComponent* tempReturn = this->m_aComp;
	this->m_aComp = aComp;
	return tempReturn;
}

bool Entity::SetGrabbed(Entity* isGrabbedBy)
{
	bool lastValue = this->m_isGrabbed;;
	this->m_isGrabbedBy = isGrabbedBy;
	
	if (this->m_isGrabbedBy != nullptr)
	{
		this->m_isGrabbed = true;
		//Deactivate the component
		this->m_pComp->PC_active = false;
		this->m_pComp->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);

	}
	else 
	{
		this->m_isGrabbed = false;
		this->m_pComp->PC_active = true;
	}
	
	return lastValue;
}

bool Entity::IsGrabbed()
{
	return this->m_isGrabbed;
}

int Entity::SetEntityID(int entityID)
{
	int lastValue = this->m_entityID;
	this->m_entityID = entityID;
	return lastValue;
}

PhysicsComponent * Entity::GetPhysicsComponent()
{
	return this->m_pComp;
}

GraphicsComponent * Entity::GetGraphicComponent()
{
	return this->m_gComp;
}

AIComponent * Entity::GetAIComponent()
{
	return this->m_aiComp;
}

AnimationComponent * Entity::GetAnimationComponent()
{
	return this->m_aComp;
}

bool Entity::GetGrabbed()
{
	return this->m_isGrabbed;
}

int Entity::GetEntityID()
{
	return this->m_entityID;
}

int Entity::InitializeBase(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, AnimationComponent* aComp, AIComponent* aiComp)
{
	int result = 1;
	this->m_isGrabbed = false;
	this->m_subject = Subject();
	this->m_entityID = entityID;
	this->m_pComp = pComp;
	this->m_gComp = gComp;
	this->m_aComp = aComp;
	this->m_aiComp = aiComp;
	this->m_isGrabbedBy = nullptr;
	return result;
}
