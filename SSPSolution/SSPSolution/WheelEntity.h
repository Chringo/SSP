#ifndef SSPAPPLICATION_ENTITIES_WHEELENTITY_H
#define SSPAPPLICATION_ENTITIES_WHEELENTITY_H
#include <math.h>
#include "Entity.h"

struct WheelSyncState{
	unsigned int entityID;
	int rotationState;	//-2:resetingRotation | -1:negativeRotation | 0:noRotation | 1:positiveRotation
	float rotationAmount;
};

class WheelEntity :
	public Entity
{
private:
	//Variables
	float m_minRotation;
	float m_maxRotation;
	float m_rotateTime;
	//Precalculate to help performance
	float m_rotatePerSec;
	float m_range;
	//Precalculate to help performance
	const float FLOAT_FIX = 1.0f - 0.00001f;

	bool m_resets;
	bool m_isMin;
	float m_resetTime;
	//Precalculate to help performance
	float m_resetRotatePerSec;
	float m_timeUntilReset;
	float m_resetCountdown;
	enum {Resetting = -2, RotatingDecrease = -1, Resting = 0, RotatingIncrease = 1, MaxRotation = 2};
	// -2:resetingRotation | -1:negativeRotation | 0:noRotation | 1:positiveRotation | 2:MaxAndResting
	int m_rotationState;

	bool m_needSync;
public:
	WheelEntity();
	~WheelEntity();
	//Max and Min rotation are all described as 1.0f = 1 = 1 entire turn of the wheel
	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, float interactionDistance = 4.0f, float minRotation = 0.0f, float maxRotation = 1.0f, float rotateTime = 1.0f, bool resets = false, float resetTime = 1.0f, float timeUntilReset = 0.0f);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	int CheckPlayerInteraction(DirectX::XMFLOAT3 playerPos, int increasing);

	float SetMinRotation(float amount);
	float SetMaxRotation(float amount);
	float SetRotateTime(float time);
	float SetInteractionDistance(float distance);
	
	float GetMinRotation();
	float GetMaxRotation();
	float GetRotateTime();
	float GetInteractionDistance();

	void SetSyncState(WheelSyncState* newSyncState);
	WheelSyncState* GetSyncState();
	WheelSyncState* GetUnconditionalState();

private:
	//Functions
};

#endif