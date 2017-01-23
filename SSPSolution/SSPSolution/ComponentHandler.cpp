#include "ComponentHandler.h"

ComponentHandler::ComponentHandler()
{
}


ComponentHandler::~ComponentHandler()
{
}

int ComponentHandler::Initialize(GraphicsHandler * graphicsHandler, PhysicsHandler* physicsHandler, AIHandler* aiHandler, AnimationHandler* aHandler)
{
	int result = 1;
	this->m_graphicsHandler = graphicsHandler;
	this->m_physicsHandler = physicsHandler;
	this->m_aiHandler = aiHandler;
	this->m_aHandler = aHandler;
	if (graphicsHandler == nullptr || physicsHandler == nullptr || aiHandler == nullptr || aHandler == nullptr)
		result = 0;
	return result;
}

GraphicsComponent * ComponentHandler::GetGraphicsComponent()
{
	GraphicsComponent* graphicsComponent = nullptr;
	if (this->m_graphicsHandler != nullptr)
	{
		graphicsComponent = this->m_graphicsHandler->GetNextAvailableComponent();
	}
	return graphicsComponent;
}
GraphicsAnimationComponent * ComponentHandler::GetGraphicsAnimationComponent()
{
	GraphicsAnimationComponent * graphicsAnimComponent = nullptr;
	if (this->m_graphicsHandler != nullptr)
	{
		graphicsAnimComponent = this->m_graphicsHandler->GetNextAvailableAnimationComponent();
	}
	return graphicsAnimComponent;
}

PhysicsComponent * ComponentHandler::GetPhysicsComponent()
{
	PhysicsComponent* newComponent = nullptr;
	DirectX::XMVECTOR tempPos = DirectX::XMVectorSet(0, 8, 60, 0);
	newComponent = this->m_physicsHandler->CreatePhysicsComponent(tempPos, false);
	return newComponent;
}

AIComponent * ComponentHandler::GetAIComponent()
{
	AIComponent* newComp = nullptr;
	if (this->m_aiHandler != nullptr)
	{
		newComp = this->m_aiHandler->GetNextAvailableComponents();
	}
	return newComp;
}

AnimationComponent * ComponentHandler::GetAnimationComponent()
{
	AnimationComponent* animationComponent = nullptr;

	if (this->m_aHandler != nullptr)
	{
		animationComponent = this->m_aHandler->GetNextAvailableComponent();
	}

	return animationComponent;
}

void ComponentHandler::UpdateGraphicsComponents()
{
	this->m_graphicsHandler->UpdateComponentList();
}

void ComponentHandler::UpdateGraphicsAnimationComponents()
{
	this->m_graphicsHandler->UpdateAnimComponentList();
}

void ComponentHandler::UpdateAIComponents()
{

}

void ComponentHandler::SetGraphicsComponentListSize(int gCompSize)
{
	this->m_graphicsHandler->SetComponentArraySize(gCompSize);
	return;
}

void ComponentHandler::SetGraphicsAnimationComponentListSize(int gCompSize)
{
	this->m_graphicsHandler->SetAnimComponentArraySize(gCompSize);
	return;
}

PhysicsHandler * ComponentHandler::GetPhysicsHandler() const
{
	return this->m_physicsHandler;
}

