#ifndef SSPEDITOR_AIHANDLER_AIHANDLER_H
#define SSPEDITOR_AIHANDLER_AIHANDLER_H
#include "../AIDLL/AIComponent.h"
#include <vector>

/*
Author: Martin Clementson

This Class holds the AI components that is in use by the current level.
The components are related to a container/entity and share the same ID.
To remove use the ID.

When a new level is created. This class is cleared of its data.
*/

class AiHandler
{
private:
	std::vector<AIComponent> m_components;

public:
	AiHandler();
	virtual ~AiHandler();

	AIComponent* NewComponent();
	AIComponent* GetComponent(int EntityID);
	std::vector<AIComponent>* GetAllComponents();
	void DeleteComponent(int EntityID);

	void Destroy(); //Used when a new level is loaded. 
};

#endif

