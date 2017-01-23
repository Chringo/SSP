#include "AIController.h"



AIController::AIController()
{
}


AIController::AIController(AIComponent *component)
{
	this->m_component = component;
}

AIController::~AIController()
{
}

void AIController::SetSpeed(float speed)
{
	m_component->AC_speed = speed;
}

void AIController::SetTime(float time)
{
	m_component->AC_time = time;
}

void AIController::AddWaypoint(DirectX::XMVECTOR position)
{
	if (m_component->AC_nrOfWaypoint < 8) {
		m_component->AC_waypoints[m_component->AC_nrOfWaypoint] = position;
		m_component->AC_nrOfWaypoint += 1;
	}
}

void AIController::RemoveWayPoint(int index)
{
	if (index > m_component->AC_nrOfWaypoint - 1 || index < 0)
		return;

	for (size_t i = index; i <  m_component->AC_nrOfWaypoint - 1; i++)
	{
	
			m_component->AC_waypoints[i] = m_component->AC_waypoints[i+1];
	}
	m_component->AC_nrOfWaypoint -= 1;
}

void AIController::DeletePath()
{
	m_component->AC_nrOfWaypoint = 0;
}

void AIController::SetID(int entityID)
{
	m_component->AC_entityID = entityID;
}

void AIController::SetPattern(int pattern)
{
	this->m_component->AC_pattern = pattern;
}
