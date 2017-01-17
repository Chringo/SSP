#ifndef SSPEDITOR_AIHANDLER_AIHANDLER_H
#define SSPEDITOR_AIHANDLER_AIHANDLER_H
#include "../AIDLL/AIComponent.h"
#include <vector>
class AiHandler
{
private:
	std::vector<AIComponent> m_components;

public:
	AiHandler();
	virtual ~AiHandler();

	AIComponent* NewComponent();
	AIComponent* GetComponent(int index);
	std::vector<AIComponent>* GetAllComponents();
	void DeleteComponent(AIComponent* component);
};

#endif

