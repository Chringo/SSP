#ifndef AIDLL_AI_AICOMPONENT_H
#define AIDLL_AI_AICOMPONENT_H

#include <DirectXMath.h>

__declspec(align(16)) struct AIComponent
{
	// system variables
	int m_active = 0;
	int m_entityID = -1;

	// AI variables
	bool m_triggered;
	int m_time;
	int m_speed;
	int m_direction;
	int m_currentWaypoint;
	int m_nextWaypoint;

	DirectX::XMVECTOR m_waypoints[8];
};

#endif