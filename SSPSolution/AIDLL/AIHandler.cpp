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
		bool x = this->m_AIComponents.at(i)->AC_triggered;

		if (this->m_AIComponents.at(i)->AC_active)
		{
			// AIComponent logic/behavior, movement of e.g. platforms
			if (this->m_AIComponents.at(i)->AC_pattern == AI_ONEWAY)
			{
				//One-way pattern
				//default direction 0 assumed in description.
				/*This pattern will move the platform from 0 to AC_nrOfWaypoint then stop*/
				if (this->m_AIComponents.at(i)->AC_direction == 0)
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f, i))
					{
						//snap into place
						this->m_AIComponents.at(i)->AC_position = this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID];
						this->m_AIComponents.at(i)->AC_latestWaypointID = this->m_AIComponents.at(i)->AC_nextWaypointID;
						this->m_AIComponents.at(i)->AC_nextWaypointID++;
						this->m_AIComponents.at(i)->AC_direction = 1;
						//the platform stops when arriving at its destination
						this->m_AIComponents.at(i)->AC_triggered = false;
					}
					else if (x)
						UpdatePosition(i);
				}
				else
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f, i))
					{
						//snap into place
						this->m_AIComponents.at(i)->AC_position = this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID];
						this->m_AIComponents.at(i)->AC_latestWaypointID = this->m_AIComponents.at(i)->AC_nextWaypointID;
						this->m_AIComponents.at(i)->AC_nextWaypointID--;
						this->m_AIComponents.at(i)->AC_direction = 0;
						//the platform stops when arriving at its destination
						this->m_AIComponents.at(i)->AC_triggered = false;
					}
					else if (x)
						UpdatePosition(i);
				}
			}
			else if (this->m_AIComponents.at(i)->AC_pattern == AI_ROUNDTRIP)
			{
				//Round-trip pattern
				//default direction 0 assumed in description.
				/*This pattern will move the platform from 0 to AC_numberOfWaypoint, back to 0 and then stop*/
				if (this->m_AIComponents.at(i)->AC_direction == 0)
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f, i))
					{
						//snap into place
						this->m_AIComponents.at(i)->AC_position = this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID];
						this->m_AIComponents.at(i)->AC_latestWaypointID = this->m_AIComponents.at(i)->AC_nextWaypointID;
						this->m_AIComponents.at(i)->AC_nextWaypointID++;

						//the platform stops when arriving at its starting location.
						if (this->m_AIComponents.at(i)->AC_nextWaypointID >= this->m_AIComponents.at(i)->AC_nrOfWaypoint)
						{
							this->m_AIComponents.at(i)->AC_nextWaypointID--;
							this->m_AIComponents.at(i)->AC_direction = 1;
						}
					}
					else if (x)
						UpdatePosition(i);
				}
				else
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f, i))
					{
						//snap into place
						this->m_AIComponents.at(i)->AC_position = this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID];
						this->m_AIComponents.at(i)->AC_latestWaypointID = this->m_AIComponents.at(i)->AC_nextWaypointID;
						this->m_AIComponents.at(i)->AC_nextWaypointID--;

						if (this->m_AIComponents.at(i)->AC_nextWaypointID < 0)
						{
							//the platform ready to be triggered again
							this->m_AIComponents.at(i)->AC_direction = 0;
							this->m_AIComponents.at(i)->AC_nextWaypointID = 0;
							//platform stops when returning to start position
							//this->m_AIComponents.at(i)->AC_triggered = false;
						}
					}
					else if (x)
						UpdatePosition(i);
				}
			}
			else
			{
				//Circular pattern
				//default direction 0 assumed in description.
				/*This pattern will move the platform from 0 to AC_nrOfWaypoints then go to 0*/
				if (this->m_AIComponents.at(i)->AC_direction == 0)
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f, i))
					{
						//snap into place
						this->m_AIComponents.at(i)->AC_position = this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID];
						//next waypoint
						this->m_AIComponents.at(i)->AC_nextWaypointID++;

						if (this->m_AIComponents.at(i)->AC_nextWaypointID >= this->m_AIComponents.at(i)->AC_nrOfWaypoint)
							this->m_AIComponents.at(i)->AC_nextWaypointID = 0;
					}
					else if (x)
						UpdatePosition(i);
				}
				else
				{
					if (WaypointApprox(
						this->m_AIComponents.at(i)->AC_position,
						this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
						1.0f, i))
					{
						//snap into place
						this->m_AIComponents.at(i)->AC_position = this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID];
						//next waypoint
						this->m_AIComponents.at(i)->AC_nextWaypointID--;

						if (this->m_AIComponents.at(i)->AC_nextWaypointID <= this->m_AIComponents.at(i)->AC_nrOfWaypoint)
							this->m_AIComponents.at(i)->AC_nextWaypointID = this->m_AIComponents.at(i)->AC_nrOfWaypoint;
					}
					else if (x)
						UpdatePosition(i);
				}
				//UpdateMe(i, deltaTime);
			}
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
	//newComponent->AC_nextWaypointID = 1;

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

	this->m_AIComponents.at(i)->AC_WaypointUpdated = false;

	return true;
}

bool AIHandler::VectorEqual(DirectX::XMVECTOR a, DirectX::XMVECTOR b)
{
	float dx = abs(DirectX::XMVectorGetX(b) - DirectX::XMVectorGetX(a));
	float dy = abs(DirectX::XMVectorGetY(b) - DirectX::XMVectorGetY(a));
	float dz = abs(DirectX::XMVectorGetZ(b) - DirectX::XMVectorGetZ(a));

	float sqrt = dx*dx + dy*dy + dz*dz;

	return sqrt < 0.0001f;
}

void AIHandler::UpdatePosition(int i)
{
	if (this->m_AIComponents.at(i)->AC_WaypointUpdated == false)
	{
		this->m_AIComponents.at(i)->AC_dir = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(
			this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
			this->m_AIComponents.at(i)->AC_position));
		this->m_AIComponents.at(i)->AC_WaypointUpdated = true;
	}

	DirectX::XMVECTOR v = DirectX::XMVECTOR();
	v = DirectX::XMVectorScale(DirectX::XMVector3Normalize(this->m_AIComponents.at(i)->AC_dir), this->m_AIComponents.at(i)->AC_speed);
	//if (dt < 144.0f)
	//{
	//	v = DirectX::XMVectorScale(v, dt);
	//}

	this->m_AIComponents.at(i)->AC_position = DirectX::XMVectorAdd(v, this->m_AIComponents.at(i)->AC_position);
}

//void AIHandler::UpdateMe(int i, float deltaTime)
//{
//	if (this->m_AIComponents.at(i)->AC_WaypointUpdated == false)
//	{
//		this->m_AIComponents.at(i)->AC_dir = DirectX::XMVectorSubtract(
//			this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
//			this->m_AIComponents.at(i)->AC_position);
//		this->m_AIComponents.at(i)->AC_WaypointUpdated = true;
//	}
//
//	DirectX::XMVECTOR a = this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID];
//	DirectX::XMVECTOR b = this->m_AIComponents.at(i)->AC_position;
//
//	float distCovered = deltaTime * this->m_AIComponents.at(i)->AC_speed;
//
//	float dx = abs(DirectX::XMVectorGetX(b) - DirectX::XMVectorGetX(a));
//	float dy = abs(DirectX::XMVectorGetY(b) - DirectX::XMVectorGetY(a));
//	float dz = abs(DirectX::XMVectorGetZ(b) - DirectX::XMVectorGetZ(a));
//
//	float sqrt = dx*dx + dy*dy + dz*dz; // magnitude/length
//
//	float fracJourney = distCovered / sqrt;
//
//	this->m_AIComponents.at(i)->AC_position = Vector3.Lerp(startMarker.position, endMarker.position, fracJourney);
//
//	/*DirectX::XMVECTOR v = DirectX::XMVECTOR();
//	this->m_AIComponents.at(i)->AC_position = DirectX::XMVectorAdd(v, this->m_AIComponents.at(i)->AC_position);*/
//}

void AIHandler::UpdateMe(int i, float deltaTime)
{
	float sqrt = 0.0f;
	DirectX::XMVECTOR a = this->m_AIComponents.at(i)->AC_position;
	DirectX::XMVECTOR b = this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID];

	/*this->m_AIComponents.at(i)->AC_dir = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(
		this->m_AIComponents.at(i)->AC_waypoints[this->m_AIComponents.at(i)->AC_nextWaypointID],
		this->m_AIComponents.at(i)->AC_position));*/
	this->m_AIComponents.at(i)->AC_WaypointUpdated = true;

	float dx = abs(DirectX::XMVectorGetX(b) - DirectX::XMVectorGetX(a));
	float dy = abs(DirectX::XMVectorGetY(b) - DirectX::XMVectorGetY(a));
	float dz = abs(DirectX::XMVectorGetZ(b) - DirectX::XMVectorGetZ(a));

	sqrt = dx*dx + dy*dy + dz*dz; // magnitude/length

	this->m_AIComponents.at(i)->AC_length = sqrt;

	float distCovered = 1.0f * this->m_AIComponents.at(i)->AC_speed;

	float fracJourney = distCovered / sqrt;

	//DirectX::XMVECTOR test = DirectX::XMVectorLerp(a, b, fracJourney);
	this->m_AIComponents.at(i)->AC_position = DirectX::XMVectorLerp(a, b, fracJourney);
	//this->m_AIComponents.at(i)->AC_position = DirectX::XMVectorAdd(DirectX::XMVectorLerp(a, b, fracJourney), this->m_AIComponents.at(i)->AC_position);
}