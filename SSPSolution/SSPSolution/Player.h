#ifndef SSPAPPLICATION_ENTITIES_PLAYER_H
#define SSPAPPLICATION_ENTITIES_PLAYER_H
#include "Entity.h"
class Player :
	public Entity
{
private:
	//Meters per second
	float m_speed;
	float m_throwStrength;

	Entity* m_grabbed;
	DirectX::XMVECTOR m_carryOffset;

	DirectX::XMVECTOR m_lookDir;
	DirectX::XMVECTOR m_upDir;
	DirectX::XMVECTOR m_rightDir;
public:
	Player();
	~Player();

	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	void SetGrabbed(Entity* ptr);

	//Returns the old speed
	float SetSpeed(float speed);
	void SetLookDir(DirectX::XMVECTOR lookDir);
	void SetUpDir(DirectX::XMVECTOR upDir);
	void SetRightDir(DirectX::XMVECTOR rightDir);
private:

};

#endif