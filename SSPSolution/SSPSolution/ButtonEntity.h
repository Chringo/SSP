#ifndef SSPAPPLICATION_ENTITIES_BUTTONENTITY_H
#define SSPAPPLICATION_ENTITIES_BUTTONENTITY_H
#include "Entity.h"

struct ButtonSyncState {
	unsigned int entityID;
	bool isActive;
};

class ButtonEntity :
	public Entity
{
private:
	//Variables
	bool m_isActive;
	float m_range;
	float m_resetTime;
	float m_elapsedResetTime;
	bool m_needSync;

	bool m_animationActive = false;
	float m_currOffsetValue  =  0; // equal to door and levers m_currRot variable
	float m_animSpeed		 =  2.0f;
	float m_activatedOffset  = -0.35f; 
	float m_targetOffset	 = 0;
public:
	ButtonEntity();
	virtual ~ButtonEntity();

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	//Time in seconds
	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, float interactionDistance = 1.5f, float resetTime = 4.0f);

	//For now we check only if the player is close enough
	int CheckPressed(DirectX::XMFLOAT3 playerPos);

	void SetSyncState(ButtonSyncState* newSyncState);
	ButtonSyncState* GetSyncState();
private:
	//Functions
};

#endif