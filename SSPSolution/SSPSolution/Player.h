#ifndef SSPAPPLICATION_ENTITIES_PLAYER_H
#define SSPAPPLICATION_ENTITIES_PLAYER_H

#include "..\GraphicsDLL\AnimationStateEnums.h"
#include "Entity.h"

class Player :
	public Entity
{
private:
	//Meters per second
	float m_acceleration;
	float m_maxSpeed;
	float m_throwStrength;
	bool m_isAiming;
	Entity* m_ball;
	Entity* m_grabbed;
	DirectX::XMVECTOR m_carryOffset;

	DirectX::XMVECTOR m_lookDir;
	DirectX::XMVECTOR m_upDir;
	DirectX::XMVECTOR m_rightDir;

	irrklang::ISound* m_walkingSound;
	int	m_oldAnimState;
	float m_timeSinceThrow;

	UIComponent* m_controlsOverlay;

public:
	Player();
	~Player();

	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, AnimationComponent* aComp, ComponentHandler* cHandler);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	//Returns the old value
	Entity* SetGrabbed(Entity* entityPtr);
	float SetMaxSpeed(float speed);
	float SetAcceleration(float acceleration);
	DirectX::XMVECTOR SetLookDir(DirectX::XMVECTOR lookDir);
	DirectX::XMVECTOR SetUpDir(DirectX::XMVECTOR upDir);
	DirectX::XMVECTOR SetRightDir(DirectX::XMVECTOR rightDir);
	void SetAiming(bool isAming);
	void SetBall(Entity* ball);

	bool stateExists(int animationState);
	void SetAnimationComponent(int animationState, float transitionDuration, Blending blendingType, bool isLooping, bool lockAnimation, float playingSpeed, float velocity);

	float GetMaxSpeed();
	float GetAcceleration();
	DirectX::XMVECTOR GetLookDir();
	DirectX::XMVECTOR GetUpDir();
	DirectX::XMVECTOR GetRightDir();
	bool GetIsAming();
	Entity* GetGrabbed();
	Entity* GetBall();
	bool isAnimationChanged();	//Compares the current Animation State against the previous frame's Animation State 
	float TimeSinceThrow();

	void Shutdown(ComponentHandler* cHandler);

private:

};

#endif