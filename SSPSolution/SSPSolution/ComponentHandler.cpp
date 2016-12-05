#include "ComponentHandler.h"

ComponentHandler::ComponentHandler()
{
}


ComponentHandler::~ComponentHandler()
{
}

int ComponentHandler::Initialize(GraphicsHandler * graphicsHandler)
{
	this->m_graphicsHandler = graphicsHandler;

	return 0;
}

GraphicsComponent * ComponentHandler::GetGraphicsComponent()
{
	return this->m_graphicsHandler->GetNextAvailableComponent();
}

PhysicsComponent * ComponentHandler::GetPhysicsComponent()
{
	return nullptr;
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
