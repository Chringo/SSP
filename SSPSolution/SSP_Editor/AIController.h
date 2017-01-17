#ifndef SSPEDITOR_AICONTROLLER_AICONTROLLER_H
#define SSPEDITOR_AICONTROLLER_AICONTROLLER_H
#include "../AIDLL/AIComponent.h"
/*
	Author: Martin Clementson

	This class is used to control AI data. Such as add/remove waypoint.
	Change speed / time ,etc.
*/
class AIController
{
	
private:
	AIComponent* m_component;
	AIController();
public:
	AIController(AIComponent* component);
	virtual ~AIController();

	void SetSpeed(float speed);
	void SetTime(float time);
	void SetID(int entityID);
};

#endif