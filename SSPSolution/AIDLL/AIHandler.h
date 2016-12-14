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
private:	// Variables
	std::vector<AIComponent*> m_AIComponents;
	int m_nrOfAIComponent;
	int m_maxAIComponent;

public:
	AIHandler();
	~AIHandler();
	int Shutdown();

	int Initialize(int nrOfAIComponents, int maxAIComponent);
	int Update(float deltaTime);
	
private:	// Helper functions
	AIComponent * CreateAIComponent();
};
#endif