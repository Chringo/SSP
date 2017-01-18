#ifndef AIDLL_AI_AICOMPONENT_H
#define AIDLL_AI_AICOMPONENT_H

#include <DirectXMath.h>
enum Pattern : int
{
	AI_LINEAR = 1,
	AI_CIRCULAR,
	AI_ROUNTRIP,
	AI_RANDOM,
	AI_NONE = -1
};
__declspec(align(16)) struct AIComponent
{
	// System variables
	int AP_active = 0;
	int AP_entityID = -1;

	// AI variables
	bool AP_triggered = false;	// Trigger handling
	int AP_time = 0;			// How long the component is active

	float AP_speed = 0;			// Movement speed
	DirectX::XMVECTOR AP_dir = DirectX::XMVECTOR();// Normalised direction vector

	DirectX::XMVECTOR AP_position = DirectX::XMVECTOR();// Current position
	int AP_pattern = AI_NONE;	// Traversing of waypoints
	int AP_direction = 0;		// Direction in array, might be removed due to AP_pattern's existance
	int AP_nextWaypointID = 0;	// Index to next waypoint 
	int AP_latestWaypointID = 1;// Index to latest visited waypoint
	int AP_nrOfWaypoint = 0;	// Nr of waypoints used in array

	DirectX::XMVECTOR AP_waypoints[8];

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
};

#endif