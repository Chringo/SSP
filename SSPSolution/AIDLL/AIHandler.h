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
	int m_maxOfAIComponents;

public:
	AIDLL_API AIHandler();
	AIDLL_API ~AIHandler();
	AIDLL_API int Shutdown();

	AIDLL_API int Initialize(int maxOfAIComponents);
	AIDLL_API int Update(float deltaTime);

	// Setters
	AIDLL_API void SetComponentActive(int compID);
	AIDLL_API void SetComponentFalse(int compID);

	//Entity ID
	AIDLL_API void SetEntityID(int compID, int entityID);
	//Active in-game
	AIDLL_API void SetTriggered(int compID, bool triggered);
	//Time limited activity
	AIDLL_API void SetTime(int compID, int time);
	//Speed for AI movement calculation
	AIDLL_API void SetSpeed(int compID, float speed);
	//Direction between waypoints for logic
	AIDLL_API void SetDirection(int compID, int direction);
	//Used as a index and to know where you are moving from	
	AIDLL_API void SetLatestWaypoint(int compID, int currentWaypoint);
	//Set the pattern for the entity. (Linear = 1, Circular = 2, Random = 3)
	AIDLL_API void SetPattern(int compID, int pattern);
	//Set waypoints, nrOfWaypoints++
	AIDLL_API void SetWaypoints(int compID, DirectX::XMVECTOR waypoints[]);

	// Getters
	AIDLL_API int GetNrOfAIComponents() const;
	//Get new position for the Object
	AIDLL_API DirectX::XMVECTOR GetPosition(int compID) const;
	//Retrieve next available component in the vector
	AIDLL_API AIComponent* GetNextAvailableComponents();

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };

private:
	// Internal Variables
	bool WaypointUpdated; //If false it will update the XMVECTOR AC_dir for the component this frame in the update function.

	// Helper functions
	AIComponent* CreateAIComponent(int entityID);
	bool WaypointApprox(DirectX::XMVECTOR c1, DirectX::XMVECTOR c2, float distance);
	int GetNextWaypoint(int compID, int pattern);

};
#endif