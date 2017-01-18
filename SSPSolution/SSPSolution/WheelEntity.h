#ifndef SSPAPPLICATION_ENTITIES_WHEELENTITY_H
#define SSPAPPLICATION_ENTITIES_WHEELENTITY_H

#include "Entity.h"
class WheelEntity :
	public Entity
{
private:
	//Variables
	float m_minRotation;
	float m_maxRotation;
	float m_rotateTime;
	float m_rotatePerSec;
	float m_range;
	// -2:minRotation | -1:negativeRotation | 0:noRotation | 1:positiveRotation | 2:maxRotation
	int m_rotationState;
public:
	WheelEntity();
	~WheelEntity();

	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, float interactionDistance = 4.0f, float minRotation = 0.0f, float maxRotation = DirectX::XM_PI / 2, float rotateTime = 1.0f);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	int CheckPlayerInteraction(DirectX::XMFLOAT3 playerPos, bool increasing);
private:
	//Functions
};

#endif