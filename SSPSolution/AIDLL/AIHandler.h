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

	//Retrieve next available component in the vector
	AIDLL_API AIComponent* GetNextAvailableComponents();
	AIDLL_API void WaypointTime();

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };

private:

	// Helper functions
	AIComponent* CreateAIComponent(int entityID);
	bool WaypointApprox(DirectX::XMVECTOR c1, DirectX::XMVECTOR c2, float distance, int i);
	float Distance(const DirectX::XMVECTOR v1, const DirectX::XMVECTOR v2);
	void UpdatePosition(int i);
	
};
#endif