#include "AiHandler.h"



AiHandler::AiHandler()
{
}


AiHandler::~AiHandler()
{
}

AIComponent * AiHandler::NewComponent()
{
	m_components.push_back(AIComponent());
	return &m_components.back();
}

AIComponent * AiHandler::GetComponent(int EntityID)
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		if (m_components.at(i).m_entityID == EntityID) {
			return &m_components.at(EntityID);
		}
	}
	return nullptr;
}

std::vector<AIComponent>* AiHandler::GetAllComponents()
{
	return &m_components;
}

void AiHandler::DeleteComponent(int EntityID)
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		if (m_components.at(i).m_entityID == EntityID) {
			m_components.erase(m_components.begin() + i);
			return;
		}
	}
}

void AiHandler::Destroy()
{
	m_components.clear();
}


