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
	bool m_isActive;
	float m_range;

	bool m_needSync;
public:
	LeverEntity();
	virtual ~LeverEntity();

	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	//For now we check only if the player is close enough
	int CheckPressed(DirectX::XMFLOAT3 playerPos);

	void SetSyncState(LeverSyncState* newSyncState);
	LeverSyncState* GetSyncState();
private:
	//Functions
};

#endif