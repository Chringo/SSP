#include "AiHandler.h"



AiHandler::AiHandler()
{
}


AiHandler::~AiHandler()
{
}

AIComponent * AiHandler::NewPathComponent()
{
	m_PathComponents.push_back(AIComponent());
	return &m_PathComponents.back();
}

AIComponent * AiHandler::GetPathComponent(int EntityID)
{
	for (size_t i = 0; i < m_PathComponents.size(); i++)
	{
		if (m_PathComponents.at(i).m_entityID == EntityID) {
			return &m_PathComponents.at(EntityID);
		}
	}
	return nullptr;
}

std::vector<AIComponent>* AiHandler::GetAllPathComponents()
{
	return &m_PathComponents;
}

void AiHandler::DeletePathComponent(int EntityID)
{
	for (size_t i = 0; i < m_PathComponents.size(); i++)
	{
		if (m_PathComponents.at(i).m_entityID == EntityID) {
			m_PathComponents.erase(m_PathComponents.begin() + i);
			return;
		}
	}
	

}

void AiHandler::Destroy()
{
	m_PathComponents.clear();
}


