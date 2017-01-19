#include "AIHandler.h"
#define SUCCESS 1
#define FAIL 0

AIHandler::AIHandler() {}
AIHandler::~AIHandler() {}
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
	//float dt = 0;
	////if (deltaTime < 0.000001f)
	////	dt = 0.00001f;
	////else
	////	dt = (deltaTime * 0.00001f);
	//dt = 1000000 / deltaTime;
	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		if (this->m_AIComponents.at(i)->AC_active && this->m_AIComponents.at(i)->AC_triggered)
		{
			// AIComponent logic/behavior, movement of e.g. platforms
			DirectX::XMVECTOR pos = this->m_AIComponents.at(i)->AC_position;
			int currentWaypoint = this->m_AIComponents.at(i)->AC_latestWaypointID;
			int nrOfWaypoint = this->m_AIComponents.at(i)->AC_nrOfWaypoint;
			int pattern = this->m_AIComponents.at(i)->AC_pattern;
			int time = this->m_AIComponents.at(i)->AC_time;
			int direction = this->m_AIComponents.at(i)->AC_direction;
			int x = this->m_AIComponents.at(i)->AC_nextWaypointID;

			if (pattern == AI_ONEWAY)
			{
				//default direction
				if (direction == 0)
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f))
					{
						this->m_AIComponents.at(i)->AC_nextWaypointID++;
						this->m_AIComponents.at(i)->AC_direction = 1;
						//this->m_AIComponents.at(i)->AC_triggered = false;
					}
				}
				else
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f))
					{
						this->m_AIComponents.at(i)->AC_nextWaypointID--;
						this->m_AIComponents.at(i)->AC_direction = 0;
						//this->m_AIComponents.at(i)->AC_triggered = false;
					}
				}
			}
			else if (pattern == AI_ROUNDTRIP)
			{
				//TODO Round-trip pattern
				if (currentWaypoint == 0 || currentWaypoint == nrOfWaypoint)
				{
					if (direction == 0)
						this->m_AIComponents.at(i)->AC_direction = 1;
					else
						this->m_AIComponents.at(i)->AC_direction = 0;
				}
			}
			else
			{
				//Circular
				if (direction == 0)
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f))
					{
						//this->m_AIComponents.at(i)->AC_position = this->m_AIComponents.at(i)->AC_waypoints[currentWaypoint];
						currentWaypoint = this->m_AIComponents.at(i)->AC_nextWaypointID;
						this->m_AIComponents.at(i)->AC_nextWaypointID++;
						if (this->m_AIComponents.at(i)->AC_nextWaypointID >= this->m_AIComponents.at(i)->AC_nrOfWaypoint)
							this->m_AIComponents.at(i)->AC_nextWaypointID = 0;
					}
				}
				else
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f))
					{
						currentWaypoint = this->m_AIComponents.at(i)->AC_nextWaypointID;
						this->m_AIComponents.at(i)->AC_nextWaypointID--;
						if (this->m_AIComponents.at(i)->AC_nextWaypointID <= this->m_AIComponents.at(i)->AC_nrOfWaypoint)
							this->m_AIComponents.at(i)->AC_nextWaypointID = nrOfWaypoint;
					}
				}
			}

			//Update position
			if (this->WaypointUpdated == false)
			{
				this->m_AIComponents.at(i)->AC_dir = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(
					this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
					pos));
				this->WaypointUpdated = true;
			}

			DirectX::XMVECTOR v = DirectX::XMVECTOR();
			v = DirectX::XMVectorScale(DirectX::XMVector3Normalize(this->m_AIComponents.at(i)->AC_dir), this->m_AIComponents.at(i)->AC_speed);
			//if (dt < 144.0f)
			//{
			//	v = DirectX::XMVectorScale(v, dt);
			//}

			this->m_AIComponents.at(i)->AC_position = DirectX::XMVectorAdd(v, this->m_AIComponents.at(i)->AC_position);
		}
	}

	return SUCCESS;
}

void AIHandler::SetComponentActive(int compID)
{
	this->m_AIComponents.at(compID)->AC_active = true;
}

void AIHandler::SetComponentFalse(int compID)
{
	this->m_AIComponents.at(compID)->AC_active = false;
}

void AIHandler::SetEntityID(int compID, int entityID)
{
	this->m_AIComponents.at(compID)->AC_entityID = entityID;
}

void AIHandler::SetTriggered(int compID, bool triggered)
{
	this->m_AIComponents.at(compID)->AC_triggered = triggered;
}

void AIHandler::SetTime(int compID, int time)
{
	this->m_AIComponents.at(compID)->AC_time = time;
}

void AIHandler::SetSpeed(int compID, float speed)
{
	this->m_AIComponents.at(compID)->AC_speed = speed;
}

void AIHandler::SetDirection(int compID, int direction)
{
	this->m_AIComponents.at(compID)->AC_direction = direction;
}

void AIHandler::SetLatestWaypoint(int compID, int latestWaypoint)
{
	this->m_AIComponents.at(compID)->AC_latestWaypointID = latestWaypoint;
}

void AIHandler::SetPattern(int compID, int pattern)
{
	this->m_AIComponents.at(compID)->AC_pattern = pattern;
}

void AIHandler::SetWaypoints(int compID, DirectX::XMVECTOR waypoints[])
{
	for (int i = 0; i < 8; i++)
	{
		this->m_AIComponents.at(compID)->AC_waypoints[i] = waypoints[i];
		this->m_AIComponents.at(compID)->AC_nrOfWaypoint++;
	}
}

int AIHandler::GetNrOfAIComponents() const
{
	return this->m_nrOfAIComponents;
}

DirectX::XMVECTOR AIHandler::GetPosition(int compID) const
{
	return this->m_AIComponents.at(compID)->AC_position;
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

	newComponent->AC_entityID = entityID;

	for (int i = 0; i < 8; i++) 
	{
		newComponent->AC_waypoints[i] = DirectX::XMVECTOR();
	}

	return newComponent;
}

bool AIHandler::WaypointApprox(DirectX::XMVECTOR c1, DirectX::XMVECTOR c2, float distance)
{
	//Approximation if position of the platform is the same as the nextWaypoint to see if it reached its goal.
	float dx = abs(DirectX::XMVectorGetX(c2) - DirectX::XMVectorGetX(c1));
	float dy = abs(DirectX::XMVectorGetY(c2) - DirectX::XMVectorGetY(c1));
	float dz = abs(DirectX::XMVectorGetZ(c2) - DirectX::XMVectorGetZ(c1));

	if (dx > distance) return false; // too far in x direction
	if (dy > distance) return false; // too far in y direction
	if (dz > distance) return false; // too far in z direction

	this->WaypointUpdated = false;

	return true;
}

int AIHandler::GetNextWaypoint(int compID, int pattern)
{
	//TODO Move some logic from update if needed to this function
	int next = this->m_AIComponents.at(compID)->AC_latestWaypointID;
	int current = this->m_AIComponents.at(compID)->AC_nextWaypointID;

	if (pattern == AI_ONEWAY)
	{
		//TODO ONEWAY pattern next waypoint logic
	}
	else
	{

	}

	if (next == current)
	{

	}

	this->m_AIComponents.at(compID)->AC_latestWaypointID;
	this->m_AIComponents.at(compID)->AC_direction;

	return 0;
}
