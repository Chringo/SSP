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

void AIController::SetID(int entityID)
{
	m_component->m_entityID = entityID;
}
