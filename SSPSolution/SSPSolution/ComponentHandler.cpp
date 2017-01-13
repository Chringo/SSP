#include "ComponentHandler.h"

ComponentHandler::ComponentHandler()
{
}


ComponentHandler::~ComponentHandler()
{
}

int ComponentHandler::Initialize(GraphicsHandler * graphicsHandler, PhysicsHandler* physicsHandler)
{
	int result = 1;
	this->m_graphicsHandler = graphicsHandler;
	this->m_physicsHandler = physicsHandler;
	if (graphicsHandler == nullptr || physicsHandler == nullptr)
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

PhysicsComponent * ComponentHandler::GetPhysicsComponent()
{
	PhysicsComponent* newComponent = nullptr;
	DirectX::XMVECTOR tempPos = DirectX::XMVectorSet(0, 8, 60, 0);
	newComponent = this->m_physicsHandler->CreatePhysicsComponent(tempPos, false);
	return newComponent;
}

void ComponentHandler::UpdateGraphicsComponents()
{
	this->m_graphicsHandler->UpdateComponentList();
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

