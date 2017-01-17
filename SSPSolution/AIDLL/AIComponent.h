#ifndef AIDLL_AI_AICOMPONENT_H
#define AIDLL_AI_AICOMPONENT_H

#include <DirectXMath.h>

__declspec(align(16)) struct AIComponent
{
	// System variables
	int m_active = 0;
	int m_entityID = -1;
	DirectX::XMVECTOR m_position;

	// AI variables
	bool m_triggered;
	int m_pattern;
	int m_time;
	int m_speed;
	int m_direction;
	int m_nextWaypoint;
	int m_currentWaypoint;
	int m_nrOfWaypoint;

	DirectX::XMVECTOR m_waypoints[8];

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
};

#endif