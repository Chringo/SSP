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

GraphicsComponent * ComponentHandler::GetStaticGraphicsComponent()
{
	GraphicsComponent* graphicsComponent = nullptr;
	if (this->m_graphicsHandler != nullptr)
	{
		graphicsComponent = this->m_graphicsHandler->GetNextAvailableStaticComponent();
	}
	return graphicsComponent;
}

GraphicsComponent * ComponentHandler::GetDynamicGraphicsComponent()
{
	GraphicsComponent* graphicsComponent = nullptr;
	if (this->m_graphicsHandler != nullptr)
	{
		graphicsComponent = this->m_graphicsHandler->GetNextAvailableDynamicComponent();
	}
	return graphicsComponent;
}

GraphicsComponent * ComponentHandler::GetPersistentGraphicsComponent()
{
	GraphicsComponent* graphicsComponent = nullptr;
	if (this->m_graphicsHandler != nullptr)
	{
		graphicsComponent = this->m_graphicsHandler->GetNextAvailablePersistentComponent();
	}
	return graphicsComponent;
}

//GraphicsComponent * ComponentHandler::GetGraphicsComponent()
//{
//	GraphicsComponent* graphicsComponent = nullptr;
//	if (this->m_graphicsHandler != nullptr)
//	{
//		graphicsComponent = this->m_graphicsHandler->GetNextAvailableComponent();
//	}
//	return graphicsComponent;
//}
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

AnimationComponent * ComponentHandler::GetAnimationComponent()
{
	AnimationComponent* animComp = nullptr;
	if (this->m_aHandler != nullptr)
	{
		animComp = this->m_aHandler->GetNextAvailableComponent();
	}
	return animComp;
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

void ComponentHandler::UpdateSoundHandler()
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

int ComponentHandler::ResizeGraphicsStatic(size_t newCap)
{
	int size = 0;
	size = this->m_graphicsHandler->ResizeStaticComponents(newCap);
	return size;
}

int ComponentHandler::ResizeGraphicsDynamic(size_t newCap)
{
	int size = 0;
	size = this->m_graphicsHandler->ResizeDynamicComponents(newCap);
	return size;
}

int ComponentHandler::ResizeGraphicsPersistent(size_t newCap)
{
	int size = 0;
	size = this->m_graphicsHandler->ResizePersistentComponents(newCap);
	return size;
}

PhysicsHandler * ComponentHandler::GetPhysicsHandler() const
{
	return this->m_physicsHandler;
}

GraphicsHandler * ComponentHandler::GetGraphicsHandler() const
{
	return this->m_graphicsHandler;
}

