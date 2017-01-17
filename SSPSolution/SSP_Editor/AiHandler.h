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
	std::vector<AIComponent> m_PathComponents;

public:
	AiHandler();
	virtual ~AiHandler();

	//Path Component Functions
	AIComponent* NewPathComponent();
	AIComponent* GetPathComponent(int EntityID);
	std::vector<AIComponent>* GetAllPathComponents();
	void DeletePathComponent(int EntityID);
	/////////////////////////////////////////////

	void Destroy(); //Used when a new level is loaded. 
};

#endif

