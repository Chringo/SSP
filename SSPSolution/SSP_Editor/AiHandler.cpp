#include "AiHandler.h"



AiHandler::AiHandler()
{
}


AiHandler::~AiHandler()
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		delete m_Components.at(i);
	}
}

AiContainer * AiHandler::NewPathComponent()
{
	m_Components.push_back(new AiContainer());
	m_Components.back()->aiComponent.AC_pattern = Pattern::AI_ONEWAY;
	return m_Components.back();
}

AiContainer * AiHandler::GetPathComponent(int EntityID)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components.at(i)->aiComponent.AC_entityID == EntityID) {
			return m_Components.at(EntityID);
		}
	}
	return nullptr;
}

std::vector<AiContainer*>* AiHandler::GetAllPathComponents()
{
	return &m_Components;
}

void AiHandler::DeletePathComponent(int EntityID)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components.at(i)->aiComponent.AC_entityID == EntityID) {
			delete m_Components.at(i);
			m_Components.erase(m_Components.begin() + i);
			return;
		}
	}
	

}

void AiHandler::Destroy()
{
	m_Components.clear();
}


