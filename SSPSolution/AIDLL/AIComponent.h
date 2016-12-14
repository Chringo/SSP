#ifndef AIDLL_AI_AICOMPONENT_H
#define AIDLL_AI_AICOMPONENT_H

#include <DirectXMath.h>

struct AIComponent
{
	int active = 0;
	int entityID = -1;

	int direction;
	int currentWaypoint;
	int nextWaypoint;

	DirectX::XMVECTOR waypoints[8];
};

#endif