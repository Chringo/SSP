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
	void AddWaypoint(DirectX::XMVECTOR position);
	void RemoveWayPoint(int index);
	void DeletePath();
	void SetID(int entityID);
	void SetComponent(AIComponent* component) { this->m_component = component; };
	void SetPattern(int pattern);
	AIComponent* GetComponent() { return m_component; };

};

#endif