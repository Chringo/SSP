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
	m_component->m_speed = speed;
}

void AIController::SetTime(float time)
{
	m_component->m_time = time;
}

void AIController::AddWaypoint(DirectX::XMVECTOR position)
{
	if (m_component->m_nrOfWaypoint < 8) {
		m_component->m_waypoints[m_component->m_nrOfWaypoint] = position;
		m_component->m_nrOfWaypoint += 1;
	}
}

void AIController::RemoveWayPoint(int index)
{
	if (index > m_component->m_nrOfWaypoint - 1 || index < 0)
		return;

	for (size_t i = index; i <  m_component->m_nrOfWaypoint - 1; i++)
	{
	
			m_component->m_waypoints[i] = m_component->m_waypoints[i+1];
	}
	m_component->m_nrOfWaypoint -= 1;
}

void AIController::DeletePath()
{
	m_component->m_nrOfWaypoint = 0;
}

void AIController::SetID(int entityID)
{
	m_component->m_entityID = entityID;
}
