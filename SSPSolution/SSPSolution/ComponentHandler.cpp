#include "ComponentHandler.h"

ComponentHandler::ComponentHandler()
{
}


ComponentHandler::~ComponentHandler()
{
}

int ComponentHandler::Initialize(GraphicsHandler * graphicsHandler, AnimationHandler* animationHandler, PhysicsHandler* physicsHandler, AIHandler* aiHandler)
{
	int result = 1;
	this->m_graphicsHandler = graphicsHandler;
	this->m_physicsHandler = physicsHandler;
	this->m_aiHandler = aiHandler;
	if (graphicsHandler == nullptr || animationHandler == nullptr || physicsHandler == nullptr || aiHandler == nullptr)
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

AnimationComponent * ComponentHandler::GetAnimationComponent()
{
	AnimationComponent* animationComponent = nullptr;
	if (this->m_AnimationHandler != nullptr)
	{
		/*I need to figure out how to store my components, because they are not 
		like any other components in list structure. They need to be in a kind of 2D-array fashion.*/
	}
	return animationComponent;
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

void ComponentHandler::UpdateGraphicsComponents()
{
	this->m_graphicsHandler->UpdateComponentList();
}

void ComponentHandler::UpdateAIComponents()
{

}

void ComponentHandler::SetGraphicsComponentListSize(int gCompSize)
{
	this->m_graphicsHandler->SetComponentArraySize(gCompSize);
	return;
}

PhysicsHandler * ComponentHandler::GetPhysicsHandler() const
{
	return this->m_physicsHandler;
}

