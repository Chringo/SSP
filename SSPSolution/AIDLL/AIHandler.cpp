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
	//float dt = deltaTime;

	for (int i = 0; i < this->m_nrOfAIComponents; i++)
	{
		if (this->m_AIComponents[i] == nullptr)
			break;

		if (this->m_AIComponents[i]->AC_triggered)
		{
			switch (this->m_AIComponents[i]->AC_pattern)
			{
			case AI_ONEWAY:
#pragma region
				//One-way pattern
				//default direction 0 assumed in description.
				/*This pattern will move the platform from 0 to AC_nrOfWaypoint then stop*/
				if (!this->m_AIComponents[i]->AC_finished)
				{
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

							//the platform stops when arriving at its destination
							if (this->m_AIComponents[i]->AC_nextWaypointID == this->m_AIComponents[i]->AC_nrOfWaypoint - 1)
								this->m_AIComponents[i]->AC_finished = true;
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

							//the platform stops when arriving at its destination
							if (this->m_AIComponents[i]->AC_nextWaypointID == 0)
								this->m_AIComponents[i]->AC_finished = true;
						}
						else
							UpdatePosition(i);
					}
				}

				break;
#pragma endregion One way

			case AI_ROUNDTRIP:
#pragma region
				//Round-trip pattern
				//default direction 0 assumed in description.
				/*This pattern will move the platform from 0 to AC_numberOfWaypoint and stop at both these positions*/

				//test
				if (this->m_AIComponents[i]->AC_latestWaypointID < 0 || this->m_AIComponents[i]->AC_latestWaypointID > 1)
				{
					int i = 0;
				}

				if (this->m_AIComponents[i]->AC_nextWaypointID < 0 || this->m_AIComponents[i]->AC_nextWaypointID > 1)
				{
					int i = 0;
				}
				//test end

				if (this->m_AIComponents[i]->AC_reset && this->m_AIComponents[i]->AC_direction == 0)
				{
					//printf("-Returning\n");

					this->ChangeDirection(i);
					UpdatePosition(i);
				}
				else if (this->m_AIComponents[i]->AC_increasing && this->m_AIComponents[i]->AC_direction == 1)
				{
					this->ChangeDirection(i);
					UpdatePosition(i);
				}


				if (this->m_AIComponents[i]->AC_direction == 0 && !this->m_AIComponents[i]->AC_reset)
				{
					if (WaypointApprox(
						this->m_AIComponents[i]->AC_position,
						this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
						0.05f, i))
					{
						this->m_AIComponents[i]->AC_latestWaypointID = this->m_AIComponents[i]->AC_nextWaypointID;

						if (this->m_AIComponents[i]->AC_nextWaypointID < this->m_AIComponents[i]->AC_nrOfWaypoint - 1)
							this->m_AIComponents[i]->AC_nextWaypointID++;

						if (this->m_AIComponents[i]->AC_latestWaypointID >= this->m_AIComponents[i]->AC_nrOfWaypoint - 1)
						{
							//printf("##LastWaypoint\n");
							this->m_AIComponents[i]->AC_position = this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID];
							this->m_AIComponents[i]->AC_nextWaypointID--;

							this->ChangeDirection(i);

							this->m_AIComponents[i]->AC_triggered = false;
						}
					}
					else
					{
						//printf("-->\n");
						UpdatePosition(i);
					}
						
				}
				else if (this->m_AIComponents[i]->AC_direction == 1)
				{
					if (WaypointApprox(
						this->m_AIComponents[i]->AC_position,
						this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
						0.05f, i))
					{
						this->m_AIComponents[i]->AC_latestWaypointID = this->m_AIComponents[i]->AC_nextWaypointID;

						if (this->m_AIComponents[i]->AC_nextWaypointID > 0)
							this->m_AIComponents[i]->AC_nextWaypointID--;

						if (this->m_AIComponents[i]->AC_latestWaypointID <= 0)
						{
							//printf("#FirstWaypoint\n");
							this->m_AIComponents[i]->AC_position = this->m_AIComponents[i]->AC_waypoints[0];
							this->m_AIComponents[i]->AC_nextWaypointID++;

							this->ChangeDirection(i);

							this->m_AIComponents[i]->AC_triggered = false;
						}
					}
					else
					{
						//printf("<--\n");
						UpdatePosition(i);
					}
						
				}

				this->m_AIComponents[i]->AC_increasing = false;

				break;
#pragma endregion Round trip

			case AI_CIRCULAR:
#pragma region
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

				break;
#pragma endregion Circular

			case AI_RANDOM:
				break;

			case AI_NONE:

				//DEBUG
				printf("AIHandler::Update - Component %d to Entity %d was called containing AI_NONE pattern\n",
					i, this->m_AIComponents[i]->AC_entityID);
				break;

			default:
				break;
			}

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

int AIHandler::UpdateAIComponentList()
{
	int result = 0;

	for (int i = 0; i < m_nrOfAIComponents - 1; i++)
	{
		if (!this->m_AIComponents[i]->AC_active)
		{
			AIComponent* tempComponentPtr = this->m_AIComponents[this->m_nrOfAIComponents - 1];
			this->m_AIComponents[this->m_nrOfAIComponents - 1] = this->m_AIComponents[i];
			this->m_AIComponents[i] = tempComponentPtr;
			this->m_nrOfAIComponents--;
			i--;
			result++;
		}
	}
	if (!this->m_AIComponents[this->m_nrOfAIComponents - 1]->AC_active)
	{
		this->m_nrOfAIComponents--;
		result++;
	}

	return result;
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
	//When the platform has reached its destination, the WaypointUpdate is not updated (false).
	//Then the platform is given a new Waypoint.
	if (this->m_AIComponents[i]->AC_WaypointUpdated == false)
	{
		this->m_AIComponents[i]->AC_dir = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(
			this->m_AIComponents[i]->AC_waypoints[this->m_AIComponents[i]->AC_nextWaypointID],
			this->m_AIComponents[i]->AC_position));

		this->m_AIComponents[i]->AC_WaypointUpdated = true;
	}
}

bool AIHandler::ChangeDirection(int i)
{
	int temp;

	if (this->m_AIComponents[i]->AC_direction == 0)
		this->m_AIComponents[i]->AC_direction = 1;
	else
		this->m_AIComponents[i]->AC_direction = 0;

	temp = this->m_AIComponents[i]->AC_nextWaypointID;
	this->m_AIComponents[i]->AC_nextWaypointID = this->m_AIComponents[i]->AC_latestWaypointID;
	this->m_AIComponents[i]->AC_latestWaypointID = temp;

	this->m_AIComponents[i]->AC_WaypointUpdated = false;

	this->m_AIComponents[i]->AC_changedDirection = true;
	//this->m_AIComponents[i]->AC_reset = false;

	/*if (this->m_AIComponents[i]->AC_time == 2)
	{
		this->m_AIComponents[i]->AC_time = 0;
	}
	else
	{
		this->m_AIComponents[i]->AC_time++;
	}*/

	return true;
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

