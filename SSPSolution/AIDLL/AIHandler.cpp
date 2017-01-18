#include "AIHandler.h"
#define SUCCESS 1
#define FAIL 0

AIHandler::AIHandler(){}
AIHandler::~AIHandler(){}
int AIHandler::Shutdown()
{
	for (int i = 0; i < this->m_maxOfAIComponents; i++)
	{
		delete this->m_AIComponents.at(i);
	}

	return SUCCESS;
}

int AIHandler::Initialize(int max)
{
	this->WaypointUpdated = false;
	this->m_nrOfAIComponents = 0;

	if (max <= 0)
	{
		// Don't allow negative or zero initiated components
		max = 2;
	}
	
	this->m_maxOfAIComponents = max;

	for (int i = 0; i < this->m_maxOfAIComponents; i++)
	{
		this->m_AIComponents.push_back(CreateAIComponent(-1));
	}

	return SUCCESS;
}
int AIHandler::Update(float deltaTime)
{
	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		if (this->m_AIComponents.at(i)->AP_active && this->m_AIComponents.at(i)->AP_triggered)
		{
			// AIComponent logic/behavior, movement of e.g. platforms
			DirectX::XMVECTOR pos = this->m_AIComponents.at(i)->AP_position;
			int currentWaypoint = this->m_AIComponents.at(i)->AP_latestWaypointID;
			int nrOfWaypoint = this->m_AIComponents.at(i)->AP_nrOfWaypoint;
			int pattern = this->m_AIComponents.at(i)->AP_pattern;
			int time = this->m_AIComponents.at(i)->AP_time;
			int direction = this->m_AIComponents.at(i)->AP_direction;

			if (pattern == 1)
			{
				if (currentWaypoint == 0 || currentWaypoint == nrOfWaypoint)
				{
					if (direction == 0)
						this->m_AIComponents.at(i)->AP_direction = 1;
					else
						this->m_AIComponents.at(i)->AP_direction = 0;
				}
			}
			else if (pattern == 3)
			{
				//TODO Round-trip pattern
			}
			else
			{
				//Identical to pattern 2 (Circular)
				if (direction == 0)
				{
					if (WaypointApprox(i))
					{
						currentWaypoint = this->m_AIComponents.at(i)->AP_nextWaypointID;
						this->m_AIComponents.at(i)->AP_nextWaypointID++;
						if (this->m_AIComponents.at(i)->AP_nextWaypointID >= this->m_AIComponents.at(i)->AP_nrOfWaypoint)
							this->m_AIComponents.at(i)->AP_nextWaypointID = 0;
					}
				}
				else
				{
					if (WaypointApprox(i))
					{
						currentWaypoint = this->m_AIComponents.at(i)->AP_nextWaypointID;
						this->m_AIComponents.at(i)->AP_nextWaypointID--;
						if (this->m_AIComponents.at(i)->AP_nextWaypointID <= this->m_AIComponents.at(i)->AP_nrOfWaypoint)
							this->m_AIComponents.at(i)->AP_nextWaypointID = nrOfWaypoint;
					}
				}
			}

			//Update position
			if (this->WaypointUpdated == false)
			{
				this->m_AIComponents.at(i)->AP_dir = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(
					this->m_AIComponents.at(i)->AP_waypoints[this->m_AIComponents.at(i)->AP_nextWaypointID],
					pos));
				this->WaypointUpdated = true;
			}

			DirectX::XMVECTOR v = DirectX::XMVECTOR();
			v = DirectX::XMVectorScale(DirectX::XMVector3Normalize(this->m_AIComponents.at(i)->AP_dir), this->m_AIComponents.at(i)->AP_speed);
			//v = DirectX::XMVectorScale(v, deltaTime);
			
			//this->m_AIComponents.at(i)->AP_position = DirectX::XMVectorMultiply(v, this->m_AIComponents.at(i)->AP_position);
			this->m_AIComponents.at(i)->AP_position = DirectX::XMVectorAdd(v, this->m_AIComponents.at(i)->AP_position);
		}
	}

	return SUCCESS;
}

void AIHandler::SetComponentActive(int compID)
{
	this->m_AIComponents.at(compID)->AP_active = true;
}

void AIHandler::SetComponentFalse(int compID)
{
	this->m_AIComponents.at(compID)->AP_active = false;
}

void AIHandler::SetEntityID(int compID, int entityID)
{
	this->m_AIComponents.at(compID)->AP_entityID = entityID;
}

void AIHandler::SetTriggered(int compID, bool triggered)
{
	this->m_AIComponents.at(compID)->AP_triggered = triggered;
}

void AIHandler::SetTime(int compID, int time)
{
	this->m_AIComponents.at(compID)->AP_time = time;
}

void AIHandler::SetSpeed(int compID, float speed)
{
	this->m_AIComponents.at(compID)->AP_speed = speed;
}

void AIHandler::SetDirection(int compID, int direction)
{
	this->m_AIComponents.at(compID)->AP_direction = direction;
}

void AIHandler::SetCurrentWaypoint(int compID, int latestWaypoint)
{
	this->m_AIComponents.at(compID)->AP_latestWaypointID = latestWaypoint;
}

void AIHandler::SetPattern(int compID, int pattern)
{
	this->m_AIComponents.at(compID)->AP_pattern = pattern;
}

void AIHandler::SetWaypoints(int compID, DirectX::XMVECTOR waypoints[])
{
	for (int i = 0; i < 8; i++)
	{
		this->m_AIComponents.at(compID)->AP_waypoints[i] = waypoints[i];
		this->m_AIComponents.at(compID)->AP_nrOfWaypoint++;
	}
}

int AIHandler::GetNrOfAIComponents() const
{
	return this->m_nrOfAIComponents;
}

DirectX::XMVECTOR AIHandler::GetPosition(int compID) const
{
	return this->m_AIComponents.at(compID)->AP_position;
}

AIComponent * AIHandler::GetNextAvailableComponents()
{
	// Increase vector by initiating new AIComponents
	if (this->m_nrOfAIComponents == this->m_maxOfAIComponents)
	{
		int oldMax = this->m_maxOfAIComponents;
		this->m_maxOfAIComponents += this->m_maxOfAIComponents;
		for (int i = oldMax; i < this->m_maxOfAIComponents; i++)
		{
			this->m_AIComponents.push_back(CreateAIComponent(i));
		}
	}
	this->m_nrOfAIComponents++;
	return this->m_AIComponents[this->m_nrOfAIComponents - 1];
}

AIComponent* AIHandler::CreateAIComponent(int entityID)
{
	AIComponent* newComponent = nullptr;
	newComponent = new AIComponent;

	newComponent->AP_active = 0;
	newComponent->AP_entityID = entityID;
	newComponent->AP_position = DirectX::XMVECTOR();

	newComponent->AP_triggered = false;
	newComponent->AP_pattern = 0;
	newComponent->AP_time = 0;
	newComponent->AP_speed = 0;
	newComponent->AP_direction = 0;
	newComponent->AP_nextWaypointID = 0;
	newComponent->AP_latestWaypointID = 0;
	newComponent->AP_nrOfWaypoint = 0;

	for (int i = 0; i < 8; i++) 
	{
		newComponent->AP_waypoints[i] = DirectX::XMVECTOR();
	}

	return newComponent;
}

bool AIHandler::WaypointApprox(int compID)
{
	int current = this->m_AIComponents.at(compID)->AP_latestWaypointID;
	int next = this->m_AIComponents.at(compID)->AP_nextWaypointID;

	DirectX::XMVECTOR v = DirectX::XMVectorSubtract(this->m_AIComponents.at(compID)->AP_waypoints[next]
		,this->m_AIComponents.at(compID)->AP_position);

	float length = VectorLength(v);

	if (length < 0.1f)
	{	
		this->WaypointUpdated = false;
		return true;
	}

	return false;
}

int AIHandler::GetNextWaypoint(int compID, int pattern)
{
	int next = this->m_AIComponents.at(compID)->AP_latestWaypointID;
	int current = this->m_AIComponents.at(compID)->AP_nextWaypointID;

	if (pattern == 1)
	{
		//TODO Linear pattern next waypoint logic
	}
	else
	{

	}

	if (next == current)
	{

	}

	this->m_AIComponents.at(compID)->AP_latestWaypointID;
	this->m_AIComponents.at(compID)->AP_direction;

	return 0;
}

float AIHandler::VectorLength(DirectX::XMVECTOR v)
{
	float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(v));
	return length;
}
