#include "ComponentHandler.h"

ComponentHandler::ComponentHandler()
{	
}


ComponentHandler::~ComponentHandler()
{
}

int ComponentHandler::Initialize(GraphicsHandler * graphicsHandler, PhysicsHandler* physicsHandler, AIHandler* aiHandler, SoundHandler* soundHandler)
{
	int result = 1;
	this->m_graphicsHandler = graphicsHandler;
	this->m_physicsHandler = physicsHandler;
	this->m_aiHandler = aiHandler;
	this->m_soundHandler = soundHandler;
	if (graphicsHandler == nullptr || physicsHandler == nullptr || aiHandler == nullptr || soundHandler == nullptr)
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


UIComponent* ComponentHandler::GetUIComponent()
{
	UIComponent* uiComponent = nullptr;
	if (this->m_graphicsHandler != nullptr)
	{
		uiComponent = this->m_graphicsHandler->GetNextAvailableUIComponent();
	}
	return uiComponent;
}

TextComponent * ComponentHandler::GetTextComponent()
{
	TextComponent* textComponent = nullptr;
	if (this->m_graphicsHandler != nullptr)
	{
		textComponent = this->m_graphicsHandler->GetNextAvailableTextComponent();
	}
	return textComponent;
}

AIComponent * ComponentHandler::GetAIComponent()
{
	AIComponent* newComp = nullptr;
	if (this->m_aiHandler != nullptr)
	{
		newComp = this->m_aiHandler->GetNextAvailableComponents();
		newComp->AC_active = 1;
	}
	return newComp;
}

SoundComponent2D * ComponentHandler::GetSoundComponent2D()
{
	return this->m_soundHandler->GetSoundComponent2D();
}

SoundComponent3D * ComponentHandler::GetSoundComponent3D()
{
	return this->m_soundHandler->GetSoundComponent3D();
}

void ComponentHandler::UpdateGraphicsComponents()
{
	this->m_graphicsHandler->UpdateComponentList();
}

void ComponentHandler::UpdateAIComponents()
{
	this->m_soundHandler->UpdateSoundHandler();
}

void ComponentHandler::UpdateSoundHandler()
{
}

void ComponentHandler::SetGraphicsComponentListSize(int gCompSize)
{
	this->m_graphicsHandler->SetComponentArraySize(gCompSize);
	return;
}

PhysicsComponent * ComponentHandler::GetClosestPhysicsComponent(PhysicsComponent * component, int minDistance)
{
	return this->m_physicsHandler->GetClosestComponent(component, minDistance);
}

PhysicsHandler * ComponentHandler::GetPhysicsHandler() const
{
	return this->m_physicsHandler;
}

