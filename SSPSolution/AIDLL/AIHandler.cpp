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
			// AIComponent logic/behavior, movement of e.g. platforms
			DirectX::XMVECTOR pos = this->m_AIComponents.at(i)->m_position;
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
				//TODO Round-trip pattern
			}
			else
			{
				//Identical to pattern 2 (Circular)
				if (direction == 0)
				{
					if (WaypointApprox(i))
					{
						currentWaypoint = this->m_AIComponents.at(i)->m_nextWaypoint;
						this->m_AIComponents.at(i)->m_nextWaypoint++;
						if (this->m_AIComponents.at(i)->m_nextWaypoint >= this->m_AIComponents.at(i)->m_nrOfWaypoint)
							this->m_AIComponents.at(i)->m_nextWaypoint = 0;
					}
				}
				else
				{
					if (WaypointApprox(i))
					{
						currentWaypoint = this->m_AIComponents.at(i)->m_nextWaypoint;
						this->m_AIComponents.at(i)->m_nextWaypoint--;
						if (this->m_AIComponents.at(i)->m_nextWaypoint <= this->m_AIComponents.at(i)->m_nrOfWaypoint)
							this->m_AIComponents.at(i)->m_nextWaypoint = nrOfWaypoint;
					}
				}
			}

			//Update position
			DirectX::XMVECTOR v;
			v = DirectX::XMVectorSubtract(
				this->m_AIComponents.at(i)->m_waypoints[this->m_AIComponents.at(i)->m_nextWaypoint],
				pos);

			DirectX::XMVECTOR m;
			m = DirectX::XMVectorScale(DirectX::XMVector3Normalize(v), 10); //Speed?
			m = DirectX::XMVectorScale(m, deltaTime);
			
			this->m_AIComponents.at(i)->m_position = DirectX::XMVectorMultiply(m, this->m_AIComponents.at(i)->m_position);
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

void AIHandler::SetPattern(int compID, int pattern)
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

DirectX::XMVECTOR AIHandler::GetPosition(int compID) const
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

bool AIHandler::WaypointApprox(int compID)
{
	using namespace DirectX;

	int next = this->m_AIComponents.at(compID)->m_currentWaypoint;
	int current = this->m_AIComponents.at(compID)->m_nextWaypoint;

	DirectX::XMVECTOR v = DirectX::XMVectorSubtract(this->m_AIComponents.at(compID)->m_waypoints[next]
		,this->m_AIComponents.at(compID)->m_waypoints[current]);

	float length = VectorLength(v);

	if (length > 0.1)
	{	
		return true;
	}

	return false;
}

int AIHandler::GetNextWaypoint(int compID, int pattern)
{
	/*const int ARR_LEN = 10;

	int arr[ARR_LEN] = { 0,1,2,3,4,5,6,7,8,9 };

	for (int i = 0; i < ARR_LEN * 2; ++i)
	cout << arr[i % ARR_LEN] << " ";*/

	int next = this->m_AIComponents.at(compID)->m_currentWaypoint;
	int current = this->m_AIComponents.at(compID)->m_nextWaypoint;

	if (pattern == 1)
	{
		//TODO Linear pattern next waypoint logic
	}
	else
	{
		//if (this->m_AIComponents.at(compID)->m_nrOfWaypoint)
	}

	if (next == current)
	{

	}

	this->m_AIComponents.at(compID)->m_currentWaypoint;
	this->m_AIComponents.at(compID)->m_direction;

	//this->m_AIComponents.at(compID)->m_waypoints[i];

	return 0;
}

float AIHandler::VectorLength(DirectX::XMVECTOR v)
{
	float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(v));
	return length;
}
