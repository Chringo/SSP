#include "AIHandler.h"
#define SUCCESS 1
#define FAIL 0

AIHandler::AIHandler() {}
AIHandler::~AIHandler() {}
int AIHandler::Shutdown()
{
	for (int i = 0; i < this->m_maxOfAIComponents; i++)
	{
		delete this->m_AIComponents[i];
	}

	return SUCCESS;
}

int AIHandler::Initialize(int max)
{
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

	float dt = deltaTime;

	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		if (this->m_AIComponents[i] == nullptr)
			break;
		//TODO: Remove active from this statement
		if (this->m_AIComponents[i]->AC_active == 1 && this->m_AIComponents[i]->AC_triggered && this->m_AIComponents[i]->AC_nrOfWaypoint != 0)
		{
			// AIComponent logic/behavior, movement of e.g. platforms
			if (this->m_AIComponents[i]->AC_pattern == AI_ONEWAY)
			{
				//One-way pattern
				//default direction 0 assumed in description.
				/*This pattern will move the platform from 0 to AC_nrOfWaypoint then stop*/
				if (this->m_AIComponents[i]->AC_direction == 0)
				{
					if (WaypointApprox(
						this->m_AIComponents[i]->AC_position,
						this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
						0.05f, i))
					{
						this->m_AIComponents[i]->AC_latestWaypointID = this->m_AIComponents[i]->AC_nextWaypointID;

						if (this->m_AIComponents[i]->AC_nextWaypointID + 1 < this->m_AIComponents[i]->AC_nrOfWaypoint)
							this->m_AIComponents[i]->AC_nextWaypointID++;

						if (this->m_AIComponents[i]->AC_nextWaypointID == this->m_AIComponents[i]->AC_nrOfWaypoint - 1)
						{
							//the platform stops when arriving at its destination
							this->m_AIComponents[i]->AC_triggered = false;
							this->m_AIComponents[i]->AC_direction = 1;
							this->m_AIComponents[i]->AC_position = this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nrOfWaypoint - 1];
							this->m_AIComponents[i]->AC_nextWaypointID = this->m_AIComponents[i]->AC_nrOfWaypoint - 2;
						}
						else if (this->m_AIComponents[i]->AC_nextWaypointID < 0)
							this->m_AIComponents[i]->AC_nextWaypointID = 0;
					}
					else
						UpdatePosition(i);
				}
				else
				{
					if (WaypointApprox(
						this->m_AIComponents[i]->AC_position,
						this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
						0.05f, i))
					{
						this->m_AIComponents[i]->AC_latestWaypointID = this->m_AIComponents[i]->AC_nextWaypointID;

						if (this->m_AIComponents[i]->AC_nextWaypointID > 0)
							this->m_AIComponents[i]->AC_nextWaypointID--;

						if (this->m_AIComponents[i]->AC_nextWaypointID == 0)
						{
							//the platform stops when arriving at its destination
							this->m_AIComponents[i]->AC_triggered = false;
							this->m_AIComponents[i]->AC_direction = 0;
							this->m_AIComponents[i]->AC_position = this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nrOfWaypoint - 1];
							this->m_AIComponents[i]->AC_nextWaypointID = 1;
						}
						else if (this->m_AIComponents[i]->AC_nextWaypointID > this->m_AIComponents[i]->AC_nrOfWaypoint - 1)
							this->m_AIComponents[i]->AC_nextWaypointID = this->m_AIComponents[i]->AC_nrOfWaypoint;
					}
					else
						UpdatePosition(i);
				}
			}
			else if (this->m_AIComponents[i]->AC_pattern == AI_ROUNDTRIP)
			{
				//Round-trip pattern
				//default direction 0 assumed in description.
				/*This pattern will move the platform from 0 to AC_numberOfWaypoint, back to 0 and then stop*/
				if (this->m_AIComponents[i]->AC_direction == 0)
				{
					if (WaypointApprox(
						this->m_AIComponents[i]->AC_position,
						this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
						0.05f, i))
					{
						this->m_AIComponents[i]->AC_latestWaypointID = this->m_AIComponents[i]->AC_nextWaypointID;
						this->m_AIComponents[i]->AC_nextWaypointID++;

						if (this->m_AIComponents[i]->AC_nextWaypointID >= this->m_AIComponents[i]->AC_nrOfWaypoint)
						{
							this->m_AIComponents[i]->AC_nextWaypointID--;
							this->m_AIComponents[i]->AC_direction = 1;
						}
					}
					else
						UpdatePosition(i);
				}
				else
				{
					if (WaypointApprox(
						this->m_AIComponents[i]->AC_position,
						this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
						0.05f, i))
					{
						this->m_AIComponents[i]->AC_latestWaypointID = this->m_AIComponents[i]->AC_nextWaypointID;
						this->m_AIComponents[i]->AC_nextWaypointID--;

						if (this->m_AIComponents[i]->AC_nextWaypointID < 0)
						{
							this->m_AIComponents[i]->AC_direction = 0;
							this->m_AIComponents[i]->AC_nextWaypointID = 1;

							//platform stops when returning to start position
							//this->m_AIComponents[i]->AC_triggered = false;
						}
					}
					else
						UpdatePosition(i);
				}
			}
			else if (this->m_AIComponents[i]->AC_pattern == AI_CIRCULAR)
			{
				//Circular pattern
				//default direction 0 assumed in description.
				/*This pattern will move the platform from 0 to AC_nrOfWaypoints then go to 0*/
				if (this->m_AIComponents[i]->AC_direction == 0)
				{
					if (WaypointApprox(
						this->m_AIComponents[i]->AC_position,
						this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
						0.05f, i))
					{
						this->m_AIComponents[i]->AC_nextWaypointID++;

						if (this->m_AIComponents[i]->AC_nextWaypointID >= this->m_AIComponents[i]->AC_nrOfWaypoint)
							this->m_AIComponents[i]->AC_nextWaypointID = 0;
					}
					else
						UpdatePosition(i);
				}
				else
				{
					if (WaypointApprox(
						this->m_AIComponents[i]->AC_position,
						this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
						0.05f, i))
					{
						this->m_AIComponents[i]->AC_nextWaypointID--;

						if (this->m_AIComponents[i]->AC_nextWaypointID <= this->m_AIComponents[i]->AC_nrOfWaypoint)
							this->m_AIComponents[i]->AC_nextWaypointID = this->m_AIComponents[i]->AC_nrOfWaypoint;
					}
					else
						UpdatePosition(i);
				}
			}
#ifdef _DEBUG
			if (this->m_AIComponents[i]->AC_pattern == AI_NONE)
			{
				printf("AIHandler::Update - Component %d to Entity %d was called containing AI_NONE pattern\n",
					i, this->m_AIComponents[i]->AC_entityID);
			}
#endif // _DEBUG
		}
	}
	return SUCCESS;
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

	for (int i = 0; i < 8; i++)
	{
		newComponent->AC_waypoints[i] = DirectX::XMVECTOR();
	}

	return newComponent;
}

bool AIHandler::WaypointApprox(DirectX::XMVECTOR c1, DirectX::XMVECTOR c2, float distance, int i)
{
	//Approximation if position of the platform is the same as the nextWaypoint to see if it reached its goal.
	float dx = abs(DirectX::XMVectorGetX(c2) - DirectX::XMVectorGetX(c1));
	float dy = abs(DirectX::XMVectorGetY(c2) - DirectX::XMVectorGetY(c1));
	float dz = abs(DirectX::XMVectorGetZ(c2) - DirectX::XMVectorGetZ(c1));

	if (dx > distance) return false; // too far in x direction
	if (dy > distance) return false; // too far in y direction
	if (dz > distance) return false; // too far in z direction

	this->m_AIComponents[i]->AC_WaypointUpdated = false;

	return true;
}

float AIHandler::Distance(const DirectX::XMVECTOR v1, const DirectX::XMVECTOR v2)
{
	DirectX::XMVECTOR vectorSub = DirectX::XMVectorSubtract(v1, v2);
	DirectX::XMVECTOR length = DirectX::XMVector3Length(vectorSub);

	float distance = 0.0f;
	DirectX::XMStoreFloat(&distance, length);

	return distance;
}

void AIHandler::UpdatePosition(int i)
{
	//When the platform has reached its destination, the WaypointUpdate is not updated (false). Then the platform is given a new Waypoint.
	if (this->m_AIComponents[i]->AC_WaypointUpdated == false && this->m_AIComponents[i]->AC_triggered)
	{
		this->m_AIComponents[i]->AC_dir = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(
			this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
			this->m_AIComponents[i]->AC_position));
		this->m_AIComponents[i]->AC_WaypointUpdated = true;
	}
}

void AIHandler::WaypointTime()
{
	float distance;
	float speed;

	for (size_t i = 0; i < this->m_nrOfAIComponents; i++)
	{
		for (size_t j = 0; j < this->m_AIComponents[i]->AC_nrOfWaypoint; j++)
		{
			DirectX::XMVECTOR v1 = this->m_AIComponents[i]->AC_waypoints[j];
			DirectX::XMVECTOR v2 = this->m_AIComponents[i]->AC_waypoints[j + 1];

			if (j == this->m_AIComponents[i]->AC_nrOfWaypoint - 1)
				v2 = this->m_AIComponents[i]->AC_waypoints[0];

			distance = Distance(v1, v2);
			speed = this->m_AIComponents[i]->AC_speed;

			this->m_AIComponents[i]->AC_waypointsTime[j] = distance / speed;
		}
	}
}
