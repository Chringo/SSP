#include "AIHandler.h"
#define SUCCESS 1
#define FAIL 0

AIHandler::AIHandler(){}
AIHandler::~AIHandler(){}
int AIHandler::Shutdown()
{
	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		delete this->m_AIComponents.at(i);
	}

	return SUCCESS;
}

int AIHandler::Initialize(int nrOfAIComponents)
{
	if (this->m_nrOfAIComponents < 0)
		return FAIL;
	
	m_nrOfAIComponents = nrOfAIComponents;

	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		m_AIComponents.push_back(CreateAIComponent(i));
	}

	return SUCCESS;
}
int AIHandler::Update(float dt)
{
	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		if (this->m_AIComponents.at(i)->m_active && this->m_AIComponents.at(i)->m_triggered)
		{
			// AIComponent logic/behavior
			// movement of e.g. platforms
		}
	}

	return SUCCESS;
}

int AIHandler::getNrOfAIComponents() const
{
	return this->m_nrOfAIComponents;
}

AIComponent* AIHandler::CreateAIComponent(int entityID)
{
	AIComponent* newComponent = nullptr;
	newComponent = new AIComponent;

	newComponent->m_active = 0;
	newComponent->m_entityID = entityID;

	newComponent->m_triggered = false;
	newComponent->m_time = 0;
	newComponent->m_speed = 0;
	newComponent->m_direction = 0;
	newComponent->m_currentWaypoint = 0;
	newComponent->m_nextWaypoint = 0;

	for (size_t i = 0; i < 8; i++)
	{
		newComponent->m_waypoints[i] = DirectX::XMVECTOR();
	}

	return newComponent;
}

