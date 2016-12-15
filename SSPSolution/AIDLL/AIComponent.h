#ifndef AIDLL_AI_AICOMPONENT_H
#define AIDLL_AI_AICOMPONENT_H

#include <DirectXMath.h>

__declspec(align(16)) struct AIComponent
{
	// system variables
	int active = 0;
	int entityID = -1;

	// AI variables
	bool triggered;
	int time;
	int speed;
	int direction;
	int currentWaypoint;
	int nextWaypoint;

	DirectX::XMVECTOR waypoints[8];
};

#endif