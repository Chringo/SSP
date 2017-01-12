#ifndef AIDLL_AI_AIHANDLER_H
#define AIDLL_AI_AIHANDLER_H

#ifdef AIDLL_EXPORTS
#define AIDLL_API __declspec(dllexport)
#else
#define AIDLL_API __declspec(dllimport)
#endif

#include "AIComponent.h"
#include <DirectXMath.h>
#include <vector>

class AIHandler
{
private:
	std::vector<AIComponent*> m_AIComponents;
	int m_nrOfAIComponents;

public:
	AIDLL_API AIHandler();
	AIDLL_API ~AIHandler();
	AIDLL_API int Shutdown();

	AIDLL_API int Initialize(int nrOfAIComponents);
	AIDLL_API int Update(float deltaTime);

	// Setters
	AIDLL_API void SetComponentActive(int compID);
	AIDLL_API void SetComponentFalse(int compID);

	AIDLL_API void SetEntityID(int compID, int entityID);
	AIDLL_API void SetTriggered(int compID, bool triggered);
	AIDLL_API void SetTime(int compID, int time);
	AIDLL_API void SetSpeed(int compID, int speed);
	AIDLL_API void SetDirection(int compID, int direction);
	AIDLL_API void SetCurrentWaypoint(int compID, int currentWaypoint);
	AIDLL_API void SetNrOfWaypoints(int compID, int nrOfWaypoints);
	AIDLL_API void SetWaypoints(int compID, DirectX::XMVECTOR waypoints[]);

	// Getters
	AIDLL_API int GetNrOfAIComponents() const;
	
private:	// Helper functions
	AIComponent* CreateAIComponent(int entityID);
};
#endif