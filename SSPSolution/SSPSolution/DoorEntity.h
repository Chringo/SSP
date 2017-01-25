#ifndef SSPAPPLICATION_ENTITIES_DOORENTITY_H
#define SSPAPPLICATION_ENTITIES_DOORENTITY_H

#include "Entity.h"
#include <vector>

class DoorEntity :
	public Entity
{
private:
	/*struct ElementState {
		int entityID;
		EVENT desiredState;
		bool desiredStateReached;
	};

	std::vector<ElementState> m_elementStates;*/
	
	bool m_isOpened;
	float m_minRotation;
	float m_maxRotation;
	float m_rotateTime;
	float m_rotatePerSec;

public:
	DoorEntity();
	virtual ~DoorEntity();
	
	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, float rotateTime = 1.0f, float minRotation = 0.0f, float maxRotation = DirectX::XM_PI / 2.0f);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	bool SetIsOpened(bool isOpened);
	bool GetIsOpened();
};

#endif