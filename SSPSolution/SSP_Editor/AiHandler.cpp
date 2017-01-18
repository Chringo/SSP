#include "AiHandler.h"



AiHandler::AiHandler()
{
}


AiHandler::~AiHandler()
{
}

AIComponent * AiHandler::NewPathComponent()
{
	m_Components.push_back(AIComponent());
	return &m_Components.back();
}

AIComponent * AiHandler::GetPathComponent(int EntityID)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components.at(i).m_entityID == EntityID) {
			return &m_Components.at(EntityID);
		}
	}
	return nullptr;
}

std::vector<AIComponent>* AiHandler::GetAllPathComponents()
{
	return &m_Components;
}

void AiHandler::DeletePathComponent(int EntityID)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components.at(i).m_entityID == EntityID) {
			m_Components.erase(m_Components.begin() + i);
			return;
		}
	}
	

}

void AiHandler::Destroy()
{
	m_Components.clear();
}


