#ifndef SSPAPPLICATION_ENTITIES_PLAYER_H
#define SSPAPPLICATION_ENTITIES_PLAYER_H

#include "..\GraphicsDLL\AnimationStateEnums.h"
#include "Entity.h"

#define BLEND_ROTATION_DURATION 0.50f

class Player :
	public Entity
{
private:
	//Meters per second
	float m_acceleration;
	float m_maxSpeed;
	float m_throwStrength;
	float m_blendTimer;
	bool m_isAiming;
	bool m_playerIsRotating;
	Entity* m_ball;

	Ragdoll* m_ragdoll;
	Entity* m_grabbed;
	DirectX::XMVECTOR m_carryOffset;
	DirectX::XMVECTOR m_anklePos;

	DirectX::XMVECTOR m_lookDir;
	DirectX::XMVECTOR m_upDir;
	DirectX::XMVECTOR m_rightDir;
	DirectX::XMVECTOR m_newPlayerDir;
	
#pragma region soundvariables
	bool m_hasBeenThrown;
	irrklang::ISound* m_thrownSound = nullptr;
	irrklang::ISound* m_walkingSound;
#pragma endregion

	float m_chainSoundTimer;
	float m_timeSinceThrow;
	bool isAbbington;

public:
	Player();
	~Player();

	int Initialize(unsigned int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, AnimationComponent* aComp);

	int Update(float dT, InputHandler* inputHandler);
	int React(unsigned int entityID, EVENT reactEvent);

	//Returns the old value
	Entity* SetGrabbed(Entity* entityPtr);
	float SetMaxSpeed(float speed);
	float SetAcceleration(float acceleration);
	DirectX::XMVECTOR SetLookDir(DirectX::XMVECTOR lookDir);
	DirectX::XMVECTOR SetUpDir(DirectX::XMVECTOR upDir);
	DirectX::XMVECTOR SetRightDir(DirectX::XMVECTOR rightDir);
	void SetAiming(bool isAming);
	void SetBall(Entity* ball);
	void SetRagdoll(Ragdoll* ragdoll);

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
	Ragdoll* GetRagdoll();
	bool isAnimationChanged();	//Compares the current Animation State against the previous frame's Animation State 
	float TimeSinceThrow();

	DirectX::XMVECTOR GetAnklePosition(); // position for where the chain is. 


private:

};

#endif