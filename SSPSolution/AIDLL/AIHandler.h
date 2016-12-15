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
	int m_nrOfAIComponents;

public:
	AIHandler();
	~AIHandler();
	int Shutdown();

	int Initialize(int nrOfAIComponents);
	int Update(float deltaTime);

	// Setters

	// Getters
	int getNrOfAIComponents() const;
	
private:	// Helper functions
	AIComponent* CreateAIComponent(int entityID);
};
#endif