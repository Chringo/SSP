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
	this->m_nrOfAIComponents = 0;

	if (nrOfAIComponents < 0)
		return FAIL;
	
	this->m_nrOfAIComponents = nrOfAIComponents;

	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		m_AIComponents.push_back(CreateAIComponent(i));
	}

	return SUCCESS;
}
int AIHandler::Update(float deltaTime)
{
	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		if (this->m_AIComponents.at(i)->m_active && this->m_AIComponents.at(i)->m_triggered)
		{
			// AIComponent logic/behavior
			// movement of e.g. platforms
			int currentWaypoint = this->m_AIComponents.at(i)->m_currentWaypoint;
			int nrOfWaypoint = this->m_AIComponents.at(i)->m_nrOfWaypoint;
			int pattern = this->m_AIComponents.at(i)->m_pattern;
			int time = this->m_AIComponents.at(i)->m_time;
			int direction = this->m_AIComponents.at(i)->m_direction;

			if (pattern == 1)
			{
				if (currentWaypoint == 0 || currentWaypoint == nrOfWaypoint)
				{
					if (direction == 0)
						this->m_AIComponents.at(i)->m_direction = 1;
					else
						this->m_AIComponents.at(i)->m_direction = 0;
				}
			}
			else if (pattern == 3)
			{
				//TODO Random pattern
			}
			else
			{
				//Identical to pattern 2 (Circular)
				if (direction == 0)
				{
					using namespace DirectX;

					this->m_AIComponents.at(i)->m_waypoints[i] - this->m_AIComponents.at(i)->m_waypoints[i];
					DirectX::XMVECTOR x;

					//Should be speed dependent
					currentWaypoint;
					this->pos;
				}
				else
				{
					this->pos;
				}
			}
		}
	}

	return SUCCESS;
}

void AIHandler::SetComponentActive(int compID)
{
	this->m_AIComponents.at(compID)->m_active = true;
}

void AIHandler::SetComponentFalse(int compID)
{
	this->m_AIComponents.at(compID)->m_active = false;
}

void AIHandler::SetEntityID(int compID, int entityID)
{
	this->m_AIComponents.at(compID)->m_entityID = entityID;
}

void AIHandler::SetTriggered(int compID, bool triggered)
{
	this->m_AIComponents.at(compID)->m_triggered = triggered;
}

void AIHandler::SetTime(int compID, int time)
{
	this->m_AIComponents.at(compID)->m_time = time;
}

void AIHandler::SetSpeed(int compID, int speed)
{
	this->m_AIComponents.at(compID)->m_speed = speed;
}

void AIHandler::SetDirection(int compID, int direction)
{
	this->m_AIComponents.at(compID)->m_direction = direction;
}

void AIHandler::SetCurrentWaypoint(int compID, int currentWaypoint)
{
	this->m_AIComponents.at(compID)->m_currentWaypoint = currentWaypoint;
}

AIDLL_API void AIHandler::SetPattern(int compID, int pattern)
{
	this->m_AIComponents.at(compID)->m_pattern = pattern;
}

void AIHandler::SetWaypoints(int compID, DirectX::XMVECTOR waypoints[])
{
	for (int i = 0; i < 8; i++)
	{
		this->m_AIComponents.at(compID)->m_waypoints[i] = waypoints[i];
		this->m_AIComponents.at(compID)->m_nrOfWaypoint++;
	}
}

int AIHandler::GetNrOfAIComponents() const
{
	return this->m_nrOfAIComponents;
}

AIDLL_API DirectX::XMVECTOR AIHandler::GetPosition(int compID) const
{
	return this->m_AIComponents.at(compID)->m_position;
}

AIComponent* AIHandler::CreateAIComponent(int entityID)
{
	AIComponent* newComponent = nullptr;
	newComponent = new AIComponent;

	newComponent->m_active = 0;
	newComponent->m_entityID = entityID;
	newComponent->m_position = DirectX::XMVECTOR();

	newComponent->m_triggered = false;
	newComponent->m_time = 0;
	newComponent->m_speed = 0;
	newComponent->m_direction = 0;
	newComponent->m_currentWaypoint = 0;
	newComponent->m_nrOfWaypoint = 0;

	for (int i = 0; i < 8; i++) {
		newComponent->m_waypoints[i] = DirectX::XMVECTOR();
		newComponent->m_nrOfWaypoint++;
	}

	return newComponent;
}
