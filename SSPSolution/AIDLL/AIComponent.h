#ifndef AIDLL_AI_AICOMPONENT_H
#define AIDLL_AI_AICOMPONENT_H

#include <DirectXMath.h>

__declspec(align(16)) struct AIComponent
{
	// System variables
	int m_active = 0;
	int m_entityID = -1;

	// AI variables
	bool m_triggered;		// Trigger handling
	int m_time;				// How long the component is active

	float m_speed;			// Movement speed
	DirectX::XMVECTOR m_dir;// Normalised direction vector

	DirectX::XMVECTOR m_position;// Current position
	int m_pattern;			// Traversing of waypoints
	int m_direction;		
	int m_nextWaypointID;	// Index to next waypoint 
	int m_latestWaypointID;	// Index to latest visited waypoint
	int m_nrOfWaypoint;		// Nr of waypoints used in array

	DirectX::XMVECTOR m_waypoints[8];

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
};

#endif