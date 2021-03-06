#ifndef SSPAPPLICATION_ENTITIES_LEVERENTITY_H
#define SSPAPPLICATION_ENTITIES_LEVERENTITY_H
#include "Entity.h"

struct LeverSyncState {
	unsigned int entityID;
	bool isActive;
};

class LeverEntity :
	public Entity
{
private:
	//Variables
	bool  m_isActive;
	float m_range;
	float m_animSpeed  = 400.0f;
	float m_targetRot  = 0;
	float m_currRot    = 0;
	float m_activatedRotation = -90.0f;
	bool  m_animationActive = false;
	bool  m_needSync;
	float m_lastFrameRotValue;
public:
	LeverEntity();
	virtual ~LeverEntity();
	 
	int Initialize(unsigned int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, float interactionDistance);

	int Update(float dT, InputHandler* inputHandler);
	int React(unsigned int entityID, EVENT reactEvent);

	//For now we check only if the player is close enough
	int CheckPressed(DirectX::XMFLOAT3 playerPos);

	void SetSyncState(LeverSyncState* newSyncState);
	LeverSyncState* GetSyncState();
	bool GetIsActive();
private:
	//Functions
};

#endif