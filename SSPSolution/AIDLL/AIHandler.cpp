#include "AIHandler.h"
#define SUCCESS 1
#define FAIL 0

AIHandler::AIHandler(){}
AIHandler::~AIHandler(){}
int AIHandler::Shutdown()
{
	for (int i = 0; i < m_nrOfAIComponent; i++)
	{
		delete this->m_AIComponents.at(i);
	}

	return SUCCESS;
}

int AIHandler::Initialize(int nrOfAIComponents, int maxAIComponent)
{
	m_nrOfAIComponent = nrOfAIComponents;
	m_maxAIComponent = maxAIComponent;

	for (int i = 0; i < m_nrOfAIComponent; i++)
	{
		m_AIComponents.push_back(CreateAIComponent());
	}

	return SUCCESS;
}
int AIHandler::Update(float dt)
{

	return SUCCESS;
}

AIComponent* AIHandler::CreateAIComponent()
{
	AIComponent* newComponent = nullptr;
	newComponent = new AIComponent;

	newComponent->active = 0;
	newComponent->entityID = -1;

	newComponent->triggered = false;
	newComponent->time = 0;
	newComponent->speed = 0;
	newComponent->direction = 0;
	newComponent->currentWaypoint = 0;
	newComponent->nextWaypoint = 0;

	for (size_t i = 0; i < 8; i++)
	{
		newComponent->waypoints[i] = DirectX::XMVECTOR();
	}

	return newComponent;
}

