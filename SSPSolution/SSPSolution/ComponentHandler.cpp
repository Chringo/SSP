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
	bool gotComponent = false;
	PhysicsComponent* newComponent = nullptr;
	for (int i = 0; i < this->m_physicsHandler->getNrOfComponents() && !gotComponent; i++)
	{
		newComponent = this->m_physicsHandler->getDynamicComponents(i);
		if (!newComponent->PC_active)
		{
			gotComponent = true;
		}
	}
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
