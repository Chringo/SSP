#ifndef SSPAPPLICATION_ENTITIES_DOORENTITY_H
#define SSPAPPLICATION_ENTITIES_DOORENTITY_H

#include "Entity.h"
#include <vector>

class DoorEntity :
	public Entity
{
private:
	struct ElementState {
		int entityID;
		EVENT desiredState;
		bool desiredStateReached;
	};

	std::vector<ElementState> m_elementStates;
	
	bool m_isOpened;
	float m_minRotation;
	float m_maxRotation;
	float m_rotateTime;
	float m_rotatePerSec;

public:
	DoorEntity();
	virtual ~DoorEntity();
	

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	bool SetIsOpened(bool isOpened);
	bool GetIsOpened();
};

#endif