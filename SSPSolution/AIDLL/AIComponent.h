#ifndef AIDLL_AI_AICOMPONENT_H
#define AIDLL_AI_AICOMPONENT_H

#include <DirectXMath.h>
enum Pattern : int
{
	AI_ONEWAY = 1,
	AI_CIRCULAR,
	AI_ROUNDTRIP,
	AI_RANDOM,
	AI_NONE = -1
};
__declspec(align(16)) struct AIComponent
{
	// System variables
	int AC_active = 0;
	int AC_entityID = -1;

	// AI variables
	bool AC_triggered = false;	// Trigger handling
	int AC_time = 0;			// How long the component is active
	bool AC_WaypointUpdated = false;//If false it will update the XMVECTOR AC_dir for the component this frame in the update function.
	float AC_speed = 0;			// Movement speed
	DirectX::XMVECTOR AC_dir = DirectX::XMVECTOR();			// Normalised direction vector

	DirectX::XMVECTOR AC_position = DirectX::XMVECTOR();	// Current position
	int AC_pattern = AI_NONE;	// Traversing of waypoints
	int AC_direction = 0;		// Direction in array, might be removed due to AC_pattern's existance
	int AC_nextWaypointID = 1;	// Index to next waypoint 
	int AC_latestWaypointID = 0;// Index to latest visited waypoint
	int AC_nrOfWaypoint = 0;	// Nr of waypoints used in array

	// One way
	bool AC_finished = false;	// Triggered by One Way pattern when reaching final destination

	// Round trip
	bool AC_endpointReached = false;
	bool AC_reset = false;
	bool AC_oldReset = false;


	DirectX::XMVECTOR AC_waypoints[8];
	float AC_waypointsTime[8];

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
};

#endif