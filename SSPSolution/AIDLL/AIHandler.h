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

public:
	AIHandler();
	~AIHandler();
	int Shutdown();

	int Initialize();
	int Update(float deltaTime);
private:	// Helper functions

};
#endif