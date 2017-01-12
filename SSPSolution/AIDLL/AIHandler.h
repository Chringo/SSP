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

class AIDLL_API AIHandler
{
private:
	std::vector<AIComponent*> m_AIComponents;
	int m_nrOfAIComponents;

public:
	AIHandler();
	~AIHandler();
	int Shutdown();

	int Initialize(int nrOfAIComponents);
	int Update(float deltaTime);

	// Setters
	void SetComponentActive(int compID);
	void SetComponentFalse(int compID);

	void SetEntityID(int compID, int entityID);
	void SetTriggered(int compID, bool triggered);
	void SetTime(int compID, int time);
	void SetSpeed(int compID, int speed);
	void SetDirection(int compID, int direction);
	void SetCurrentWaypoint(int compID, int currentWaypoint);
	void SetNrOfWaypoints(int compID, int nrOfWaypoints);
	void SetWaypoints(int compID, DirectX::XMVECTOR waypoints[]);

	// Getters
	int GetNrOfAIComponents() const;

	
private:	// Helper functions
	AIComponent* CreateAIComponent(int entityID);
};
#endif