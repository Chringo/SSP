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

AIComponent * AiHandler::NewPathComponent()
{
	m_Components.push_back(new AIComponent());
	return m_Components.back();
}

AIComponent * AiHandler::GetPathComponent(int EntityID)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components.at(i)->AC_entityID == EntityID) {
			return m_Components.at(EntityID);
		}
	}
	return nullptr;
}

std::vector<AIComponent*>* AiHandler::GetAllPathComponents()
{
	return &m_Components;
}

void AiHandler::DeletePathComponent(int EntityID)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components.at(i)->AC_entityID == EntityID) {
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


