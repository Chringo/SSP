#ifndef AIDLL_AI_AICOMPONENT_H
#define AIDLL_AI_AICOMPONENT_H

#include <DirectXMath.h>

__declspec(align(16)) struct AIComponent
{
	// System variables
	int AP_active = 0;
	int AP_entityID = -1;

	// AI variables
	bool AP_triggered;			// Trigger handling
	int AP_time;				// How long the component is active

	float AP_speed;				// Movement speed
	DirectX::XMVECTOR AP_dir;	// Normalised direction vector

	DirectX::XMVECTOR AP_position;// Current position
	int AP_pattern;				// Traversing of waypoints
	int AP_direction;			// Direction in array, might be removed due to AP_pattern's existance
	int AP_nextWaypointID;		// Index to next waypoint 
	int AP_latestWaypointID;	// Index to latest visited waypoint
	int AP_nrOfWaypoint;		// Nr of waypoints used in array

	DirectX::XMVECTOR AP_waypoints[8];

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
};

#endif